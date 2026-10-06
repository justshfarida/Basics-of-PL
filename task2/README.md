# Memory Size of Python Tuples vs. Lists

## 1. Objective

This report investigates why two Python containers holding the same three values occupy different amounts of memory:

```python
tpl = (1, 2, 3)
lst = [1, 2, 3]
```

The goal is to measure their sizes with `__sizeof__()`, explain the difference using the internal memory layout of CPython, and discuss what the difference means in practice.

## 2. Environment

| Item | Value |
|---|---|
| Platform | OnlineGDB (online Python compiler) |
| Python | 3.13.7 (main, Aug 14 2025) [GCC 14.3.0] |
| OS | Linux-6.8.0-1069-gcp-x86_64-with-glibc2.40 |
| Architecture | 64-bit (pointer size = 8 bytes) |

The exact numbers in this report depend on the Python implementation (CPython), its version, and the architecture. On a 32-bit build, or on older CPython versions, the values will differ.

## 3. Experiment

### 3.1 Basic measurement

```python
tpl = (1, 2, 3)
lst = [1, 2, 3]

print("Tuple:", tpl.__sizeof__(), "bytes")
print("List:", lst.__sizeof__(), "bytes")
```

Output:

```
Tuple: 48 bytes
List: 72 bytes
```

### 3.2 Additional measurements

```python
import sys

print("Empty tuple:", ().__sizeof__())
print("Empty list:", [].__sizeof__())
print("getsizeof tuple:", sys.getsizeof((1, 2, 3)))
print("getsizeof list:", sys.getsizeof([1, 2, 3]))
print("Size of int 1:", sys.getsizeof(1))
```

Output:

```
Empty tuple: 24
Empty list: 40
getsizeof tuple: 64
getsizeof list: 88
Size of int 1: 28
```

## 4. Analysis

### 4.1 Memory layout of a tuple

A tuple is immutable. Its length is fixed at creation, so CPython stores the element pointers directly inside the tuple object, in one contiguous block:

| Field | Purpose | Size (bytes) |
|---|---|---|
| `ob_refcnt` | Reference count(Py_ssize_t type is 8 bytes) | 8 |
| `ob_type` | Pointer to the type object | 8 |
| `ob_size` | Number of elements | 8 |
| `ob_item[3]` | Three pointers to the elements | 24 |
| **Total** | | **48** |

This matches the measurement: the empty tuple is 24 bytes (header only), and each element adds exactly one 8-byte pointer.

### 4.1 Memory layout of a List

A list is immutable, so it can grow and shrink. This is the list object fields:

| Field | Purpose | Size (bytes) |
|---|---|---|
| `ob_refcnt` | Reference count | 8 |
| `ob_type` | Pointer to the type object | 8 |
| `ob_size` | Number of elements in use | 8 |
| `ob_item` | Pointer to the separate pointer array | 8 |
| `allocated` | Capacity of the pointer array | 8 |
| **Header subtotal** | | **40** |

List's elements cannot live inside the object itself, because it should be resizable.
So, list's ob_item field holds a pointer to the array of pointers(array of values' memory addresses).
The empty list measures 40 bytes, which confirms the header size. 
The list header has two fields a tuple does not need: the `ob_item` pointer (because the array is stored elsewhere) and `allocated` (because capacity and length can differ).

The size of the 3 element list is 72, 72 minus 40 makes 32. Where does this 32 come from? We have only 3 elements.And each integer takes up 8 bytes.
Normally, it should be 8x3=24 bytes, not 32.
So i tried to investigate it.

I ran a code to find the capacity of the tuple:
```python
def capacity(lst):
    return (lst.__sizeof__() - 40) // 8

a = [1, 2, 3]
print(len(a), capacity(a))  
```

```
3 4 
```
Capacity is 4.

### 4.3 Why 4 slots instead of 3
 
The list holds 3 elements but has room for 4. Recent CPython versions (3.12 and later) round the initial capacity up to an even number when a list is built from a known-size sequence. So `[1, 2, 3]` reserves 4 slots, and 72 = 40 + 4 × 8. On older versions such as 3.11, the same list would typically report 64 bytes (40 + 3 × 8).

### 4.4 `__sizeof__()` vs. `sys.getsizeof()`

Section 3.2 shows that the two functions give different results for the same objects:

| Object | `__sizeof__()` | `sys.getsizeof()` | Difference |
|---|---|---|---|
| `(1, 2, 3)` | 48 | 64 | 16 |
| `[1, 2, 3]` | 72 | 88 | 16 |
| `()` | 24 | 40 | 16 |
| `[]` | 40 | 56 | 16 |
| `1` | 28 | 28 | 0 |
| `3.5` | 24 | 24 | 0 |
| `"abc"` | 44 | 44 | 0 |

**What each function measures.**

- `__sizeof__()` is a method that every object has. It returns the size of the object's own structure: exactly the fields described in sections 4.1 and 4.2, plus the separate pointer array for a list.
- `sys.getsizeof()` calls `__sizeof__()` and then adds the extra memory that CPython keeps in front of the object for the garbage collector. According to the Python documentation, it "calls the object's `__sizeof__` method and adds an additional garbage collector overhead if the object is managed by the garbage collector."

**Why only tuples and lists get the extra 16 bytes.** CPython frees most objects with reference counting: when `ob_refcnt` drops to 0, the object is deleted. This fails for objects that refer to each other in a cycle, for example a list that contains itself:

```python
a = []
a.append(a)   # a refers to itself, so ob_refcnt never reaches 0
del a         # the list can no longer be reached, but is not freed
```

To clean up such cycles, CPython has a separate cycle garbage collector. Only **containers** can be part of a cycle, because only they hold references to other objects. CPython therefore stores a small GC header directly in front of every container object. On a 64-bit build of CPython 3.13 this header holds two 8-byte pointers that link the object into the collector's list of tracked objects, so it takes **16 bytes**.

Integers, floats and strings cannot reference other objects, so they have no GC header, and both functions return the same value for them.

The extra 16 bytes depend on the **type**, not on whether the collector currently tracks this particular object. The empty tuple `()` is never tracked, because it cannot contain anything, but `sys.getsizeof(())` still adds 16 bytes because every tuple is allocated with room for the header.

**What neither function measures.** Both functions are **shallow**: they report the memory of the container itself, not of the objects it points to. `sys.getsizeof([1, 2, 3])` counts the three 8-byte pointers in the list's array, but not the three 28-byte integer objects they point to. To measure everything a container holds, its elements have to be added up recursively, for example with the `tracemalloc` module or a third-party tool such as `pympler`.

**Which one to use.** `sys.getsizeof()` is closer to the real memory cost of an object, because the GC header is allocated together with it. `__sizeof__()` matches the C structure of the object field by field, which is why the tables in this report use it.

## 5. Discussion and Critique
 
**Tuples are cheaper, but the saving is constant.** For three elements, the list costs 24 bytes more (50% more). However, this overhead is mostly a fixed header plus spare capacity, so the relative difference shrinks as containers grow. For a single small container it rarely matters. For millions of small records (for example, rows of data or coordinates), choosing tuples can save significant memory.

**The difference reflects a design trade-off, not inefficiency.** The list pays for mutability: an extra indirection to reach elements, a capacity field, and unused slots. In return it supports fast appends, insertions, and in-place modification. A tuple gives up all of that in exchange for compactness. Tuples are also hashable when their elements are, so they can be used as dictionary keys and set members, which lists cannot.

## References

- Python documentation, `sys.getsizeof`: https://docs.python.org/3/library/sys.html#sys.getsizeof
- Python documentation, supporting cyclic garbage collection: https://docs.python.org/3/c-api/gcsupport.html
- Python documentation, `gc` module: https://docs.python.org/3/library/gc.html
- CPython source, `sys.getsizeof` implementation (`_PySys_GetSizeOf`): https://github.com/python/cpython/blob/3.13/Python/sysmodule.c
- CPython source, GC header (`PyGC_Head`): https://github.com/python/cpython/blob/3.13/Include/internal/pycore_gc.h
- CPython source, object header (`ob_refcnt`, `ob_type`, `ob_size`): https://github.com/python/cpython/blob/3.13/Include/object.h
- CPython source, tuple structure: https://github.com/python/cpython/blob/3.13/Include/cpython/tupleobject.h
- CPython source, list structure: https://github.com/python/cpython/blob/3.13/Include/cpython/listobject.h
