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

