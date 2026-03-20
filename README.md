# Memory Allocator in C

A simple memory allocator that replaces `malloc()` and `free()` with custom implementations built from scratch using `mmap()`.

## How It Works

The allocator requests a single 4096-byte block of memory from the OS using `mmap()`, then manages that memory internally using a linked list of block headers.

Each block in the arena has a **header** (24 bytes) followed by **usable memory**:

```
[header][usable bytes][header][usable bytes][header][usable bytes]...
```

The header is a struct that tracks:
- `size` — how many usable bytes follow this header
- `free` — whether the block is available (1) or in use (0)
- `next` — pointer to the next header in the linked list

## Functions

| Function | Description |
|---|---|
| `heap_init()` | Calls `mmap()` once to get a 4096-byte arena. Sets up the first header at the start. |
| `my_malloc(size)` | Walks the linked list to find the first free block big enough (first-fit). Splits the block if there's enough leftover space for a new header + usable bytes. Returns a pointer to the memory after the header. |
| `my_free(ptr)` | Steps back from the user pointer to find the header. Marks it as free. Coalesces (merges) with the next block if it's also free. |
| `heap_dump()` | Debugging tool — prints each block's size and free/used status. |

## Example Output

```
=== after heap_init ===
size is 4072 bytes
free is 1

=== after malloc(100) and malloc(200) ===
size is 100 bytes
free is 0
size is 200 bytes
free is 0
size is 3724 bytes
free is 1

=== after freeing first block (100) ===
size is 100 bytes
free is 1
size is 200 bytes
free is 0
size is 3724 bytes
free is 1

=== after freeing second block (200) — should coalesce ===
size is 100 bytes
free is 1
size is 3948 bytes
free is 1
```

## Building and Running

Requires Linux (uses `mmap` and `sys/mman.h`). On Windows, compile through WSL:

```bash
gcc *.c -o main && ./main
```

## Files

- `allocator.h` — struct definition and function declarations
- `allocator.c` — all allocator logic (heap_init, my_malloc, my_free, heap_dump)
- `main.c` — test code and concept reference diagrams

## Key Concepts Learned

- How `malloc()` and `free()` work under the hood
- `mmap()` syscall for requesting memory from the OS
- Linked list-based free list management
- First-fit allocation strategy
- Block splitting (dividing large free blocks)
- Coalescing (merging adjacent free blocks to prevent fragmentation)
- Pointer arithmetic and casting (`char *` for byte-level precision)
- Embedding metadata (headers) inside the memory arena itself
