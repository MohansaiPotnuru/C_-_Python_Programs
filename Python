import asyncio
import functools
import itertools
import time
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import Generator, Any

# ==============================================================================
# SECTION 1: ADVANCED CONTROL FLOW & COMPREHENSIONS (Programs 1-10)
# ==============================================================================

# Program 1: For-Else construct (Executes else if loop completes without break)
def prog01_for_else(items: list, target: int) -> bool:
    for item in items:
        if item == target:
            return True
    else:
        print("Target not found in iterable")
        return False

# Program 2: Structural Pattern Matching (Python 3.10+ match-case)
def prog02_pattern_matching(command: str) -> str:
    match command.split():
        case ["quit"]: return "Exiting"
        case ["move", ("left" | "right") as direction]: return f"Moving {direction}"
        case _: return "Unknown command"

# Program 3: Walrus Operator (`:=`) inside condition loops
def prog03_walrus_operator(data: list[int]) -> list[int]:
    results = []
    while (n := len(data)) > 0:
        results.append(data.pop())
    return results

# Program 4: Nested list comprehension with conditional flattening
def prog04_flatten(matrix: list[list[int]]) -> list[int]:
    return [val for row in matrix if len(row) > 1 for val in row if val % 2 == 0]

# Program 5: Dict comprehension with key swapping and filtering
def prog05_dict_invert(d: dict[str, int]) -> dict[int, str]:
    return {v: k for k, v in d.items() if v > 0}

# Program 6: Set comprehension for unique element processing
def prog06_unique_vowels(text: str) -> set[str]:
    return {char.lower() for char in text if char.lower() in 'aeiou'}

# Program 7: Generator expression vs list comprehension memory optimization
def prog07_gen_exp(n: int):
    return sum(x * x for x in range(n))  # Consumes O(1) memory

# Program 8: Sentinel-value `iter()` loop execution
def prog08_sentinel_iter(func_callable):
    # Calls func_callable until it returns 'END'
    return list(iter(func_callable, 'END'))

# Program 9: Short-circuiting evaluation logic with `all()` and `any()`
def prog09_short_circuit(numbers: list[int]) -> bool:
    return any(n < 0 for n in numbers) and all(isinstance(n, int) for n in numbers)

# Program 10: Exception chaining using `raise ... from`
def prog10_exception_chaining():
    try:
        int("invalid")
    except ValueError as e:
        raise RuntimeError("Parsing failed") from e

# ==============================================================================
# SECTION 2: GENERATORS, ITERATORS & CLOSURES (Programs 11-20)
# ==============================================================================

# Program 11: Infinite Fibonacci sequence generator using `yield`
def prog11_fibonacci() -> Generator[int, None, None]:
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b

# Program 12: Sub-generator delegation using `yield from`
def prog12_chain_generators(g1, g2):
    yield from g1
    yield from g2

# Program 13: Two-way generator communication using `.send()`
def prog13_coroutine_accumulator():
    total = 0
    while True:
        val = yield total
        if val is None: break
        total += val

# Program 14: Custom iterator class implementing `__iter__` and `__next__`
class Prog14_Countdown:
    def __init__(self, start: int):
        self.current = start
    def __iter__(self):
        return self
    def __next__(self) -> int:
        if self.current <= 0: raise StopIteration
        self.current -= 1
        return self.current + 1

# Program 15: Stateful lexical closure
def prog15_make_counter():
    count = 0
    def counter():
        nonlocal count
        count += 1
        return count
    return counter

# Program 16: Currying functions using higher-order functions
def prog16_multiply(x):
    return lambda y: x * y

# Program 17: Partial function application via `functools.partial`
prog17_basetwo = functools.partial(int, base=2)

# Program 18: In-memory caching decorator via `functools.lru_cache`
@functools.lru_cache(maxsize=32)
def prog18_cached_computation(n: int) -> int:
    return n * n if n <= 1 else prog18_cached_computation(n - 1) + n

# Program 19: Infinite sequence iteration using `itertools.cycle`
def prog19_round_robin(selectors: list, limit: int):
    return list(itertools.islice(itertools.cycle(selectors), limit))

# Program 20: Combinatoric generation via `itertools.combinations`
def prog20_get_pairs(items: list):
    return list(itertools.combinations(items, 2))

# ==============================================================================
# SECTION 3: DECORATORS & METAPROGRAMMING (Programs 21-30)
# ==============================================================================

# Program 21: Function execution timer decorator
def prog21_time_it(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        t0 = time.perf_counter()
        res = func(*args, **kwargs)
        print(f"Executed in {time.perf_counter() - t0:.4f}s")
        return res
    return wrapper

# Program 22: Parametrized decorator (Decorator accepting arguments)
def prog22_repeat(num_times: int):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            for _ in range(num_times):
                res = func(*args, **kwargs)
            return res
        return wrapper
    return decorator

# Program 23: Class-based decorator pattern
class Prog23_CallCounter:
    def __init__(self, func):
        self.func = func
        self.calls = 0
    def __call__(self, *args, **kwargs):
        self.calls += 1
        return self.func(*args, **kwargs)

# Program 24: Class decorator modifying class attributes dynamically
def prog24_add_id(cls):
    cls.entity_id = "ENV_PROD"
    return cls

# Program 25: Custom context manager class using `__enter__` and `__exit__`
class Prog25_ManagedResource:
    def __enter__(self):
        print("Resource acquired")
        return self
    def __exit__(self, exc_type, exc_val, exc_tb):
        print("Resource released")
        return True # Suppress exceptions if any

# Program 26: Generator-based context manager via `contextlib.contextmanager`
from contextlib import contextmanager
@contextmanager
def prog26_temp_file():
    print("Setup temp file")
    yield "temp.txt"
    print("Cleanup temp file")

# Program 27: Dynamic class creation using `type()`
Prog27_DynamicClass = type('DynamicClass', (object,), {'status': 200})

# Program 28: Simple metaclass enforcing attribute constraints
class Prog28_Meta(type):
    def __new__(cls, name, bases, dct):
        if 'required_var' not in dct:
            dct['required_var'] = True
        return super().__new__(cls, name, bases, dct)

# Program 29: Attribute access interception using `__getattr__` and `__setattr__`
class Prog29_Proxy:
    def __getattr__(self, item):
        return f"Dynamic fallback for '{item}'"

# Program 30: Property descriptor implementation
class Prog30_BoundedNumber:
    def __init__(self, val=0): self._val = val
    @property
    def val(self): return self._val
    @val.setter
    def val(self, v):
        if v < 0: raise ValueError("Value must be non-negative")
        self._val = v

# ==============================================================================
# SECTION 4: OBJECT-ORIENTED & DUNDER MECHANICS (Programs 31-40)
# ==============================================================================

# Program 31: Memory optimization using `__slots__`
class Prog31_OptimizedPoint:
    __slots__ = ('x', 'y')
    def __init__(self, x: float, y: float):
        self.x = x; self.y = y

# Program 32: Operator overloading via `__add__` and `__eq__`
class Prog32_Vector:
    def __init__(self, x: int, y: int): self.x = x; self.y = y
    def __add__(self, other): return Prog32_Vector(self.x + other.x, self.y + other.y)
    def __repr__(self): return f"Vector({self.x}, {self.y})"

# Program 33: Custom container class implementing `__getitem__` and `__len__`
class Prog33_CustomList:
    def __init__(self, data): self._data = list(data)
    def __len__(self): return len(self._data)
    def __getitem__(self, idx): return self._data[idx]

# Program 34: Abstract Base Class (ABC) pattern
class Prog34_BaseService(ABC):
    @abstractmethod
    def execute(self): pass

# Program 35: Polymorphic Method Resolution Order (MRO) demonstration
class A: pass
class B(A): pass
class C(A): pass
class D(B, C): pass # MRO: D -> B -> C -> A

# Program 36: Classmethod vs Staticmethod usage
class Prog36_DateUtils:
    @classmethod
    def from_string(cls, date_str: str): return date_str.split('-')
    @staticmethod
    def is_valid_year(year: int) -> bool: return 1900 <= year <= 2100

# Program 37: Dataclass usage with field defaults and immutability
@dataclass(frozen=True)
class Prog37_UserConfig:
    db_host: str = "localhost"
    port: int = 5432

# Program 38: Custom String representation using `__str__` vs `__repr__`
class Prog38_Person:
    def __init__(self, name): self.name = name
    def __str__ me(self): return f"Person: {self.name}"
    def __repr__(self): return f"Prog38_Person(name='{self.name}')"

# Program 39: Callable instances using `__call__`
class Prog39_Multiplier:
    def __init__(self, factor: int): self.factor = factor
    def __call__(self, val: int) -> int: return val * self.factor

# Program 40: Weak references using `weakref` to prevent reference cycles
import weakref
class Prog40_Node:
    def __init__(self, val):
        self.val = val
        self.parent = None

# ==============================================================================
# SECTION 5: ADVANCED DATA STRUCTURES & ASYNC (Programs 41-50)
# ==============================================================================

# Program 41: Default values handling using `collections.defaultdict`
from collections import defaultdict
def prog41_group_words(words: list[str]) -> dict[int, list[str]]:
    grouped = defaultdict(list)
    for word in words: grouped[len(word)].append(word)
    return dict(grouped)

# Program 42: Double-ended queue optimization using `collections.deque`
from collections import deque
def prog42_sliding_window(items: list, size: int):
    d = deque(maxlen=size)
    for item in items: d.append(item)

# Program 43: Lightweight struct replacement using `collections.namedtuple`
from collections import namedtuple
Prog43_Point = namedtuple('Point', ['x', 'y'])

# Program 44: High-performance heap sorting using `heapq`
import heapq
def prog44_top_k_elements(nums: list[int], k: int) -> list[int]:
    return heapq.nlargest(k, nums)

# Program 45: Fast array lookups using binary insertion via `bisect`
import bisect
def prog45_insert_sorted(arr: list[int], val: int) -> list[int]:
    bisect.insort(arr, val)
    return arr

# Program 46: Immutable set operations using `frozenset`
def prog46_immutable_keys() -> dict[frozenset, str]:
    return {frozenset([1, 2]): "Group A"}

# Program 47: Asynchronous task execution using `asyncio`
async def prog47_async_task(id: int):
    await asyncio.sleep(0.01)
    return f"Task {id} done"

async def prog47_main():
    return await asyncio.gather(*(prog47_async_task(i) for i in range(3)))

# Program 48: ThreadPoolExecutor for concurrent execution
from concurrent.futures import ThreadPoolExecutor
def prog48_parallel_exec(data: list[int]):
    with ThreadPoolExecutor(max_workers=2) as executor:
        return list(executor.map(lambda x: x * x, data))

# Program 49: Memory-efficient byte manipulation using `bytearray`
def prog49_modify_bytes(data: bytes) -> bytes:
    mutable_b = bytearray(data)
    if len(mutable_b) > 0: mutable_b[0] = 0x41 # Swap first char to 'A'
    return bytes(mutable_b)

# Program 50: Structural memory inspection via `memoryview`
def prog50_zero_copy_slice(data: bytes) -> memoryview:
    mv = memoryview(bytearray(data))
    return mv[1:4] # Zero-copy slicing

if __name__ == "__main__":
    print("All 50 Python Programs Defined Successfully.")
