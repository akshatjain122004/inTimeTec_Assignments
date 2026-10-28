#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printMatrix(int **cellPointers, int matrixSize)
{
    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++)
        {
            printf("%4d", **(cellPointers + rowIndex * matrixSize + columnIndex));
        }
        printf("\n");
    }
}

void rotateMatrixClockwise(int **cellPointers, int matrixSize)
{
    for (int layerIndex = 0; layerIndex < matrixSize / 2; layerIndex++)
    {
        int firstIndex = layerIndex;
        int lastIndex = matrixSize - 1 - layerIndex;

        for (int offset = 0; offset < lastIndex - firstIndex; offset++)
        {
            int **topCell = cellPointers + firstIndex * matrixSize + (firstIndex + offset);
            int **rightCell = cellPointers + (firstIndex + offset) * matrixSize + lastIndex;
            int **bottomCell = cellPointers + lastIndex * matrixSize + (lastIndex - offset);
            int **leftCell = cellPointers + (lastIndex - offset) * matrixSize + firstIndex;

            int *temporaryPointer = *topCell;
            *topCell = *leftCell;
            *leftCell = *bottomCell;
            *bottomCell = *rightCell;
            *rightCell = temporaryPointer;
        }
    }
}

void applySmoothingFilter(int **cellPointers, int matrixSize)
{
    int *previousRowValues = (int *)malloc(matrixSize * sizeof(int));
    int *currentRowValues = (int *)malloc(matrixSize * sizeof(int));

    for (int rowIndex = 0; rowIndex < matrixSize; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++)
        {
            *(currentRowValues + columnIndex) = **(cellPointers + rowIndex * matrixSize + columnIndex);
        }

        for (int columnIndex = 0; columnIndex < matrixSize; columnIndex++)
        {
            int neighborSum = 0;
            int neighborCount = 0;

            for (int rowOffset = -1; rowOffset <= 1; rowOffset++)
            {
                int neighborRow = rowIndex + rowOffset;
                if (neighborRow < 0 || neighborRow >= matrixSize)
                {
                    continue;
                }

                for (int columnOffset = -1; columnOffset <= 1; columnOffset++)
                {
                    int neighborColumn = columnIndex + columnOffset;
                    if (neighborColumn < 0 || neighborColumn >= matrixSize)
                    {
                        continue;
                    }

                    int neighborValue;
                    if (rowOffset == -1)
                    {
                        neighborValue = *(previousRowValues + neighborColumn);
                    }
                    else if (rowOffset == 0)
                    {
                        neighborValue = *(currentRowValues + neighborColumn);
                    }
                    else
                    {
                        neighborValue = **(cellPointers + neighborRow * matrixSize + neighborColumn);
                    }

                    neighborSum += neighborValue;
                    neighborCount++;
                }
            }
            **(cellPointers + rowIndex * matrixSize + columnIndex) = neighborSum / neighborCount;
        }

        int *temporaryRow = previousRowValues;
        previousRowValues = currentRowValues;
        currentRowValues = temporaryRow;
    }

    free(previousRowValues);
    free(currentRowValues);
}

int main(void)
{
    int matrixSize;
    printf("Enter matrix size (2-10): ");
    if (scanf("%d", &matrixSize) != 1 || matrixSize < 2 || matrixSize > 10)
    {
        printf("Invalid size.\n");
        return 1;
    }

    int *matrixValues = (int *)malloc(matrixSize * matrixSize * sizeof(int));
    int **cellPointers = (int **)malloc(matrixSize * matrixSize * sizeof(int *));

    srand((unsigned)time(NULL));
    for (int index = 0; index < matrixSize * matrixSize; index++)
    {
        *(matrixValues + index) = rand() % 256;
        *(cellPointers + index) = matrixValues + index;
    }

    printf("Original Randomly Generated Matrix:\n");
    printMatrix(cellPointers, matrixSize);

    rotateMatrixClockwise(cellPointers, matrixSize);
    printf("\nMatrix after 90° Clockwise Rotation:\n");
    printMatrix(cellPointers, matrixSize);

    applySmoothingFilter(cellPointers, matrixSize);
    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    printMatrix(cellPointers, matrixSize);

    free(cellPointers);
    free(matrixValues);
    return 0;
}