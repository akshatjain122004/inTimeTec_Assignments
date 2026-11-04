# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# define MINIMUM_POOL_SIZE 1
# define MAXIMUM_POOL_SIZE 1000000
# define INITIAL_BLOCK_CAPACITY 2

struct MemoryBlock
{
  int allocatedID;
  bool isAllocated;
  int size;
  int startOffset;
};

bool validateMemoryPoolSize(int size);

void displayMenu();

void displaySelectedStrategy(int choice);

int readIntegerFromUser(const char *prompt, int *value);

bool isStrategyChoiceValid(int choice);

bool ensureBlockCapacityForOneMoreBlock(struct MemoryBlock **blocks, int *count, int *capacity);

bool insertBlockAt(struct MemoryBlock **blocks, int *count, int *capacity, int index, struct MemoryBlock newBlock);

void removeBlockAt(struct MemoryBlock *blocks, int *count, int index);

void fillRegionWithAllocationPattern(char *pool, int startOffset, int length, int allocationID);

void allocateMemory(int strategy, int poolSize, char *pool, struct MemoryBlock **blocks, int *count, int *capacity, int *nextID);

int findSuitableBlock(int strategy, int size, struct MemoryBlock *blocks, int count);

void releaseMemory(struct MemoryBlock *blocks, int *count, int nextID);

int isBlockExists(int id, struct MemoryBlock *blocks, int count);

void mergeFreeBlocks(struct MemoryBlock *blocks, int *count);

void displayMemoryMap(struct MemoryBlock *blocks, int count, int poolSize);

void displayMemoryStatistics(struct MemoryBlock *blocks, int count, int poolSize);

void reportExternalFragmentationIfPresent(struct MemoryBlock *blocks, int count, int requestedSize);

void changeAllocationStrategy(int *strategyChoice);

bool shrinkAllocationInPlace(struct MemoryBlock **blocks, int *count, int *capacity, int block, int newSize);

bool tryExpandAllocationInPlace(struct MemoryBlock **blocks, int *count, char *pool, int block, int newSize);

bool moveAllocationToNewRegion(int strategy, struct MemoryBlock **blocks, int *count, int *capacity, char *pool, int block, int newSize);

void resizeAllocation(int strategy, struct MemoryBlock **blocks, int *count, int *capacity, char *pool, int poolSize);

int main()
{
  int memoryPoolSize;
  int menuChoice;
  int allocationStrategyChoice;

  if (readIntegerFromUser("Enter the size of the memory pool(1 - 1000000): ", &memoryPoolSize) != 1)
  {
    printf("Invalid input. Exiting the program.\n");
    return 0;
  }

  if (!validateMemoryPoolSize(memoryPoolSize))
  {
    return 0;
  }

  char *memoryPool = (char *)malloc(memoryPoolSize * sizeof(char));

  if (memoryPool == NULL)
  {
    printf("Memory allocation failed. Exiting the program.\n");
    return 0;
  }

  if (readIntegerFromUser("Select Allocation Strategy: (1. First Fit 2. Best Fit 3. Worst Fit)\n", &allocationStrategyChoice) != 1 ||
      !isStrategyChoiceValid(allocationStrategyChoice))
  {
    printf("Invalid choice. Please select a valid allocation strategy.\n");
    free(memoryPool);
    return 0;
  }

  displaySelectedStrategy(allocationStrategyChoice);

  struct MemoryBlock *memoryBlocks = (struct MemoryBlock *)malloc(INITIAL_BLOCK_CAPACITY * sizeof(struct MemoryBlock));

  if (memoryBlocks == NULL)
  {
    printf("Memory allocation failed. Exiting the program.\n");
    free(memoryPool);
    return 0;
  }

  (memoryBlocks + 0)->allocatedID = 0;
  (memoryBlocks + 0)->isAllocated = false;
  (memoryBlocks + 0)->size = memoryPoolSize;
  (memoryBlocks + 0)->startOffset = 0;

  int blockCapacity = INITIAL_BLOCK_CAPACITY;
  int blockCount = 1;
  int nextAllocatedID = 1;

  do
  {
    printf("\n\n");

    displayMenu();

    int inputStatus = readIntegerFromUser("Enter your choice: ", &menuChoice);

    if (inputStatus == -1)
    {
      break;
    }

    if (inputStatus == 0)
    {
      printf("Invalid input. Please enter a number from 1 to 7.\n");
      menuChoice = 0;
      continue;
    }

    switch (menuChoice)
    {
      case 1:
        // Allocate Memory
        allocateMemory(allocationStrategyChoice, memoryPoolSize, memoryPool, &memoryBlocks, &blockCount, &blockCapacity, &nextAllocatedID);
        break;

      case 2:
        // Release Memory
        releaseMemory(memoryBlocks, &blockCount, nextAllocatedID);
        break;

      case 3:
        // Resize Allocation
        resizeAllocation(allocationStrategyChoice, &memoryBlocks, &blockCount, &blockCapacity, memoryPool, memoryPoolSize);
        break;

      case 4:
        // Display Memory Map
        displayMemoryMap(memoryBlocks, blockCount, memoryPoolSize);
        break;

      case 5:
        // Display Memory statistics
        displayMemoryStatistics(memoryBlocks, blockCount, memoryPoolSize);
        break;

      case 6:
        // Change Memory Allocation Strategy
        changeAllocationStrategy(&allocationStrategyChoice);
        break;

      case 7:
        printf("Exiting the program.\n");
        break;

      default:
        printf("Invalid choice. Please try again.\n");
    }
  }
  while (menuChoice != 7);

  free(memoryPool);
  free(memoryBlocks);

  return 0;
}

bool validateMemoryPoolSize(int size)
{
  if (size < MINIMUM_POOL_SIZE || size > MAXIMUM_POOL_SIZE)
  {
    printf("Invalid memory pool size. Please enter a value between 1 and 1000000.\n");
    return false;
  }
  return true;
}

void displayMenu()
{
  printf("1. Allocate Memory\n");
  printf("2. Release Memory\n");
  printf("3. Resize Allocation\n");
  printf("4. Display Memory Map\n");
  printf("5. Display Memory statistics\n");
  printf("6. Change Memory Allocation Strategy\n");
  printf("7. Exit\n");
}

void displaySelectedStrategy(int choice)
{
  switch (choice)
  {
    case 1:
      printf("Selected Allocation Strategy: First Fit\n");
      break;
    case 2:
      printf("Selected Allocation Strategy: Best Fit\n");
      break;
    case 3:
      printf("Selected Allocation Strategy: Worst Fit\n");
      break;
  }
}

// Returns 1 on success, 0 on non-numeric input (bad line is discarded), -1 when input ends.
int readIntegerFromUser(const char *prompt, int *value)
{
  printf("%s", prompt);

  int scanResult = scanf("%d", value);

  if (scanResult == EOF)
  {
    return -1;
  }

  if (scanResult != 1)
  {
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {}
    return 0;
  }

  return 1;
}

bool isStrategyChoiceValid(int choice)
{
  return choice >= 1 && choice <= 3;
}

// Grows the block array with realloc only when it is full; the old array stays valid if realloc fails.
bool ensureBlockCapacityForOneMoreBlock(struct MemoryBlock **blocks, int *count, int *capacity)
{
  if (*count < *capacity)
  {
    return true;
  }

  int newCapacity = *capacity * 2;
  struct MemoryBlock *temp = (struct MemoryBlock *)realloc(*blocks, (size_t)newCapacity * sizeof(struct MemoryBlock));

  if (temp == NULL)
  {
    return false;
  }

  *blocks = temp;
  *capacity = newCapacity;
  return true;
}

// Inserts a block at index and shifts later blocks right so the array stays ordered by offset.
bool insertBlockAt(struct MemoryBlock **blocks, int *count, int *capacity, int index, struct MemoryBlock newBlock)
{
  if (!ensureBlockCapacityForOneMoreBlock(blocks, count, capacity))
  {
    return false;
  }

  for (int i = *count; i > index; i--)
  {
    (*blocks)[i] = (*blocks)[i - 1];
  }

  (*blocks)[index] = newBlock;
  (*count)++;
  return true;
}

void removeBlockAt(struct MemoryBlock *blocks, int *count, int index)
{
  for (int i = index; i < *count - 1; i++)
  {
    blocks[i] = blocks[i + 1];
  }

  (*count)--;
}

// Stamps the region with the owner's ID so moved/resized data can be recognised.
void fillRegionWithAllocationPattern(char *pool, int startOffset, int length, int allocationID)
{
  for (int i = 0; i < length; i++)
  {
    *(pool + startOffset + i) = (char)allocationID;
  }
}

void allocateMemory(int strategy, int poolSize, char *pool, struct MemoryBlock **blocks, int *count, int *capacity, int *nextID)
{
  int size;

  if (readIntegerFromUser("Enter the size of memory to allocate: ", &size) != 1)
  {
    printf("Invalid input.\n");
    return;
  }

  if (size <= 0 || size > poolSize)
  {
    printf("Invalid size.\n");
    return;
  }

  int block = findSuitableBlock(strategy, size, *blocks, *count);

  if (block == -1)
  {
    printf("No suitable block found for allocation.\n");
    reportExternalFragmentationIfPresent(*blocks, *count, size);
    return;
  }

  int originalSize = (*blocks)[block].size;
  int originalStartOffset = (*blocks)[block].startOffset;

  if (originalSize > size)
  {
    struct MemoryBlock remainingFreeBlock;
    remainingFreeBlock.allocatedID = 0;
    remainingFreeBlock.isAllocated = false;
    remainingFreeBlock.size = originalSize - size;
    remainingFreeBlock.startOffset = originalStartOffset + size;

    // Done before touching the chosen block, so a failure leaves the map unchanged.
    if (!insertBlockAt(blocks, count, capacity, block + 1, remainingFreeBlock))
    {
      printf("Memory reallocation failed.Previous records retained.\n");
      return;
    }
  }

  (*blocks)[block].isAllocated = true;
  (*blocks)[block].allocatedID = *nextID;
  (*nextID)++;
  (*blocks)[block].size = size;

  fillRegionWithAllocationPattern(pool, originalStartOffset, size, (*blocks)[block].allocatedID);

  printf("Allocation successful. ID: %d, Start: %d, Size: %d\n", (*blocks)[block].allocatedID, (*blocks)[block].startOffset, (*blocks)[block].size);
}

int findSuitableBlock(int strategy, int size, struct MemoryBlock *blocks, int count)
{
  switch(strategy)
  {
    case 1: // first fit
    {
      for (int i = 0; i < count; i++)
      {
        if (!blocks[i].isAllocated && blocks[i].size >= size)
        {
          return i;
        }
      }
      return -1;
    }

    case 2: // best fit
    {
      int bestIndex = -1;
      int bestSize = 0;

      for (int i = 0; i < count; i++)
      {
        if (!blocks[i].isAllocated && blocks[i].size >= size)
        {
          if (bestIndex == -1 || blocks[i].size < bestSize)
          {
            bestIndex = i;
            bestSize = blocks[i].size;
          }
        }
      }
      return bestIndex;
    }

    case 3: // worst fit
    {
      int worstIndex = -1;
      int worstSize = 0;

      for (int i = 0; i < count; i++)
      {
        if (!blocks[i].isAllocated && blocks[i].size >= size)
        {
          if (worstIndex == -1 || blocks[i].size > worstSize)
          {
            worstIndex = i;
            worstSize = blocks[i].size;
          }
        }
      }
      return worstIndex;
    }
  }

  return -1;
}

void releaseMemory(struct MemoryBlock *blocks, int *count, int nextID)
{
  int id;

  if (readIntegerFromUser("Enter the ID of the memory block to release: ", &id) != 1)
  {
    printf("Invalid input.\n");
    return;
  }

  if (id <= 0 || id >= nextID)
  {
    printf("Unknown allocation ID %d.\n", id);
    return;
  }

  int block = isBlockExists(id, blocks, *count);
  if (block == -1)
  {
    printf("Block with ID %d is already free.\n", id);
    return;
  }

  blocks[block].isAllocated = false;
  blocks[block].allocatedID = 0;

  mergeFreeBlocks(blocks, count);
  printf("Memory block with ID %d released successfully.\n", id);
}

int isBlockExists(int id, struct MemoryBlock *blocks, int count)
{
  for (int i = 0; i < count; i++)
  {
    if (blocks[i].allocatedID == id && blocks[i].isAllocated)
    {
      return i;
    }
  }

  return -1;
}

void mergeFreeBlocks(struct MemoryBlock *blocks, int *count)
{
  int i = 0;

  while (i < *count - 1)
  {
    if (!blocks[i].isAllocated && !blocks[i + 1].isAllocated)
    {
      blocks[i].size += blocks[i + 1].size;
      removeBlockAt(blocks, count, i + 1);
    }
    else
    {
      i++;
    }
  }
}

void displayMemoryMap(struct MemoryBlock *blocks, int count, int poolSize)
{
  printf("Memory Map(%d bytes)\n", poolSize);

  for (int i = 0; i < count; i++)
  {
    if (blocks[i].isAllocated)
    {
      printf("Allocated Id %d: %d bytes, Start: %d\n", blocks[i].allocatedID, blocks[i].size, blocks[i].startOffset);
    }
    else
    {
      printf("Free: %d bytes, Start: %d\n", blocks[i].size, blocks[i].startOffset);
    }
  }
}

void displayMemoryStatistics(struct MemoryBlock *blocks, int count, int poolSize)
{
  printf("Total Memory: %d bytes\n", poolSize);
  int allocatedMemory = 0;
  int freeMemory = 0;
  int allocatedBlocks = 0;
  int freeBlocks = 0;
  int largestFreeBlock = 0;

  for (int i = 0; i < count; i++)
  {
    if (blocks[i].isAllocated)
    {
      allocatedMemory += blocks[i].size;
      allocatedBlocks++;
    }
    else
    {
      freeMemory += blocks[i].size;
      freeBlocks++;

      if (blocks[i].size > largestFreeBlock)
      {
        largestFreeBlock = blocks[i].size;
      }
    }
  }

  printf("Allocated Memory: %d bytes\n", allocatedMemory);
  printf("Free Memory: %d bytes\n", freeMemory);
  printf("Number of Allocated Blocks: %d\n", allocatedBlocks);
  printf("Number of Free Blocks: %d\n", freeBlocks);
  printf("Largest Free Block: %d bytes\n", largestFreeBlock);
}

// Explains a failure caused by external fragmentation using the sizes of the individual free blocks.
void reportExternalFragmentationIfPresent(struct MemoryBlock *blocks, int count, int requestedSize)
{
  int totalFree = 0;

  for (int i = 0; i < count; i++)
  {
    if (!blocks[i].isAllocated)
    {
      totalFree += blocks[i].size;
    }
  }

  if (totalFree < requestedSize)
  {
    return;
  }

  printf("External fragmentation: the request for %d bytes failed even though %d bytes are free,\n", requestedSize, totalFree);
  printf("because the free memory is split into separate blocks of sizes:");

  for (int i = 0; i < count; i++)
  {
    if (!blocks[i].isAllocated)
    {
      printf(" %d", blocks[i].size);
    }
  }

  printf(" bytes.\n");
}

void changeAllocationStrategy(int *strategyChoice)
{
  int newChoice;

  if (readIntegerFromUser("Select Allocation Strategy: (1. First Fit 2. Best Fit 3. Worst Fit)\n", &newChoice) != 1 ||
      !isStrategyChoiceValid(newChoice))
  {
    printf("Invalid choice. Strategy unchanged.\n");
    return;
  }

  *strategyChoice = newChoice;
  displaySelectedStrategy(*strategyChoice);
}

// Gives the tail of an allocation back: it joins the next free block, or becomes a new free block.
bool shrinkAllocationInPlace(struct MemoryBlock **blocks, int *count, int *capacity, int block, int newSize)
{
  int oldStart = (*blocks)[block].startOffset;
  int releasedSize = (*blocks)[block].size - newSize;

  if (block + 1 < *count && !(*blocks)[block + 1].isAllocated)
  {
    (*blocks)[block + 1].startOffset = oldStart + newSize;
    (*blocks)[block + 1].size += releasedSize;
  }
  else
  {
    struct MemoryBlock releasedTailBlock;
    releasedTailBlock.allocatedID = 0;
    releasedTailBlock.isAllocated = false;
    releasedTailBlock.size = releasedSize;
    releasedTailBlock.startOffset = oldStart + newSize;

    if (!insertBlockAt(blocks, count, capacity, block + 1, releasedTailBlock))
    {
      printf("Resize failed. Original allocation unchanged.\n");
      return false;
    }
  }

  (*blocks)[block].size = newSize;
  return true;
}

// Grows into the free block that directly follows, if that block has enough room.
bool tryExpandAllocationInPlace(struct MemoryBlock **blocks, int *count, char *pool, int block, int newSize)
{
  int oldSize = (*blocks)[block].size;
  int extraNeeded = newSize - oldSize;
  int oldEnd = (*blocks)[block].startOffset + oldSize;

  if (block + 1 >= *count ||
      (*blocks)[block + 1].isAllocated ||
      (*blocks)[block + 1].size < extraNeeded)
  {
    return false;
  }

  if ((*blocks)[block + 1].size == extraNeeded)
  {
    removeBlockAt(*blocks, count, block + 1);
  }
  else
  {
    (*blocks)[block + 1].startOffset += extraNeeded;
    (*blocks)[block + 1].size -= extraNeeded;
  }

  (*blocks)[block].size = newSize;
  fillRegionWithAllocationPattern(pool, oldEnd, extraNeeded, (*blocks)[block].allocatedID);
  return true;
}

// Moves to another free region chosen by the active strategy; the old region is freed only after the copy.
bool moveAllocationToNewRegion(int strategy, struct MemoryBlock **blocks, int *count, int *capacity, char *pool, int block, int newSize)
{
  int id = (*blocks)[block].allocatedID;
  int oldSize = (*blocks)[block].size;
  int oldStart = (*blocks)[block].startOffset;

  int destination = findSuitableBlock(strategy, newSize, *blocks, *count);

  if (destination == -1)
  {
    printf("Resize failed: no suitable free region.\n");
    printf("Original allocation and data remain unchanged.\n");
    reportExternalFragmentationIfPresent(*blocks, *count, newSize);
    return false;
  }

  int destinationSize = (*blocks)[destination].size;
  int destinationStart = (*blocks)[destination].startOffset;

  if (destinationSize > newSize)
  {
    struct MemoryBlock remainingFreeBlock;
    remainingFreeBlock.allocatedID = 0;
    remainingFreeBlock.isAllocated = false;
    remainingFreeBlock.size = destinationSize - newSize;
    remainingFreeBlock.startOffset = destinationStart + newSize;

    if (!insertBlockAt(blocks, count, capacity, destination + 1, remainingFreeBlock))
    {
      printf("Resize failed. Original allocation unchanged.\n");
      return false;
    }

    // The insert shifted every block after the destination one step right.
    if (destination < block)
    {
      block++;
    }
  }

  for (int i = 0; i < oldSize; i++)
  {
    *(pool + destinationStart + i) = *(pool + oldStart + i);
  }

  fillRegionWithAllocationPattern(pool, destinationStart + oldSize, newSize - oldSize, id);

  (*blocks)[block].isAllocated = false;
  (*blocks)[block].allocatedID = 0;

  (*blocks)[destination].isAllocated = true;
  (*blocks)[destination].allocatedID = id;
  (*blocks)[destination].size = newSize;

  mergeFreeBlocks(*blocks, count);

  printf("Allocation %d moved from offset %d to %d, size %d bytes.\n", id, oldStart, destinationStart, newSize);
  return true;
}

void resizeAllocation(int strategy, struct MemoryBlock **blocks, int *count, int *capacity, char *pool, int poolSize)
{
  int id, newSize;

  if (readIntegerFromUser("Enter the ID of the memory block to resize: ", &id) != 1)
  {
    printf("Invalid input.\n");
    return;
  }

  int block = isBlockExists(id, *blocks, *count);

  if (block == -1)
  {
    printf("No allocated block found with ID %d.\n", id);
    return;
  }

  if (readIntegerFromUser("Enter the new size: ", &newSize) != 1)
  {
    printf("Invalid input.\n");
    return;
  }

  if (newSize <= 0 || newSize > poolSize)
  {
    printf("Invalid size. Enter between 1 and %d bytes.\n", poolSize);
    return;
  }

  int oldSize = (*blocks)[block].size;

  if (newSize == oldSize)
  {
    printf("Allocation already has this size.\n");
    return;
  }

  if (newSize < oldSize)
  {
    if (shrinkAllocationInPlace(blocks, count, capacity, block, newSize))
    {
      mergeFreeBlocks(*blocks, count);
      printf("Allocation %d shrunk to %d bytes.\n", id, newSize);
    }
    return;
  }

  if (tryExpandAllocationInPlace(blocks, count, pool, block, newSize))
  {
    printf("Allocation %d expanded in place to %d bytes.\n", id, newSize);
    return;
  }

  moveAllocationToNewRegion(strategy, blocks, count, capacity, pool, block, newSize);
}