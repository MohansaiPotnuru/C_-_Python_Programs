"""
50 Advanced Python Programs - each concept is a small function or class, and
`run_all()` exercises every one of them with an assertion, so running the file
doubles as a test suite.

    python advanced_python_programs.py        # requires Python 3.10+

Sections
    1. Control flow & comprehensions        (1-10)
    2. Generators, iterators & closures     (11-20)
    3. Decorators & metaprogramming         (21-30)
    4. Object-oriented & dunder mechanics   (31-40)
    5. Data structures & concurrency        (41-50)
"""
from __future__ import annotations

import asyncio
import bisect
import functools
import heapq
import itertools
import time
import weakref
from abc import ABC, abstractmethod
from collections import defaultdict, deque, namedtuple
from concurrent.futures import ThreadPoolExecutor
from contextlib import contextmanager
from dataclasses import FrozenInstanceError, dataclass
from typing import Any, Callable, Generator, Iterable

# ==============================================================================
# SECTION 1: CONTROL FLOW & COMPREHENSIONS (Programs 1-10)
# ==============================================================================


# Program 1: for-else - the else block runs only if the loop did NOT break.
def prog01_for_else(items: Iterable[int], target: int) -> str:
    for item in items:
        if item == target:
            result = "found"
            break
    else:
        result = "not found"
    return result


# Program 2: Structural pattern matching (Python 3.10+).
def prog02_pattern_matching(command: str) -> str:
    match command.split():
        case ["quit"]:
            return "Exiting"
        case ["move", ("left" | "right") as direction]:
            return f"Moving {direction}"
        case ["move", _]:
            return "Invalid direction"
        case _:
            return "Unknown command"


# Program 3: Walrus operator (:=) - assign and test in one expression.
def prog03_walrus_chunks(data: list[int], size: int) -> list[list[int]]:
    it = iter(data)
    chunks = []
    while chunk := list(itertools.islice(it, size)):
        chunks.append(chunk)
    return chunks


# Program 4: Nested comprehension - flatten a matrix keeping even numbers.
def prog04_flatten_evens(matrix: list[list[int]]) -> list[int]:
    return [val for row in matrix for val in row if val % 2 == 0]


# Program 5: Dict comprehension - invert a mapping, keeping positive values.
def prog05_dict_invert(d: dict[str, int]) -> dict[int, str]:
    return {v: k for k, v in d.items() if v > 0}


# Program 6: Set comprehension - unique vowels in a string.
def prog06_unique_vowels(text: str) -> set[str]:
    return {ch for ch in text.lower() if ch in "aeiou"}


# Program 7: Generator expression - O(1) memory instead of building a list.
def prog07_sum_of_squares(n: int) -> int:
    return sum(x * x for x in range(n))


# Program 8: Two-argument iter(callable, sentinel) - call until it returns the sentinel.
def prog08_sentinel_iter(func_callable: Callable[[], Any], sentinel: Any = "END") -> list:
    return list(iter(func_callable, sentinel))


# Program 9: any()/all() short-circuit - stop at the first decisive element.
def prog09_validate(numbers: list) -> bool:
    return all(isinstance(n, int) for n in numbers) and not any(n < 0 for n in numbers)


# Program 10: Exception chaining with `raise ... from` keeps the root cause.
def prog10_parse_int(text: str) -> int:
    try:
        return int(text)
    except ValueError as exc:
        raise RuntimeError(f"Parsing failed for {text!r}") from exc


# ==============================================================================
# SECTION 2: GENERATORS, ITERATORS & CLOSURES (Programs 11-20)
# ==============================================================================


# Program 11: Infinite Fibonacci generator.
def prog11_fibonacci() -> Generator[int, None, None]:
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b


# Program 12: Delegation to sub-generators with `yield from`.
def prog12_chain_generators(*gens: Iterable) -> Generator:
    for g in gens:
        yield from g


# Program 13: Two-way generator (coroutine) using .send().
def prog13_running_total() -> Generator[int, int | None, None]:
    total = 0
    while True:
        val = yield total
        if val is None:
            return
        total += val


# Program 14: Custom iterator class implementing __iter__ and __next__.
class Prog14Countdown:
    def __init__(self, start: int):
        self.current = start

    def __iter__(self):
        return self

    def __next__(self) -> int:
        if self.current <= 0:
            raise StopIteration
        self.current -= 1
        return self.current + 1


# Program 15: Stateful closure using `nonlocal`.
def prog15_make_counter() -> Callable[[], int]:
    count = 0

    def counter() -> int:
        nonlocal count
        count += 1
        return count

    return counter


# Program 16: Currying with a higher-order function.
def prog16_multiply(x: int) -> Callable[[int], int]:
    return lambda y: x * y


# Program 17: Partial application with functools.partial.
prog17_from_binary = functools.partial(int, base=2)


# Program 18: Memoisation with functools.lru_cache - exponential -> linear time.
@functools.lru_cache(maxsize=None)
def prog18_fib(n: int) -> int:
    return n if n < 2 else prog18_fib(n - 1) + prog18_fib(n - 2)


# Program 19: Round-robin scheduling with itertools.cycle + islice.
def prog19_round_robin(workers: list, limit: int) -> list:
    return list(itertools.islice(itertools.cycle(workers), limit))


# Program 20: Combinatorics with itertools.combinations.
def prog20_pairs(items: list) -> list[tuple]:
    return list(itertools.combinations(items, 2))


# ==============================================================================
# SECTION 3: DECORATORS & METAPROGRAMMING (Programs 21-30)
# ==============================================================================


# Program 21: Timing decorator (functools.wraps keeps the original name/docstring).
def prog21_time_it(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        t0 = time.perf_counter()
        result = func(*args, **kwargs)
        wrapper.last_elapsed = time.perf_counter() - t0
        return result

    wrapper.last_elapsed = 0.0
    return wrapper


# Program 22: Decorator factory (decorator that takes arguments).
def prog22_repeat(num_times: int):
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            return [func(*args, **kwargs) for _ in range(num_times)]

        return wrapper

    return decorator


# Program 23: Class-based decorator that counts calls.
class Prog23CallCounter:
    def __init__(self, func):
        functools.update_wrapper(self, func)
        self.func = func
        self.calls = 0

    def __call__(self, *args, **kwargs):
        self.calls += 1
        return self.func(*args, **kwargs)


# Program 24: Class decorator that injects an attribute.
def prog24_add_env(cls):
    cls.environment = "PROD"
    return cls


# Program 25: Context manager class with __enter__/__exit__.
class Prog25ManagedResource:
    def __init__(self):
        self.log: list[str] = []

    def __enter__(self):
        self.log.append("acquired")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.log.append("released")
        # Return True only to suppress *expected* errors; swallowing every
        # exception hides bugs.
        return exc_type is KeyError


# Program 26: Generator-based context manager with contextlib.contextmanager.
@contextmanager
def prog26_temp_setting(config: dict, key: str, value):
    old = config.get(key)
    config[key] = value
    try:
        yield config
    finally:  # always restore, even if the body raised
        config[key] = old


# Program 27: Create a class at run time with type(name, bases, namespace).
Prog27DynamicClass = type("Prog27DynamicClass", (object,), {"status": 200, "ok": lambda self: True})


# Program 28: Metaclass that enforces a class attribute.
class Prog28RequireName(type):
    def __new__(mcs, name, bases, namespace):
        if bases and "plugin_name" not in namespace:
            raise TypeError(f"{name} must define 'plugin_name'")
        return super().__new__(mcs, name, bases, namespace)


class Prog28PluginBase(metaclass=Prog28RequireName):
    pass


# Program 29: __getattr__ fallback (called only when normal lookup fails).
class Prog29Proxy:
    def __init__(self):
        self.real = "real value"

    def __getattr__(self, item):
        return f"dynamic:{item}"


# Program 30: Validated attribute with @property / setter.
class Prog30BoundedNumber:
    def __init__(self, val: int = 0):
        self.val = val  # goes through the setter

    @property
    def val(self) -> int:
        return self._val

    @val.setter
    def val(self, v: int) -> None:
        if v < 0:
            raise ValueError("Value must be non-negative")
        self._val = v


# ==============================================================================
# SECTION 4: OBJECT-ORIENTED & DUNDER MECHANICS (Programs 31-40)
# ==============================================================================


# Program 31: __slots__ - no per-instance __dict__, less memory, fixed attributes.
class Prog31Point:
    __slots__ = ("x", "y")

    def __init__(self, x: float, y: float):
        self.x, self.y = x, y


# Program 32: Operator overloading (__add__, __eq__, __repr__).
class Prog32Vector:
    def __init__(self, x: int, y: int):
        self.x, self.y = x, y

    def __add__(self, other: Prog32Vector) -> Prog32Vector:
        return Prog32Vector(self.x + other.x, self.y + other.y)

    def __eq__(self, other) -> bool:
        return isinstance(other, Prog32Vector) and (self.x, self.y) == (other.x, other.y)

    __hash__ = None  # mutable + __eq__ -> explicitly unhashable

    def __repr__(self) -> str:
        return f"Vector({self.x}, {self.y})"


# Program 33: Sequence protocol (__len__, __getitem__) - enables len(), indexing, iteration.
class Prog33CustomList:
    def __init__(self, data):
        self._data = list(data)

    def __len__(self) -> int:
        return len(self._data)

    def __getitem__(self, idx):
        return self._data[idx]


# Program 34: Abstract base class - cannot be instantiated until methods are implemented.
class Prog34BaseService(ABC):
    @abstractmethod
    def execute(self) -> str: ...


class Prog34EchoService(Prog34BaseService):
    def execute(self) -> str:
        return "executed"


# Program 35: Method Resolution Order in diamond inheritance (C3 linearisation).
class A:
    def who(self):
        return "A"


class B(A):
    def who(self):
        return "B>" + super().who()


class C(A):
    def who(self):
        return "C>" + super().who()


class D(B, C):  # MRO: D -> B -> C -> A
    pass


# Program 36: @classmethod (alternative constructor) vs @staticmethod (utility).
class Prog36Date:
    def __init__(self, year: int, month: int, day: int):
        self.year, self.month, self.day = year, month, day

    @classmethod
    def from_string(cls, date_str: str) -> Prog36Date:
        return cls(*map(int, date_str.split("-")))

    @staticmethod
    def is_valid_year(year: int) -> bool:
        return 1900 <= year <= 2100


# Program 37: Frozen dataclass - generated __init__/__repr__/__eq__, immutable.
@dataclass(frozen=True)
class Prog37UserConfig:
    db_host: str = "localhost"
    port: int = 5432


# Program 38: __str__ (for users) vs __repr__ (for developers).
class Prog38Person:
    def __init__(self, name: str):
        self.name = name

    def __str__(self) -> str:
        return f"Person: {self.name}"

    def __repr__(self) -> str:
        return f"Prog38Person(name={self.name!r})"


# Program 39: Callable instances via __call__.
class Prog39Multiplier:
    def __init__(self, factor: int):
        self.factor = factor

    def __call__(self, val: int) -> int:
        return val * self.factor


# Program 40: weakref - a child refers to its parent without keeping it alive,
# which breaks parent<->child reference cycles.
class Prog40Node:
    def __init__(self, val):
        self.val = val
        self._parent = None
        self.children: list[Prog40Node] = []

    @property
    def parent(self):
        return self._parent() if self._parent else None

    def add_child(self, child: Prog40Node) -> None:
        child._parent = weakref.ref(self)
        self.children.append(child)


# ==============================================================================
# SECTION 5: DATA STRUCTURES & CONCURRENCY (Programs 41-50)
# ==============================================================================


# Program 41: defaultdict - group without checking for missing keys.
def prog41_group_by_length(words: list[str]) -> dict[int, list[str]]:
    grouped: defaultdict[int, list[str]] = defaultdict(list)
    for word in words:
        grouped[len(word)].append(word)
    return dict(grouped)


# Program 42: deque(maxlen=n) - O(1) sliding window (moving average).
def prog42_moving_average(items: list[float], size: int) -> list[float]:
    window: deque[float] = deque(maxlen=size)
    out = []
    for item in items:
        window.append(item)
        if len(window) == size:
            out.append(sum(window) / size)
    return out


# Program 43: namedtuple - lightweight immutable record with named fields.
Prog43Point = namedtuple("Prog43Point", ["x", "y"])


# Program 44: heapq - top-k in O(n log k).
def prog44_top_k(nums: list[int], k: int) -> list[int]:
    return heapq.nlargest(k, nums)


# Program 45: bisect - keep a list sorted with O(log n) search.
def prog45_insert_sorted(arr: list[int], val: int) -> list[int]:
    bisect.insort(arr, val)
    return arr


# Program 46: frozenset - hashable set, usable as a dict key.
def prog46_group_lookup(members: Iterable[int]) -> str | None:
    groups = {frozenset({1, 2}): "Group A", frozenset({3, 4}): "Group B"}
    return groups.get(frozenset(members))


# Program 47: asyncio.gather - run coroutines concurrently.
async def prog47_task(task_id: int) -> str:
    await asyncio.sleep(0.01)
    return f"Task {task_id} done"


async def prog47_main(n: int = 3) -> list[str]:
    return await asyncio.gather(*(prog47_task(i) for i in range(n)))


# Program 48: ThreadPoolExecutor - parallel map for I/O-bound work.
def prog48_parallel_square(data: list[int]) -> list[int]:
    with ThreadPoolExecutor(max_workers=4) as executor:
        return list(executor.map(lambda x: x * x, data))


# Program 49: bytearray - mutable bytes, edited in place.
def prog49_capitalise_first(data: bytes) -> bytes:
    buf = bytearray(data)
    if buf and 0x61 <= buf[0] <= 0x7A:  # 'a'..'z'
        buf[0] -= 0x20
    return bytes(buf)


# Program 50: memoryview - zero-copy slicing; writes go through to the buffer.
def prog50_zero_copy_patch(data: bytes) -> bytes:
    buf = bytearray(data)
    view = memoryview(buf)[1:4]  # no copy made
    view[:] = b"XYZ"
    return bytes(buf)


# ==============================================================================
# DEMO / SELF-TEST
# ==============================================================================


def run_all() -> int:
    checks = 0

    def check(cond: bool, label: str) -> None:
        nonlocal checks
        if not cond:
            raise AssertionError(f"check failed: {label}")
        checks += 1

    # Section 1
    check(prog01_for_else([1, 2, 3], 2) == "found", "1")
    check(prog01_for_else([1, 2, 3], 9) == "not found", "1b")
    check(prog02_pattern_matching("move left") == "Moving left", "2")
    check(prog02_pattern_matching("dance") == "Unknown command", "2b")
    check(prog03_walrus_chunks([1, 2, 3, 4, 5], 2) == [[1, 2], [3, 4], [5]], "3")
    check(prog04_flatten_evens([[1, 2], [3, 4, 6]]) == [2, 4, 6], "4")
    check(prog05_dict_invert({"a": 1, "b": -1}) == {1: "a"}, "5")
    check(prog06_unique_vowels("Embedded Systems") == {"e"}, "6")
    check(prog07_sum_of_squares(4) == 14, "7")
    feed = iter(["a", "b", "END", "c"])
    check(prog08_sentinel_iter(lambda: next(feed)) == ["a", "b"], "8")
    check(prog09_validate([1, 2, 3]) and not prog09_validate([1, -2]), "9")
    try:
        prog10_parse_int("abc")
        check(False, "10")
    except RuntimeError as exc:
        check(isinstance(exc.__cause__, ValueError), "10")

    # Section 2
    check(list(itertools.islice(prog11_fibonacci(), 8)) == [0, 1, 1, 2, 3, 5, 8, 13], "11")
    check(list(prog12_chain_generators([1, 2], (3,), range(4, 6))) == [1, 2, 3, 4, 5], "12")
    acc = prog13_running_total()
    next(acc)
    acc.send(10)
    check(acc.send(5) == 15, "13")
    check(list(Prog14Countdown(3)) == [3, 2, 1], "14")
    counter = prog15_make_counter()
    counter()
    check(counter() == 2, "15")
    check(prog16_multiply(6)(7) == 42, "16")
    check(prog17_from_binary("1010") == 10, "17")
    check(prog18_fib(80) == 23416728348467685, "18")
    check(prog19_round_robin(["a", "b"], 5) == ["a", "b", "a", "b", "a"], "19")
    check(prog20_pairs([1, 2, 3]) == [(1, 2), (1, 3), (2, 3)], "20")

    # Section 3
    @prog21_time_it
    def slow_add(x, y):
        """adds"""
        return x + y

    check(slow_add(2, 3) == 5 and slow_add.__name__ == "slow_add" and slow_add.last_elapsed >= 0, "21")

    @prog22_repeat(3)
    def ping():
        return "pong"

    check(ping() == ["pong"] * 3, "22")

    @Prog23CallCounter
    def noop():
        return None

    noop(), noop()
    check(noop.calls == 2, "23")

    @prog24_add_env
    class Service:
        pass

    check(Service.environment == "PROD", "24")
    res = Prog25ManagedResource()
    with res:
        raise KeyError("suppressed on purpose")
    check(res.log == ["acquired", "released"], "25")
    cfg = {"debug": False}
    with prog26_temp_setting(cfg, "debug", True) as c:
        check(c["debug"] is True, "26")
    check(cfg["debug"] is False, "26b")
    check(Prog27DynamicClass().status == 200 and Prog27DynamicClass().ok(), "27")
    try:
        class BadPlugin(Prog28PluginBase):  # noqa: F841 - must fail
            pass
        check(False, "28")
    except TypeError:
        check(True, "28")

    class GoodPlugin(Prog28PluginBase):
        plugin_name = "good"

    check(GoodPlugin.plugin_name == "good", "28b")
    p = Prog29Proxy()
    check(p.real == "real value" and p.missing == "dynamic:missing", "29")
    num = Prog30BoundedNumber(5)
    try:
        num.val = -1
        check(False, "30")
    except ValueError:
        check(num.val == 5, "30")

    # Section 4
    pt = Prog31Point(1, 2)
    check(not hasattr(pt, "__dict__"), "31")
    check(Prog32Vector(1, 2) + Prog32Vector(3, 4) == Prog32Vector(4, 6), "32")
    cl = Prog33CustomList("abc")
    check(len(cl) == 3 and cl[-1] == "c" and list(cl) == ["a", "b", "c"], "33")
    try:
        Prog34BaseService()
        check(False, "34")
    except TypeError:
        check(Prog34EchoService().execute() == "executed", "34")
    check(D().who() == "B>C>A" and [k.__name__ for k in D.__mro__][:4] == ["D", "B", "C", "A"], "35")
    d = Prog36Date.from_string("2025-03-10")
    check(d.month == 3 and Prog36Date.is_valid_year(2025) and not Prog36Date.is_valid_year(1800), "36")
    conf = Prog37UserConfig()
    try:
        conf.port = 1  # type: ignore[misc]
        check(False, "37")
    except FrozenInstanceError:
        check(conf == Prog37UserConfig("localhost", 5432), "37")
    person = Prog38Person("Ada")
    check(str(person) == "Person: Ada" and repr(person) == "Prog38Person(name='Ada')", "38")
    check(Prog39Multiplier(3)(14) == 42, "39")
    root = Prog40Node("root")
    leaf = Prog40Node("leaf")
    root.add_child(leaf)
    check(leaf.parent is root, "40")
    del root
    check(leaf.parent is None, "40b")  # parent was freed; weakref did not keep it alive

    # Section 5
    check(prog41_group_by_length(["hi", "yo", "hey"]) == {2: ["hi", "yo"], 3: ["hey"]}, "41")
    check(prog42_moving_average([1, 2, 3, 4], 2) == [1.5, 2.5, 3.5], "42")
    q = Prog43Point(3, 4)
    check(q.x == 3 and q._replace(y=9) == (3, 9), "43")
    check(prog44_top_k([5, 1, 9, 3, 7], 2) == [9, 7], "44")
    check(prog45_insert_sorted([1, 3, 5], 4) == [1, 3, 4, 5], "45")
    check(prog46_group_lookup([2, 1]) == "Group A" and prog46_group_lookup([9]) is None, "46")
    check(asyncio.run(prog47_main()) == ["Task 0 done", "Task 1 done", "Task 2 done"], "47")
    check(prog48_parallel_square([1, 2, 3]) == [1, 4, 9], "48")
    check(prog49_capitalise_first(b"python") == b"Python", "49")
    check(prog50_zero_copy_patch(b"abcdef") == b"aXYZef", "50")

    return checks


if __name__ == "__main__":
    n = run_all()
    print(f"All 50 programs ran - {n} checks passed.")
