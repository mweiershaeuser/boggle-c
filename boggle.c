#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CODE 84

void handleInput(int, char **, char **, char **, long *);
void validateInput(char *, long);
void generateGridArray(char *, long, char (*)[*]);
void outputGrid(char *, long, char[*][*]);

int main(int argc, char **argv)
{
    char *grid, *word = "", *result;
    long size = 4;

    handleInput(argc, argv, &grid, &word, &size);
    validateInput(grid, size);

    char gridArray[4][4];
    generateGridArray(grid, size, gridArray);

    outputGrid(grid, size, gridArray);

    /* Handle not given word
        if (word[0] == '\0')
        {
            printf("Es ist null!");
        }
    */

    return EXIT_SUCCESS;
}

void handleInput(int argc, char **argv, char **grid, char **word, long *size)
{
    if (argc > 1)
    {
        if (argc % 2 != 0)
        {
            for (int i = 1; i < argc; i = i + 2)
            {

                if (strcmp(argv[i], "-g") == 0)
                {
                    *grid = argv[i + 1];
                }
                else if (strcmp(argv[i], "-s") == 0)
                {
                    *size = strtol(argv[i + 1], NULL, 10);
                }
                else if (strcmp(argv[i], "-w") == 0)
                {
                    *word = argv[i + 1];
                }
                else
                {
                    fprintf(stderr, "Syntax Error: Unknown flag. Please adhere to the format './boggle -g GRID [-s SIZE] [-w WORD]'\n");
                    exit(ERROR_CODE);
                }
            }
        }
        else
        {
            fprintf(stderr, "Syntax Error: Please adhere to the format './boggle -g GRID [-s SIZE] [-w WORD]'\n");
            exit(ERROR_CODE);
        }
    }
    else
    {
        fprintf(stderr, "Syntax Error: No grid provided. Please provide a grid using -g flag.\n");
        exit(ERROR_CODE);
    }
}

void validateInput(char *grid, long size)
{
    if (strlen(grid) != (size * size))
    {
        fprintf(stderr, "Error: The amount of characters in the grid '%s' does not match the provided grid size of %ld.\n", grid, size);
        exit(ERROR_CODE);
    }
}

void generateGridArray(char *grid, long size, char (*gridArray)[size])
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            gridArray[i][j] = grid[i * size + j];
        }
    }
}

void outputGrid(char *grid, long size, char gridArray[size][size])
{
    // character + space to the right + 3 characters space for pipe characters left and rigth and space left + newline
    int rowSize = (size * 2 + 4);
    // characters per row for each row + a padding row at the top and bottom
    int resultSize = rowSize * (size + 2);

    char result[resultSize];

    for (int i = 0; i < rowSize; i++)
    {
        strcat(result, "+");
    }

    strcat(result, "\n");

    for (int i = 0; i < size; i++)
    {
        strcat(result, "| ");

        for (int j = 0; j < size; j++)
        {
            char character[2] = {' ', ' '};
            character[0] = gridArray[i][j];
            strcat(result, character);
        }

        strcat(result, "|\n");
    }

    for (int i = 0; i < rowSize; i++)
    {
        strcat(result, "+");
    }
    strcat(result, "\n");

    printf("%s", result);
}