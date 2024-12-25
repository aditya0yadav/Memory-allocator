# Custom Memory Allocator

A thread-safe implementation of a basic memory allocator in C, providing custom `malloc()` functionality.

## Features

- Thread-safe memory allocation using mutex locks
- Free block reuse
- Linked list memory block management
- Memory alignment support

## Implementation Details

The allocator uses a linked list of memory blocks, each with a header containing:
- Block size
- Free/occupied status
- Pointer to next block

### Key Components

```c
struct head_t {
    size_t size;
    unsigned is_free;
    struct head_t *next;
};
```

### Memory Management Strategy

1. **Block Reuse**: Searches for existing free blocks before requesting new memory
2. **Memory Extension**: Uses `sbrk()` to extend heap when needed
3. **Thread Safety**: Implements mutex locks for concurrent access

## Usage

```c
void *ptr1 = malloc(100);  // Allocate 100 bytes
void *ptr2 = malloc(50);   // Allocate 50 bytes
```

## Building

Compile with pthread support:

```bash
gcc -pthread allocator.c -o allocator
```

## Limitations

- No memory coalescing
- No splitting of larger blocks
- Simple first-fit allocation strategy

## License

MIT License

## Contributing

Pull requests welcome. Please ensure thread safety is maintained in any modifications.
