# Smart Memory Manager

A simple memory manager written in C. It simulates allocating, releasing and resizing regions of a fixed-size memory pool, and compares three allocation strategies: First Fit, Best Fit and Worst Fit.

## Features

- Creates a memory pool of 1 to 1,000,000 bytes with `malloc()`.
- Allocates blocks using First Fit, Best Fit or Worst Fit. You can switch strategy at any time from the menu.
- Splits a free block when an allocation is smaller than the block.
- Releases an allocation by ID and merges neighbouring free blocks.
- Resizes an allocation:
  - Shrinking gives the tail back to the free space.
  - Growing first tries to expand into the free block right after it.
  - If that is not possible, it moves the data to another free region chosen by the active strategy, then frees the old region.
  - If a resize fails, the original allocation and its data stay unchanged.
- Shows the memory map and statistics (allocated, free, block counts, largest free block).
- Detects external fragmentation. When a request fails even though enough total memory is free, it lists the sizes of the individual free blocks.

## Build and Run

```
gcc -Wall -Wextra -o mm memory_manager.c
./mm
```

To check for memory errors during development:

```
gcc -Wall -Wextra -g -fsanitize=address,undefined -o mm memory_manager.c
```

## Menu

```
1. Allocate Memory
2. Release Memory
3. Resize Allocation
4. Display Memory Map
5. Display Memory statistics
6. Change Memory Allocation Strategy
7. Exit
```

## Example Session (First Fit, 1000-byte pool)

```
Enter the size of the memory pool(1 - 1000000): 1000
Select Allocation Strategy: (1. First Fit 2. Best Fit 3. Worst Fit)
1
Selected Allocation Strategy: First Fit

Enter your choice: 1
Enter the size of memory to allocate: 200
Allocation successful. ID: 1, Start: 0, Size: 200

Enter your choice: 1
Enter the size of memory to allocate: 300
Allocation successful. ID: 2, Start: 200, Size: 300

Enter your choice: 2
Enter the ID of the memory block to release: 1
Memory block with ID 1 released successfully.

Enter your choice: 1
Enter the size of memory to allocate: 250
Allocation successful. ID: 3, Start: 500, Size: 250

Enter your choice: 4
Memory Map(1000 bytes)
Free: 200 bytes, Start: 0
Allocated Id 2: 300 bytes, Start: 200
Allocated Id 3: 250 bytes, Start: 500
Free: 250 bytes, Start: 750

Enter your choice: 1
Enter the size of memory to allocate: 300
No suitable block found for allocation.
External fragmentation: the request for 300 bytes failed even though 450 bytes are free,
because the free memory is split into separate blocks of sizes: 200 250 bytes.
```

## How It Works

### Data structures

- **Memory pool:** one `char` buffer allocated with `malloc()`.
- **Block metadata:** a separate dynamic array of `struct MemoryBlock`. It starts with room for 2 blocks and doubles with `realloc()` when full. There is no linked list and no fixed-size array.

```c
struct MemoryBlock
{
  int allocatedID;    // 0 for a free block, positive for an allocated block
  bool isAllocated;
  int size;
  int startOffset;
};
```

### Rules the program keeps after every operation

- The metadata array is ordered by `startOffset`.
- Blocks cover the whole pool with no gaps and no overlaps, so the sizes always add up to the pool size.
- Two free blocks are never next to each other (they are merged).
- Allocation IDs are positive and never reused.
- Sizes and IDs are validated before the memory map is changed. Any operation that needs to add a block does that first, so if `realloc()` fails the map is left as it was.

### Functions

| Function | Why it exists |
|---|---|
| `validateMemoryPoolSize` | Rejects pool sizes outside 1 to 1,000,000. |
| `readIntegerFromUser` | Reads a number safely and discards bad input so the menu cannot loop forever. |
| `isStrategyChoiceValid` | Accepts only strategy choices 1, 2 or 3. |
| `ensureBlockCapacityForOneMoreBlock` | Grows the metadata array with `realloc()` only when it is full. |
| `insertBlockAt` | Inserts a block and shifts later ones right, keeping address order. |
| `removeBlockAt` | Removes a block and shifts later ones left. |
| `fillRegionWithAllocationPattern` | Marks bytes with the owner's ID so moved data can be recognised. |
| `allocateMemory` | Reads a size, picks a block with the active strategy, and splits it if needed. |
| `findSuitableBlock` | Implements First Fit, Best Fit and Worst Fit. Returns -1 if nothing fits. |
| `releaseMemory` | Frees an allocation by ID, rejecting unknown and already-free IDs. |
| `isBlockExists` | Finds the allocated block with a given ID. |
| `mergeFreeBlocks` | Merges every run of neighbouring free blocks into one. |
| `shrinkAllocationInPlace` | Returns the tail of an allocation to the free space. |
| `tryExpandAllocationInPlace` | Grows into the free block right after the allocation, if it is big enough. |
| `moveAllocationToNewRegion` | Reserves a new region, copies the data, then frees the old region. |
| `resizeAllocation` | Reads the request and chooses between shrinking, expanding in place and moving. |
| `displayMemoryMap` | Prints blocks in address order with offset, size and status. |
| `displayMemoryStatistics` | Prints totals, block counts and the largest free block. |
| `reportExternalFragmentationIfPresent` | Explains a failed request using the sizes of the individual free blocks. |
| `changeAllocationStrategy` | Switches the strategy used by later allocations and resizes. |

### Allocation strategies

| Strategy | Chooses |
|---|---|
| First Fit | The first free block that is large enough. |
| Best Fit | The smallest free block that is large enough. |
| Worst Fit | The largest free block that is large enough. |

Example: with free blocks of 100, 500 and 200 bytes and a request for 150 bytes, First Fit takes the 500-byte block, Best Fit takes the 200-byte block, and Worst Fit takes the 500-byte block.

## Limits and Notes

- Pool size: 1 to 1,000,000 bytes. Each request must be greater than 0 and no larger than the pool size.
- The pool holds simulated data only. Allocated regions are filled with a pattern based on their ID, just to show that data survives a move.
- Resizing to the current size does nothing.
- A resize that has to move the data needs one contiguous free block of the full new size.

## Files

- `memory_manager.c`: the full program.
- `README.md`: this file.
