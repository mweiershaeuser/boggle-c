#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ERROR_CODE 84

typedef struct Occurrence
{
    int characterIndex;
    int row;
    int column;
} OCCURRENCE;

void handleInput(int, char **, char **, char **, long *);
void validateInput(char *, long);
void generateGridArray(char *, long, char (*)[*]);
int validateWord(char *, long, char[*][*], OCCURRENCE *);
int findNeigbouringOccurence(char *, long, char[*][*], OCCURRENCE *, OCCURRENCE);
void highlightOccurrences(long, char (*)[*], int, OCCURRENCE *);
void outputGrid(char *, long, char[*][*]);

int main(int argc, char **argv)
{
    char *grid, *word = "", *result;
    long size = 4;

    handleInput(argc, argv, &grid, &word, &size);
    validateInput(grid, size);

    char gridArray[4][4];
    generateGridArray(grid, size, gridArray);

    int exitLoop = 0;

    if (word[0] != '\0')
    {
        exitLoop = 1;
    }

    do
    {
        if (exitLoop == 0)
        {
            outputGrid(grid, size, gridArray);
            char wordInput[20];
            scanf("%19s", wordInput);
            word = wordInput;
        }

        OCCURRENCE finalOccurrences[strlen(word)];
        int wordFound = validateWord(word, size, gridArray, finalOccurrences);

        if (wordFound == 0)
        {
            printf("The word '%s' is not in the grid.\n", word);
        }
        else
        {
            highlightOccurrences(size, gridArray, strlen(word), finalOccurrences);
            outputGrid(grid, size, gridArray);
            exitLoop = 1;
        }
    } while (exitLoop == 0);

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

int validateWord(char *word, long size, char gridArray[size][size], OCCURRENCE *finalOccurrences)
{

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (gridArray[i][j] == word[0])
            {
                OCCURRENCE occurrence;
                occurrence.characterIndex = 0;
                occurrence.row = i;
                occurrence.column = j;

                int wordFound = findNeigbouringOccurence(word, size, gridArray, finalOccurrences, occurrence);

                if (wordFound == 1)
                {
                    finalOccurrences[0] = occurrence;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int findNeigbouringOccurence(char *word, long size, char gridArray[size][size], OCCURRENCE *finalOccurrences, OCCURRENCE occurrence)
{
    int isLastCharacter = 0;
    if (occurrence.characterIndex == (strlen(word) - 2))
    {
        isLastCharacter = 1;
    }

    // check character above
    if (occurrence.row != 0)
    {
        if (gridArray[occurrence.row - 1][occurrence.column] == word[occurrence.characterIndex + 1])
        {
            OCCURRENCE nextOccurrence;
            nextOccurrence.characterIndex = occurrence.characterIndex + 1;
            nextOccurrence.row = occurrence.row - 1;
            nextOccurrence.column = occurrence.column;

            if (!isLastCharacter)
            {
                int wordFound = findNeigbouringOccurence(word, size, gridArray, finalOccurrences, nextOccurrence);
                if (wordFound == 1)
                {
                    finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                    return 1;
                }
            }
            else
            {
                finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                return 1;
            }
        }
    }

    // check character below
    if (occurrence.row != size)
    {
        if (gridArray[occurrence.row + 1][occurrence.column] == word[occurrence.characterIndex + 1])
        {
            OCCURRENCE nextOccurrence;
            nextOccurrence.characterIndex = occurrence.characterIndex + 1;
            nextOccurrence.row = occurrence.row + 1;
            nextOccurrence.column = occurrence.column;

            if (!isLastCharacter)
            {
                int wordFound = findNeigbouringOccurence(word, size, gridArray, finalOccurrences, nextOccurrence);
                if (wordFound == 1)
                {
                    finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                    return wordFound;
                }
            }
            else
            {
                finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                return 1;
            }
        }
    }

    // check character left
    if (occurrence.column != 0)
    {
        if (gridArray[occurrence.row][occurrence.column - 1] == word[occurrence.characterIndex + 1])
        {
            OCCURRENCE nextOccurrence;
            nextOccurrence.characterIndex = occurrence.characterIndex + 1;
            nextOccurrence.row = occurrence.row;
            nextOccurrence.column = occurrence.column - 1;

            if (!isLastCharacter)
            {
                int wordFound = findNeigbouringOccurence(word, size, gridArray, finalOccurrences, nextOccurrence);
                if (wordFound)
                {
                    finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                    return 1;
                }
            }
            else
            {
                finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                return 1;
            }
        }
    }

    // check character right
    if (occurrence.column != size)
    {
        if (gridArray[occurrence.row][occurrence.column + 1] == word[occurrence.characterIndex + 1])
        {
            OCCURRENCE nextOccurrence;
            nextOccurrence.characterIndex = occurrence.characterIndex + 1;
            nextOccurrence.row = occurrence.row;
            nextOccurrence.column = occurrence.column + 1;

            if (!isLastCharacter)
            {
                int wordFound = findNeigbouringOccurence(word, size, gridArray, finalOccurrences, nextOccurrence);
                if (wordFound)
                {
                    finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                    return 1;
                }
            }
            else
            {
                finalOccurrences[nextOccurrence.characterIndex] = nextOccurrence;
                return 1;
            }
        }
    }

    return 0;
}

void highlightOccurrences(long size, char (*gridArray)[size], int wordLength, OCCURRENCE *finalOccurrences)
{
    for (int i = 0; i < wordLength; i++)
    {
        gridArray[finalOccurrences[i].row][finalOccurrences[i].column] = toupper(gridArray[finalOccurrences[i].row][finalOccurrences[i].column]);
    }
}

void outputGrid(char *grid, long size, char gridArray[size][size])
{
    // character + space to the right + 3 characters space for pipe characters left and rigth and space left + newline
    int rowSize = (size * 2 + 4);
    // characters per row for each row + a padding row at the top and bottom
    int resultSize = rowSize * (size + 2);

    char result[resultSize];

    for (int i = 0; i < (rowSize - 1); i++)
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

    for (int i = 0; i < (rowSize - 1); i++)
    {
        strcat(result, "+");
    }
    strcat(result, "\n");

    printf("%s", result);
}