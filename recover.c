#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    //check for proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover card\n");
        return 1;
    }

    //remember file names
    char *card = argv[1];

    //open card.raw file
    FILE *inptr = fopen(card, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", card);
        return 2;
    }

    //create an output file pointer and initialise it to NULL
    FILE *outptr = NULL;

    //counter to keep track of images
    int counter = 0;

    //declaring pointer buffer and allocate memory
    BYTE *buffer = malloc(512 * sizeof(BYTE));

    //declare filename pointer and allocate memory
    char *imageNAME = malloc(8 * sizeof(char));

    while (fread(buffer, 1, BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check if there is an open file before opening a new file.
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            sprintf(imageNAME, "%03i.jpg", counter);
            outptr = fopen(imageNAME, "w");
            counter++;;
        }

        if (outptr != NULL)
        {
            fwrite(buffer, sizeof(char), BLOCK_SIZE, outptr);
        }
    }

    //close the last output file if it is still open
    if (outptr != NULL)
    {
        fclose(outptr);
    }

    //freeing memory and closing files
    free(imageNAME);
    free(buffer);
    fclose(inptr);

    return 0;
}