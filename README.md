# Puddle

Puddle is a highly efficient pool allocator
designed for allocating single objects.

## Overview

Puddle uses a simple pool allocation method.

Objects are allocated in large blocks of memory.
A list of free nodes is stored interleaved with the allocated objects
in these blocks of memory.

When there are no more free nodes, a new block of memory is allocated.
These blocks of memory will remain allocated until the program terminates
(although you could easily force all blocks to deallocate).

Using this technique,
the most recently freed node will be the one that is next allocated.

### Example Allocation

An example allocation pattern with a block size of 4 objects:

    Blocks: 1
    Next free: 0:0
    Block 0:
        Object 0: (free, next=0:1)
        Object 1: (free, next=0:2)
        Object 2: (free, next=0:3)
        Object 3: (free, next=null)

We now call `allocate()` 4 times:

    Blocks: 1
    Next free: null
    Block 0:
        Object 0: (allocated)
        Object 1: (allocated)
        Object 2: (allocated)
        Object 3: (allocated)

Now we'll `deallocate()` objects `0:1` and then `0:3`:

    Blocks: 1
    Next free: 0:3
    Block 0:
        Object 0: (allocated)
        Object 1: (free, next=null)
        Object 2: (allocated)
        Object 3: (free, next=0:1)

Three more `allocate()` calls:

    Blocks: 2
    Next free: 1:1
    Block 0:
        Object 0: (allocated)
        Object 1: (allocated)
        Object 2: (allocated)
        Object 3: (allocated)
    Block 1:
        Object 0: (allocated)
        Object 1: (free, next=1:2)
        Object 2: (free, next=1:3)
        Object 3: (free, next=null)

## Usage

`Puddle::Allocator<T>` meets all of the requirements for C++ allocators,
with one exception; it cannot allocate arrays.
Only single objects may be allocated with `allocate(1)`.

A `std::runtime_error` will be thrown if `allocate()` is called
with a size other than `1`.
An equivalent restriction is palced on `deallocate()`.

Because of this restriction,
`Puddle::Allocator<T>` will not work for array-based containers,
such as `std::vector` and `std::deque`.

However, it works with most other containers,
and provides a significant performance boost.

For example:

```C++
struct Widget {/* ... */};

std::list<Widget, Puddle::Allocator<Widget>> widgets;
```

A generic alias:

```C++
template <typename T>
using fast_list = std::list<T, Puddle::Allocator<T>>;

fast_list<Widget> widgets;
```
