# RESOURCES

This document summarizes the key concepts, knowledge, and skills required to implement a custom memory allocator like `ft_malloc` from scratch. No external links are provided, only the topics and ideas you should master.

## 1. C Programming Fundamentals
- Pointers, pointer arithmetic, and memory addressing
- Structs and unions
- Function pointers
- Header files and modular code organization
- Compilation, linking, and shared libraries (`.so`)
- Error handling and debugging (e.g., `gdb`, `valgrind`)

## 2. Operating System Concepts
- Virtual memory and address spaces
- Memory pages and page size
- Heap vs. stack memory
- System calls for memory management (`mmap`, `munmap`, `brk`)
- File descriptors and permissions

## 3. Memory Management Algorithms
- Free lists and block splitting/merging (coalescing)
- Fragmentation (internal and external)
- Alignment and padding
- Zone/heap organization (TINY, SMALL, LARGE)
- Allocation strategies (first-fit, best-fit, etc.)

## 4. Data Structures
- Linked lists (single and double)
- Circular lists
- Custom metadata for blocks and heaps
- Efficient search and insertion/removal

## 5. Thread Safety
- Race conditions and data consistency
- Mutexes (`pthread_mutex_t`)
- Atomic operations (basics)

## 6. Dynamic Linking and Preloading
- Environment variables (`LD_PRELOAD`, `LD_LIBRARY_PATH`)
- Overriding standard library functions
- Building and using shared objects

## 7. Testing and Debugging
- Writing and running unit tests
- Stress testing and edge cases
- Using custom test suites
- Memory leak detection
- Logging and debug output

## 8. 42 Norminette Coding Style
- Function and file length limits
- Indentation and formatting rules
- Variable declaration and assignment rules
- Header and function documentation

## 9. Performance Considerations
- Minimizing system calls
- Reducing fragmentation
- Efficient block splitting/merging
- Cache locality

## 10. Project Management
- Makefiles and build automation
- Version control basics (e.g., git)
- Documentation and code comments

---

Mastering these areas will enable you to design, implement, debug, and maintain a robust custom memory allocator in C, suitable for use as a drop-in replacement for the standard `malloc`/`free`/`realloc`/`calloc` functions.