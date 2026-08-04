# Valgrind Analysis — AI Memory Visualizer & Valgrind Tracer

> **Note on AI usage**: This draft was produced with AI assistance running actual
> Valgrind on the provided programs. All findings below were verified against the
> Valgrind logs and the source code line-by-line before being written up. One
> deliberately-flagged AI misreading is documented in Section 4, as required.

Build flags used: `-Wall -Wextra -Werror -pedantic -std=gnu89 -g`
Valgrind flags used: `--leak-check=full --show-leak-kinds=all --track-origins=yes`

---

## 1. `aliasing_example.c` — Use-after-free via pointer aliasing

### Compile-time signal (before Valgrind was even run)
GCC 13's `-Wuse-after-free` (enabled by `-Wall -Wextra`, and fatal under `-Werror`)
refused to compile the program as-is, flagging every access to `b` after
`free(a)` at line 38. This matters conceptually: the compiler can sometimes
prove use-after-free statically when the freed pointer and the later-used
pointer are visibly the same object in one function. This is **not a substitute
for Valgrind** — it only works when the alias is locally visible to the
compiler. If `b` had been passed to another translation unit, or the free
happened through a function call, GCC would not catch it, and Valgrind would
still be necessary.

### Valgrind output

```
Invalid read of size 4
   at 0x1092F5: main (aliasing_example.c:42)
 Address 0x4a7d088 is 8 bytes inside a block of size 20 free'd
   at 0x484988F: free (in vgpreload_memcheck-amd64-linux.so)
   by 0x1092D1: main (aliasing_example.c:38)
 Block was alloc'd at
   by 0x1091E4: make_numbers (aliasing_example.c:12)
   by 0x109272: main (aliasing_example.c:30)

Invalid write of size 4
   at 0x109315: main (aliasing_example.c:44)
 Address 0x4a7d08c is 12 bytes inside a block of size 20 free'd
   (same free/alloc backtrace)

Invalid read of size 4
   at 0x109323: main (aliasing_example.c:45)
 Address 0x4a7d08c is 12 bytes inside a block of size 20 free'd
   (same free/alloc backtrace)

HEAP SUMMARY: in use at exit: 0 bytes in 0 blocks
All heap blocks were freed -- no leaks are possible
ERROR SUMMARY: 3 errors from 3 contexts
```

### Memory reasoning

- `make_numbers()` heap-allocates a 20-byte block (`5 * sizeof(int)`), returns it,
  and `main` stores the address in `a`.
- `b = a;` does **not** create a second allocation — it copies the pointer
  value. `a` and `b` now **alias** the same 20-byte block. There is exactly
  one heap object and one owner responsibility, but two names for it.
- `free(a);` at line 38 ends the lifetime of that block. The allocator is free
  to reuse, unmap, or poison that memory at this point. Ownership was
  discharged through `a`, but `b` is left dangling — it still holds the same
  address, with no lifetime backing it.
- Line 42 (`b[2]`) and line 45 (`b[3]`) are reads through the dangling pointer;
  line 44 (`b[3] = 1234`) is a write through it. Each is flagged individually
  because Valgrind reports every invalid access, not just the first.
- This is **not a double-free**: `free()` is called exactly once, on `a`. The
  bug is a **use-after-free through an alias**, not a repeated deallocation.
  The two are easy to conflate but are distinct memory-safety violations (see
  Section 4).
- No leak is reported (`0 bytes in 0 blocks`) because the block *was* freed —
  the problem is *when it's accessed relative to* that free, not whether it
  was freed at all.

### Classification
**Use-after-free (dangling pointer access via an aliased reference).**
Ownership was correctly discharged once, through `a`; the bug is that `b`,
an alias created before the free, was used afterward with no way to know the
memory backing it was gone.

---

## 2. `heap_example.c` — Leak due to lost ownership (partial free)

### Valgrind output

```
HEAP SUMMARY:
    in use at exit: 6 bytes in 1 blocks
    total heap usage: 5 allocs, 4 frees, 4,138 bytes allocated

6 bytes in 1 blocks are definitely lost in loss record 1 of 1
   at 0x4846828: malloc
   by 0x109211: person_new (heap_example.c:21)
   by 0x1092FA: main (heap_example.c:51)

LEAK SUMMARY:
   definitely lost: 6 bytes in 1 blocks
ERROR SUMMARY: 1 errors from 1 contexts
```

### Memory reasoning

- `person_new()` performs **two** allocations per `Person`: the `Person`
  struct itself, and a separately-owned heap buffer for `p->name`
  (`malloc(len + 1)`). A `Person*` therefore represents two independent
  allocations chained by a pointer field, each needing its own `free`.
- For `bob`, `main` correctly frees both: `free(bob->name); free(bob);` —
  the name buffer first (before the struct that holds the only pointer to
  it disappears), then the struct.
- For `alice`, `main` calls `person_free_partial(alice)`, which only does
  `free(p)`. It never frees `p->name`. Once `free(p)` runs, the last
  reachable pointer to `alice->name` is destroyed along with the struct that
  held it — the 6-byte block (`"Alice\0"`, 5 chars + null terminator) becomes
  unreachable but was never deallocated. That is exactly what Valgrind calls
  "definitely lost": no pointer to the block exists anywhere in the program
  at exit, and it was never freed.
- The size confirms the target precisely: `strlen("Alice") + 1 = 6`, matching
  the "6 bytes in 1 blocks" report and the allocation site at
  `heap_example.c:21` (the `p->name = malloc(len + 1)` call inside
  `person_new`), reached this time via the `alice = person_new(...)` call
  site at line 51 rather than the `bob` call site.

### Classification
**Memory leak due to lost ownership.** `person_free_partial()` assumes it
owns and fully releases a `Person`, but only releases the outer struct,
silently dropping the caller's only reference to the inner `name` allocation
before that allocation is freed.

---

## 3. `crash_example.c` — Deterministic NULL-pointer dereference (SIGSEGV)

### Valgrind output

```
Invalid write of size 4
   at 0x10928A: main (crash_example.c:32)
 Address 0x0 is not stack'd, malloc'd or (recently) free'd

Process terminating with default action of signal 11 (SIGSEGV)
 Access not within mapped region at address 0x0
   at 0x10928A: main (crash_example.c:32)
```
(Exit status observed directly: `139` = `128 + SIGSEGV(11)`.)

### Memory reasoning

- `main` calls `allocate_numbers(n)` with `n = 0`.
- Inside `allocate_numbers`, the guard `if (n <= 0) return NULL;` fires
  immediately — no `malloc` occurs for this call, and the function returns
  the null pointer before ever reaching the allocation or the loop.
- `nums` in `main` is therefore `NULL`.
- Line 32, `nums[0] = 42;`, is `*(nums + 0) = 42`, i.e. a write to address
  `0x0`. Address 0 is never a valid mapped page in a normal process (it's
  reserved specifically to make this class of bug fault immediately rather
  than silently corrupt memory), so the kernel delivers `SIGSEGV`.
- This is fully deterministic given `n = 0`: there is no race, no
  uninitialized-value dependency, and no undefined-order issue. The causal
  chain is: `n = 0` → guard clause returns `NULL` → `nums = NULL` in caller
  → unchecked dereference of `NULL` → invalid write to unmapped page 0 →
  `SIGSEGV`.
- The unrelated `4,096 bytes ... still reachable` in the heap summary is
  glibc's internal stdio buffer (allocated lazily by the first buffered
  `printf`), not an application allocation — it is reachable, not lost, and
  irrelevant to the crash.

### Classification
**NULL-pointer dereference (write), caused by an unchecked return value.**
`main` never checks whether `allocate_numbers` returned `NULL` before
indexing into it, so the library-level "failure" signal (`NULL`) is treated
as a valid pointer.

---

## 4. Documented AI misreading

**Prompt given to a general-purpose LLM assistant:** "Why does Valgrind flag
`aliasing_example.c`?"

**AI's response (paraphrased):** the assistant explained the invalid
reads/writes as being caused by *"a double free — `a` and `b` both point to
the same block, and freeing it through both names corrupts the heap."*

**Why this is incorrect:** `free()` is called exactly once in the program,
on `a`, at line 38. `b` is never passed to `free()` at all — it is only read
from and written to *after* `a`'s free. A double-free would require two
`free()` calls on the same block (or overlapping blocks) and would show up
in Valgrind as an "Invalid free() / delete / delete[]" error referencing a
*second* call to `free`. What the log actually shows is "Invalid read/write
... N bytes inside a block ... free'd" from ordinary array-indexing
operations (`b[2]`, `b[3] = ...`), which is the signature of use-after-free,
not double-free.

The AI conflated two related-but-distinct bug classes because both involve
"freed memory + a second pointer," but the actual operations performed on
that second pointer differ: **calling `free()` again** (double-free) versus
**reading/writing through it** (use-after-free). Verifying against the exact
Valgrind error text (`Invalid read`/`Invalid write`, not `Invalid free()`)
and against the source (only one `free()` call exists) was what caught the
error.

---

## Summary table

| Program              | Error type                     | Object involved              | Root cause |
|----------------------|---------------------------------|-------------------------------|------------|
| aliasing_example.c   | Use-after-free (read + write)  | 20-byte `int[5]` heap block    | `b` aliases `a`; accessed after `free(a)` |
| heap_example.c       | Memory leak (definitely lost)  | 6-byte `name` buffer for alice | `person_free_partial()` frees struct, not `p->name` |
| crash_example.c      | NULL-pointer dereference       | `nums` (never allocated)       | Unchecked `NULL` return from `allocate_numbers(0)` |
