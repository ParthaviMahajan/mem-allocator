# Custom Memory Allocator in C

A custom malloc/free-style memory allocator built in C to understand low-level heap management, memory fragmentation, alignment, and allocator internals.

This project implements a free-list based allocator using explicit heap expansion via `sbrk()` and includes features such as block splitting, coalescing, alignment guarantees, heap integrity checks, and corruption detection.


## Features

- Custom `malloc()` / `free()` style allocator
- Free-list based heap management
- First-fit allocation strategy
- Block splitting
- Block coalescing
- 8-byte memory alignment
- Explicit heap growth using `sbrk()`
- Heap layout visualization tools
- Heap integrity checker
- Cycle detection using Floyd’s Algorithm
- Double-free detection
- Fragmentation visualization and diagnostics


## Project Structure

```text
mem-allocator/
│
├── src/
│   └── allocator.c
│
├── include/
│   └── allocator.h
│
├── tests/
│   └── test_allocator.c
│
├── Makefile
├── README.md
└── .gitignore
```


## Allocator Architecture

Each allocation is organized as:

```text
[ METADATA ][ USER MEMORY ]
```

The allocator stores metadata before every allocated block:

```c
typedef struct block{
    size_t size;
    int free;
    struct block *next;
} block_t;
```

The linked list tracks heap blocks and enables:
- traversal
- allocation
- splitting
- coalescing
- debugging


## Memory Alignment

The allocator guarantees 8-byte aligned allocations using:

```c
(size + 7) & ~7
```

This ensures:
- proper CPU alignment
- safe pointer access
- reduced undefined behavior risk


## Build and Run

### Compile

```bash
make
```

### Run

```bash
make run
```


## Sample Heap Layout

```text
=========== HEAP LAYOUT ===========

BLOCK 0
Block Address      : 0x...
Aligned Payload    : 104 bytes
State              : ALLOCATED

BLOCK 1
Block Address      : 0x...
Aligned Payload    : 200 bytes
State              : FREE
```


## Current Limitations

- Single-threaded only
- Uses singly linked free list
- No `realloc()` support yet
- No `calloc()` support yet
- No boundary tags
- No segregated free lists
- Heap memory is not returned to the OS
- No `mmap()` support


## Learning Outcomes

This project helped deepen understanding of:

- Heap memory management
- Pointer arithmetic
- Fragmentation handling
- Memory alignment
- Heap traversal
- Low-level systems programming
- Allocator debugging techniques
- Heap corruption detection


## Future Improvements

Planned future enhancements include:

- `realloc()` implementation
- `calloc()` implementation
- Explicit free lists
- Boundary tags
- Best-fit allocation strategy
- Segregated free lists
- Thread safety
- `mmap()` support for large allocations