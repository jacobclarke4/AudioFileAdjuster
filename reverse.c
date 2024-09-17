#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Can't open input\n");
        return 1;
    }
    // Read header
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input);
    // Use check_format to ensure WAV format
    // TODO #4
    if (check_format(header) == 1)
    {
        printf("Input is not a WAV file.");
        return 2;
    }
    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output);
    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    BYTE buffer[block_size];
    //put the pointer posiition to 1 block_size past the end of the input file
    fseek(input, block_size, SEEK_END);
    // Write reversed audio to file
    // TODO #8
    while (ftell(input) - block_size > sizeof(header))
    {
        //move pointer 2 block_sizes back(right to left) toward the beginning of the input file
        //we have to seek 2 block_size back because we will have to read 1 block_size forward
        fseek(input, -2 * block_size, SEEK_CUR);
        //read 1 block_size forward(left to right)
        fread(&buffer, sizeof(buffer), 1, input);
        fwrite(&buffer, sizeof(buffer), 1, output);
    }

    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 0;
    }
    return 1;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block_size = 0;
    block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}