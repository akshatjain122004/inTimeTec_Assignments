# Sonar Image Matrix Processor

A C program that generates a random N x N sonar intensity matrix, rotates it 90° clockwise, and applies a 3x3 smoothing filter. All operations are in-place and use pointer arithmetic only (no array indexing).

## Features

- Generates a random N x N matrix (2 <= N <= 10) with values from 0 to 255
- Rotates the matrix 90° clockwise by swapping pointers (addresses), not values
- Applies a 3x3 average filter, using only valid neighbors for edge and corner cells
- No extra N x N matrix is used; extra space is O(N)

## How It Works

1. **Generation:** values are stored in one block, with a table of pointers to each cell.
2. **Rotation:** pointers are swapped layer by layer, four cells at a time.
3. **Smoothing:** two O(N) helper rows hold the original values of the previous and current rows, so updated cells never affect their neighbors.

## Build and Run

```
gcc sonar_matrix.c -o sonar_matrix
./sonar_matrix
```

## Example

```
Enter matrix size (2-10): 3

Original Randomly Generated Matrix:
  12  60  90
 200 150  30
  10 220 180

Matrix after 90° Clockwise Rotation:
  10 200  12
 220 150  60
 180  30  90

Matrix after Applying 3x3 Smoothing Filter:
 145 108 105
 131 105  90
 145 121  82
```

## Constraints

- 2 <= N <= 10
- Pointer-based access only, no array indexing
- No extra N x N matrix
- Written in C
