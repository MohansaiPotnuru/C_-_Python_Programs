# 50 Advanced C & 50 Advanced Python Programs

[![CI](https://github.com/MohansaiPotnuru/C_-_Python_Programs/actions/workflows/ci.yml/badge.svg)](https://github.com/MohansaiPotnuru/C_-_Python_Programs/actions/workflows/ci.yml)

100 short, runnable programs covering the C and Python features that come up in embedded, systems and interview work. Every program is one small function or class, and each file's `main` runs all 50 with self-checks, so the files are both a reference and a test suite.

| File | Language | Run |
|---|---|---|
| [`c/advanced_c_programs.c`](c/advanced_c_programs.c) | C11 | `gcc -std=c11 -Wall -Wextra -O2 c/advanced_c_programs.c -o advanced_c && ./advanced_c` |
| [`python/advanced_python_programs.py`](python/advanced_python_programs.py) | Python 3.10+ | `python python/advanced_python_programs.py` |

On every push, CI builds the C file with **GCC and Clang** (`-Wall -Wextra -Wpedantic -Werror`), runs it under **AddressSanitizer + UBSan**, and runs the Python file on **3.10, 3.12 and 3.13**.

## C programs

| # | Section | Topics |
|---|---|---|
| 1–10 | Bitwise tricks | XOR swap (with aliasing guard), power-of-two test, Kernighan bit count, parity, single-number XOR, shifts, branch-free `abs` and sign |
| 11–20 | Control flow | `goto` out of nested loops, Duff's device, `for(;;)`, two-index loops, `setjmp`/`longjmp` exceptions, tail recursion, sequence points, `do { } while (0)` macros, switch fall-through |
| 21–30 | Pointers & memory | `void*` generics, pointer arithmetic, double-pointer allocation, function-pointer tables, safe `realloc`, `const` pointer rules, endianness, safe `free`, custom `memcpy`, guarded VLAs |
| 31–40 | Types | Bit-fields, `#pragma pack`, flexible array members, union type-punning, linked lists, C11 anonymous structs, opaque handles, `offsetof`, `container_of`, flag enums |
| 41–50 | Preprocessor & system | `#` / `##`, `__VA_ARGS__`, `<stdarg.h>`, `volatile`, `static` locals, `strtok`, `_Static_assert`, `restrict`, `aligned_alloc` (portable to Windows) |

Sample output:

```text
Section 1: bitwise
  [9] Condition met!
...
  [32] normal=8 bytes, packed=5 bytes
...
All 50 programs ran - 47 checks passed.
```

## Python programs

| # | Section | Topics |
|---|---|---|
| 1–10 | Control flow | `for`/`else`, `match`/`case`, walrus `:=`, list/dict/set comprehensions, generator expressions, `iter(callable, sentinel)`, `any`/`all`, `raise ... from` |
| 11–20 | Generators & closures | Infinite generators, `yield from`, `.send()` coroutines, the iterator protocol, `nonlocal`, currying, `functools.partial`, `lru_cache`, `itertools.cycle` / `combinations` |
| 21–30 | Decorators & metaprogramming | Timing decorators, decorator factories, class decorators, context managers (class and `@contextmanager`), `type()`, metaclasses, `__getattr__`, `@property` validation |
| 31–40 | OOP & dunders | `__slots__`, operator overloading, the sequence protocol, ABCs, MRO / `super()` in a diamond, `classmethod` vs `staticmethod`, frozen dataclasses, `__str__` vs `__repr__`, `__call__`, `weakref` parent links |
| 41–50 | Data structures & concurrency | `defaultdict`, `deque` sliding windows, `namedtuple`, `heapq`, `bisect`, `frozenset` keys, `asyncio.gather`, `ThreadPoolExecutor`, `bytearray`, zero-copy `memoryview` |

## What changed

The original upload kept the code inside `C README.md` and `Python README.md`, so it couldn't be compiled or run as is. The update:

- moves the code into real `.c` / `.py` source files;
- fixes a **syntax error** in Python #38 (`def __str__ me(self)`), which stopped the whole file from importing;
- fixes programs that did nothing or were wrong:
  - C #1 now guards against `a == b` aliasing, which zeroed the value.
  - C #47 now actually walks the tokens.
  - Python #25 no longer swallows *every* exception.
  - Python #40 actually uses `weakref`.
  - Python #42 now returns its result.
  - Python #18 now demonstrates a real memoisation win.
- replaces the `main` functions, which only printed "compiled successfully", with self-checks that exercise every program;
- adds CI, sanitizers and this README.

## Author

**P. Mohan Sai** – [GitHub](https://github.com/MohansaiPotnuru)
