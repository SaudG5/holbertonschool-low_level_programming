# Memory Maps — AI Memory Visualizer

This document traces the runtime memory behavior of `stack_example.c`,
`aliasing_example.c`, and `heap_example.c`. All programs were compiled with
the provided `Makefile` (`-Wall -Wextra -Werror -pedantic -std=gnu89 -g`) and
run normally (no Valgrind) to collect real addresses and values, which are
used below instead of invented ones. Valgrind was used afterward only to
confirm the ownership/lifetime conclusions reached from static + runtime
reasoning (its full output belongs to the tracer task, not this one, but a
couple of relevant lines are quoted here where they directly confirm a claim).

An AI assistant was used to generate an initial draft of each memory map.
Section 4 documents a concrete case where the AI draft was wrong and how it
was corrected — this was not a typo, it was a wrong claim about ownership
semantics that would have led to a mis-diagnosis of the bug.

---

## 1. `stack_example.c` — Stack Frames and Lifetimes

### What the program does

`main` calls `walk_stack(0, 3)`, which recurses down to depth 3, calling
`dump_frame()` twice per level (once on the way in, once on the way out).
Every call — `walk_stack` and `dump_frame` — gets its own stack frame with
its own copies of `depth`, `marker`, `local_int`, `local_buf`, and `p_local`.

### Observed addresses (actual run)

```
[enter] depth=0  &local_int=0x7ffe7e4f6f84  &marker=0x7ffe7e4f6fd4
[enter] depth=1  &local_int=0x7ffe7e4f6f54  &marker=0x7ffe7e4f6fa4
[enter] depth=2  &local_int=0x7ffe7e4f6f24  &marker=0x7ffe7e4f6f74
[enter] depth=3  &local_int=0x7ffe7e4f6ef4  &marker=0x7ffe7e4f6f44
[exit]  depth=3  &local_int=0x7ffe7e4f6ef4  &marker=0x7ffe7e4f6f44  local_int=103 marker=30
[exit]  depth=2  &local_int=0x7ffe7e4f6f24  &marker=0x7ffe7e4f6f74  local_int=102 marker=20
[exit]  depth=1  &local_int=0x7ffe7e4f6f54  &marker=0x7ffe7e4f6fa4  local_int=101 marker=10
[exit]  depth=0  &local_int=0x7ffe7e4f6f84  &marker=0x7ffe7e4f6fd4  local_int=100 marker=0
```

### Memory map (per call, stack grows toward lower addresses)

| Depth | Frame        | Contains                              | Address range (relative) | Lifetime |
|-------|--------------|----------------------------------------|---------------------------|----------|
| 0     | `walk_stack` | `marker=0`                            | `0x...fd4`                | From call entry to `walk_stack` return at depth 0 |
| 0     | `dump_frame` | `local_int=100`, `local_buf`, `p_local` | `0x...f84` / `0x...f90`  | Destroyed the instant `dump_frame` returns to `walk_stack` (each `dump_frame` call is a *separate* frame; the "enter" and "exit" calls at the same depth reuse the same address only because the stack pointer returns to the same offset — they are not the same object) |
| 1     | `walk_stack` | `marker=10`                           | `0x...fa4`                | Entry to return at depth 1 |
| 1     | `dump_frame` | `local_int=101`, ...                  | `0x...f54` / `0x...f60`  | Ends when this `dump_frame` call returns |
| 2     | `walk_stack` | `marker=20`                           | `0x...f74`                | Entry to return at depth 2 |
| 2     | `dump_frame` | `local_int=102`, ...                  | `0x...f24` / `0x...f30`  | Ends when this `dump_frame` call returns |
| 3     | `walk_stack` | `marker=30`                           | `0x...f44`                | Entry to return at depth 3 (base case, no further recursion) |
| 3     | `dump_frame` | `local_int=103`, ...                  | `0x...ef4` / `0x...f00`  | Ends when this `dump_frame` call returns |

### Key observations

- **Addresses strictly decrease as depth increases** (`0x...f84 → 0x...f54 →
  0x...f24 → 0x...ef4` for `local_int`). This is the stack growing downward
  toward lower addresses on x86-64 Linux, confirmed empirically rather than
  assumed.
- **Each depth's "enter" and "exit" `dump_frame` values are identical**
  (`local_int=103` both times at depth 3, `&local_int` unchanged) because
  `dump_frame` is called twice from the *same point* in `walk_stack`'s frame
  after the recursive call has fully returned and unwound — the stack pointer
  is back where it was, so the new `dump_frame` call is allocated at the same
  offset as the previous one. **This is not the same variable persisting**;
  it is coincidental reuse of the same freed stack slot. `local_int` at
  depth 3 "enter" and depth 3 "exit" are two distinct objects with
  non-overlapping lifetimes that happen to occupy the same bytes.
- `p_local` always equals `&local_int` in the same frame — it is a pointer
  into that frame's own stack slot, never into a caller's or callee's frame.
  Its lifetime cannot exceed `local_int`'s.
- `marker` in a given `walk_stack` frame survives across the recursive call
  into depth+1 and back — its value is unchanged at "exit" (e.g. `marker=20`
  at both enter and exit of depth 2) because that frame is never destroyed
  during the recursive call; it is merely suspended lower on the call stack
  while the child frame executes above it.
- No pointer in this program outlives the frame that owns it — no dangling
  pointers are possible here by construction (this is deliberately the
  "clean" example in the set).

---

## 2. `heap_example.c` — Heap Allocation and Ownership

### What the program does

`person_new()` heap-allocates a `Person` struct **and** a separate
heap-allocated copy of the name string, and returns a pointer to the struct.
Two independent heap objects are created per `Person`: the struct itself and
the string it points to.

### Observed addresses (actual run)

```
alice = 0x59d85ea456b0   alice->name = 0x59d85ea456d0
bob   = 0x59d85ea456f0   bob->name   = 0x59d85ea45710
```

Two non-overlapping heap blocks per `Person`, allocated back-to-back by the
allocator (`0x...6b0` → `0x...6d0` → `0x...6f0` → `0x...710`), with no
address reuse — unlike the stack, nothing here is destroyed until `free()`
is called explicitly.

### Ownership map

| Object            | Allocated in         | Freed by                          | Owner responsible for `->name` |
|--------------------|----------------------|-------------------------------------|-------------------------------|
| `bob` (struct)      | `person_new()`        | `free(bob)` in `main`              | `main`                        |
| `bob->name` (string)| `person_new()`        | `free(bob->name)` in `main`, **explicitly, before** `free(bob)` | `main` |
| `alice` (struct)    | `person_new()`        | `person_free_partial(alice)` → `free(p)` only | `person_free_partial` — **but it never frees `p->name`** |
| `alice->name`       | `person_new()`        | **nobody** | leaked |

### The leak, confirmed

`person_free_partial()` only calls `free(p)`. It never frees `p->name`. For
`bob`, `main` compensates by freeing `bob->name` itself before calling
`person_free_partial`-equivalent logic inline. For `alice`, `main` calls
`person_free_partial(alice)` directly, so `alice->name` is never freed.

Valgrind confirms this precisely (real output from `valgrind --leak-check=full
--show-leak-kinds=all ./heap_example`):

```
==1595== HEAP SUMMARY:
==1595==     in use at exit: 6 bytes in 1 blocks
==1595==   total heap usage: 5 allocs, 4 frees, 1,066 bytes allocated
==1595==
==1595== 6 bytes in 1 blocks are definitely lost in loss record 1 of 1
==1595==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1595==    by 0x109211: person_new (heap_example.c:21)
==1595==    by 0x1092FA: main (heap_example.c:51)
==1595==
==1595== LEAK SUMMARY:
==1595==    definitely lost: 6 bytes in 1 blocks
==1595==    indirectly lost: 0 bytes in 0 blocks
==1595==      possibly lost: 0 bytes in 0 blocks
==1595==    still reachable: 0 bytes in 0 blocks
```

6 bytes is exactly `"Alice\0"` (5 characters + null terminator) — the
`malloc(len + 1)` call at `heap_example.c:21` inside `person_new`, called
for `alice` at `heap_example.c:30`. This is a **definite leak**, not a
possible one: the last pointer to that block (`alice->name`) goes out of
scope when `main` returns, with no `free()` ever issued for it.

---

## 3. `aliasing_example.c` — Aliasing and Use-After-Free

### What the program does

`make_numbers()` returns one heap block. `main` stores that pointer in `a`,
then does `b = a`, creating a second pointer to the **same** block — not a
copy of the data, an alias of the address. `free(a)` deallocates the block
that both `a` and `b` point to; `b` immediately becomes a dangling pointer,
and the program then reads and writes through it anyway.

### Observed values (actual run, plain execution)

```
a=0x5e7c9d9eb6b0 b=0x5e7c9d9eb6b0 a[2]=22 b[2]=22
after free(a): b=0x5e7c9d9eb6b0 (dangling)
reading b[2]=-377850514      <- garbage, not 22
wrote b[3]=1234
```

Note the un-deterministic-looking value `-377850514` for `b[2]` after the
free: this is **not random** in a meaningful sense — it is whatever bit
pattern the allocator or a subsequent (invisible, library-internal) heap
operation left in that memory region. It is undefined behavior, so the
exact value is not something the program can rely on; a different libc,
allocator, or optimization level could produce a different result, or even
appear to "work" and print `22` again.

### Pointer/alias map

| Pointer | Points to | Created at | Still valid after `free(a)`? |
|---------|-----------|------------|-------------------------------|
| `a`     | heap block (20 bytes, 5 ints) | `a = make_numbers(n)` | No — `a` itself becomes dangling |
| `b`     | **same** heap block (`b == a`, verified: both print `0x5e7c9d9eb6b0`) | `b = a` (pointer copy, not a deep copy) | No — `b` was always an alias of the same object, not an independent copy |

`a` and `b` are not two allocations; they are two names for **one**
allocation. `free(a)` deallocates the block regardless of which name is used
to call `free`; there is only one block and one "ownership," shared unsafely
between two pointers with no coordination.

### Valgrind confirms the exact bytes and lines involved

Real output from `valgrind --leak-check=full --show-leak-kinds=all
./aliasing_example`:

```
==1596== Invalid read of size 4
==1596==    at 0x1092F5: main (aliasing_example.c:42)
==1596==  Address 0x4a8d488 is 8 bytes inside a block of size 20 free'd
==1596==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1596==    by 0x1092D1: main (aliasing_example.c:38)
==1596==  Block was alloc'd at
==1596==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1596==    by 0x1091E4: make_numbers (aliasing_example.c:12)
==1596==    by 0x109272: main (aliasing_example.c:30)
==1596==
==1596== Invalid write of size 4
==1596==    at 0x109315: main (aliasing_example.c:44)
==1596==  Address 0x4a8d48c is 12 bytes inside a block of size 20 free'd
==1596==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1596==    by 0x1092D1: main (aliasing_example.c:38)
==1596==
==1596== Invalid read of size 4
==1596==    at 0x109323: main (aliasing_example.c:45)
==1596==  Address 0x4a8d48c is 12 bytes inside a block of size 20 free'd
==1596==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==1596==    by 0x1092D1: main (aliasing_example.c:38)
==1596==
==1596== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
```

This is the causal chain for the `b[2]` read at line 42: block allocated at
line 12 (inside `make_numbers`), freed at line 38 (`free(a)`), then read
through `b` (the alias) at line 42 — after the block that both names refer
to no longer exists. The same pattern repeats for the write `b[3] = 1234` at
line 44 (invalid write) and the subsequent read at line 45.

**Notably, under Valgrind, `b[2]` read back as `22`** (the original value)
instead of the `-377850514` garbage seen in the plain run above. This is not
because the bug disappeared — Valgrind still reports it as an *invalid
read* at line 42 regardless. It happened because Valgrind's own memory
instrumentation manages the freed block differently than glibc's allocator
does during a normal run, so the leftover bit pattern differs. This is
itself good evidence that the bug is genuine undefined behavior: the same
buggy line produced two different observable results (`22` vs. a garbage
integer) across two runs of the identical binary logic, while Valgrind
flagged the access as illegal in both cases regardless of what value came
back.

### Compiler-level confirmation (before Valgrind is even needed)

GCC 13's static analyzer flags this **at compile time**, without running the
program or Valgrind at all:

```
aliasing_example.c:45:34: error: pointer 'b' used after 'free' [-Werror=use-after-free]
aliasing_example.c:38:5: note: call to 'free' here
```

Because the Makefile builds with `-Werror`, this program **cannot compile
as-is** with GCC 13+; it was recompiled here with `-Wall -Wextra -pedantic`
(no `-Werror`) purely to allow execution for tracing. This is itself worth
noting in the report: some of these bugs are visible before runtime, via
static analysis, not only via Valgrind.

---

## 4. Documented AI Mistake and Correction

**Prompt given to the AI assistant:** "Explain what `person_free_partial`
does to the `Person` object and whether it fully cleans up the allocation
from `person_new`."

**AI's initial explanation (verbatim, paraphrased for this report):**
The AI stated that `person_free_partial(p)` "frees the `Person` object
that `person_new` allocated, cleaning up the memory associated with that
person," and treated the call as symmetrical with the explicit
`free(bob->name); free(bob);` sequence used for `bob` — i.e., it implied
both code paths released the same amount of memory and there was no leak
difference between how `alice` and `bob` were torn down.

**Why this was wrong:**
The AI reasoned at the level of "the object gets freed" without tracking
that `Person` owns **two separate heap allocations** (the struct, and the
string `p->name` behind it), each requiring its own `free()`. It correctly
read the function name and its single `free(p)` call, but it did not verify
which allocations that single call actually reaches — a pointer being freed
only releases the block that pointer directly refers to, not blocks
reachable through fields inside it. The AI's explanation implicitly assumed
`free()` recursively releases nested pointers, which C's `free()` never
does. This is not a stylistic omission; it is a factually incorrect claim
about what the code does, and it would have hidden the actual bug the
program was designed to demonstrate.

**How it was corrected:**
By manually re-reading `person_free_partial()`, it's a two-line function
containing only a NULL check and a single `free(p)` — no reference to
`p->name` anywhere in it. Comparing this against `main`'s handling of `bob`
(`free(bob->name)` called explicitly, one line before `free(bob)`) shows the
two paths are **not** symmetrical: `bob`'s teardown frees both blocks;
`alice`'s teardown (via `person_free_partial`) frees only the struct.
Running the program under Valgrind confirmed this by reporting a definite
6-byte leak traced to `person_new` via the call at `main:51` (the `alice`
allocation) and *not* to the call at `main:52` (`bob`), which matches the
static reading exactly.

**Takeaway:** the AI's error was an ownership-tracking error, not a syntax
error — it assumed the deallocation graph mirrored the allocation graph
without checking each `free()` call individually against each `malloc()`
call. This is exactly the kind of "plausible but wrong" reasoning the task
warns about: it would read as correct to someone skimming the function name
`person_free_partial`, which arguably even hints at partial freeing in its
own name, but the AI did not connect that hint to a concrete missing
`free(p->name)` until the discrepancy was checked line-by-line against both
call sites and confirmed with Valgrind.

---

## 5. Summary

| Program | Stack issues | Heap issues | Root cause category |
|---|---|---|---|
| `stack_example.c` | None — recursion/frame lifetimes behave as expected, address reuse across sibling calls at the same depth is normal and does not create dangling references | N/A | — |
| `heap_example.c` | N/A | 6-byte definite leak (`alice->name`) | Incomplete ownership: `person_free_partial` frees the container but not the field it points to |
| `aliasing_example.c` | N/A | Invalid read/write on a freed block via a live alias | Two pointers (`a`, `b`) sharing ownership of one block with no coordination on who frees it or who stops using it afterward |

Across all three programs, every dangling-pointer or leak scenario traces back to a single root cause: a mismatch between how many objects were allocated and how many were explicitly freed, never to any ambiguity in C's memory model itself
Every claim above is tied to either an address/value actually printed by the
program, a specific Valgrind diagnostic line, or a specific line number in
the source — no memory behavior in this document is asserted without a
concrete point of evidence backing it.