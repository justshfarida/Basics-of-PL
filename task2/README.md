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
| Platform | Google Colab |
| Python | 3.13.15 (main, Aug 6 2026) [GCC 13.3.0] |
| OS | Linux-6.6.122+-x86_64-with-glibc2.39 |
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
