# Crash Report — `crash_example.c`

> **AI usage note**: This report was produced with AI assistance. All claims were
> verified directly against GDB and Valgrind output (Section 2) rather than taken
> on faith. Section 5 documents where an AI's proposed explanation was speculative
> or wrong, as required.

## 1. Description of the crash

Running the built binary terminates with a segmentation fault:

```
$ ./crash_example
crash_example: deterministic NULL dereference (segmentation fault)
  requesting n=0
Segmentation fault (core dumped)
$ echo $?
139
```

Exit status `139` = `128 + 11`, where `11` is `SIGSEGV`. The crash is
reproducible on every run with no variation — there is no race, no
uninitialized-value dependency, and no environment sensitivity involved.

## 2. Direct evidence (no re-running required to explain it further)

**GDB**, run once to capture the state at the fault, confirms the exact
faulting instruction and the value of every relevant variable:

```
Program received signal SIGSEGV, Segmentation fault.
0x000055555555528a in main () at crash_example.c:32
32          nums[0] = 42;
nums = 0x0
n = 0
```

**Valgrind** (`memcheck`), run once, independently confirms the same fault
and adds the classification of the access:

```
Invalid write of size 4
   at 0x10928A: main (crash_example.c:32)
 Address 0x0 is not stack'd, malloc'd or (recently) free'd
Process terminating with default action of signal 11 (SIGSEGV)
 Access not within mapped region at address 0x0
```

Both tools agree, independently, on:
- the faulting line (`crash_example.c:32`),
- the faulting address (`0x0`),
- the faulting operation (a 4-byte **write**).

This is the evidence base for the rest of this report — no further runs are
needed to justify the conclusions below.

## 3. Root cause analysis (causal chain)

```c
static int *allocate_numbers(int n)
{
    int *arr = NULL;
    ...
    if (n <= 0)
        return NULL;          /* (A) */
    arr = (int *)malloc((size_t)n * sizeof(int));
    ...
    return arr;
}

int main(void)
{
    int *nums = NULL;
    int n = 0;                /* (B) */

    nums = allocate_numbers(n);   /* (C) */

    nums[0] = 42;              /* (D) — crash site */
    ...
}
```

The causal chain, in order:

1. **(B)** `n` is initialized to `0` in `main`.
2. **(C)** `allocate_numbers(0)` is called. Inside it, the guard clause
   **(A)**, `if (n <= 0) return NULL;`, evaluates true and the function
   returns immediately. Crucially, `malloc` is **never called** on this
   path — there is no heap object involved in this crash at all.
3. Back in `main`, `nums` is assigned the return value: `nums = NULL`
   (`0x0`), confirmed directly by GDB's `nums = 0x0`.
4. **main never checks whether `allocate_numbers` returned `NULL`.** The
   function's contract (a `NULL` return signaling "no allocation happened")
   is silently discarded by the caller.
5. **(D)** `nums[0] = 42;` is evaluated as `*(nums + 0) = 42`, i.e. a
   4-byte write to address `0x0 + 0 = 0x0`.
6. Address `0x0` is deliberately kept unmapped by the OS/loader in a normal
   process specifically so that null-pointer accesses fault immediately
   instead of silently corrupting arbitrary memory. The CPU's page-table
   walk for that write finds no valid mapping, the kernel raises a page
   fault, and delivers `SIGSEGV` to the process, which terminates it by
   default.

Every step here is fully determined by the value `n = 0`; nothing about the
crash depends on the state of the heap, the stack layout, or any prior
execution history.

## 4. Why the memory access is invalid

- **Category of undefined behavior**: NULL-pointer dereference (specifically
  a **write** through a null pointer), arising from an **unchecked failure
  return value**.
- **Memory region involved**: neither the stack nor the heap, precisely. The
  access targets address `0x0`, which is not part of any valid memory
  segment of the process (not stack, not heap, not `.data`/`.bss`, not a
  mapped file) — Valgrind states this explicitly: *"Address 0x0 is not
  stack'd, malloc'd or (recently) free'd."* The bug is a failure to ever
  obtain valid memory in the first place, not a lifetime violation on
  memory that once existed. This distinguishes it from the use-after-free
  and leak bugs analyzed elsewhere in this project, both of which involve
  a real heap object with a lifetime that was mishandled.
- **Why the language allows this to happen**: C does not enforce checking a
  pointer for `NULL` before dereferencing it. `malloc`-family functions and
  functions like `allocate_numbers` here communicate failure by convention
  (returning `NULL`), and the compiler cannot force the caller to check
  that convention — `-Wall -Wextra -Werror -pedantic` do not catch this,
  because syntactically `nums[0] = 42;` is perfectly valid C; only its
  *runtime value* makes it invalid.

## 5. AI-proposed causes and fixes — critical evaluation

**Prompt given to a general-purpose LLM assistant:** "Why does
`crash_example.c` segfault?"

**AI's proposed explanations (paraphrased):**

1. *"The array `arr` inside `allocate_numbers` may be written out of bounds
   in the `for` loop, corrupting the heap and causing the crash later in
   `main`."*
2. *"`nums[0] = 42` dereferences a null pointer because `allocate_numbers`
   returned `NULL`."*
3. *Suggested fix: "Check `if (nums == NULL) return 1;` immediately after
   the call to `allocate_numbers`, or `-1` for the same effect."*

**Evaluation:**

- **Explanation 1 is incorrect and speculative.** With `n = 0`, the guard
  clause at line 11 (`if (n <= 0) return NULL;`) returns before the loop is
  ever reached, so the loop body never executes at all on this run — there
  is no out-of-bounds write in `allocate_numbers` to evaluate. This
  explanation reads like a plausible-sounding generic guess about array
  code ("loops can overflow buffers") rather than a claim grounded in the
  actual value of `n` for this specific run. Verifying it required reading
  the guard clause and confirming `n = 0` via GDB (`info locals`), which
  showed the loop path was never taken.
- **Explanation 2 is correct**, and matches both the GDB (`nums = 0x0`) and
  Valgrind ("Address 0x0 is not stack'd, malloc'd or (recently) free'd")
  evidence directly.
- **The suggested fix is directionally correct but incomplete as stated.**
  Checking `nums == NULL` after the call does stop the crash, but the AI's
  phrasing treated the check as arbitrary error-handling boilerplate rather
  than tying it to *why* `allocate_numbers` can return `NULL` in the first
  place (i.e., that `n <= 0` is a legitimate, expected input the function
  is designed to reject). A more precise fix also validates `n` before or
  instead of relying solely on the post-hoc `NULL` check — see Section 6.

**Takeaway**: the AI got the final diagnosis right but arrived there partly
via an unverified, incorrect alternative hypothesis (explanation 1) offered
with equal confidence. Cross-checking each claim against a single GDB/
Valgrind run — not re-running the crash repeatedly, just reading the one
capture closely — was what separated the correct explanation from the
speculative one.

## 6. Suggested fix (optional, clearly labeled)

> The following is a suggested fix, not a required code change per the task
> instructions. The code itself was not modified for this report.

```c
nums = allocate_numbers(n);
if (nums == NULL)
{
    fprintf(stderr, "allocate_numbers: allocation failed or n <= 0\n");
    return 1;
}
```

This directly closes the gap identified in Section 3 step 4: it makes the
caller respect the `NULL`-on-failure contract instead of silently
discarding it. It does not change the behavior for `n = 0` in the sense
that the program still cannot compute with zero numbers — it simply fails
*safely and legibly* instead of crashing with an unmapped-address write.
