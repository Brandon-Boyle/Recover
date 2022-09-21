#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
int main(int argc, char *argv[])
{
    //check to see if there are 2 command line arguments, if not return usage error code and 1
    if (argc != 2) 
    {
        printf("Usage ./recover raw.card");
        return 1;
    }
    // Open the file so we can read it
    FILE *input_file = fopen(argv[1], "r");
    // If the check for the input_file fails to open, then return the error "could not open file" and return 2
    if (input_file == NULL)
    {
        printf("Could not open file");
        return 2;
    }
    
    // Declare the vairiable buffer to unsigned char to store the 512 bit files
    unsigned char buffer[512];
    
    // declare the intiger variable count_image to later count the amount of images in the file with a loop
    int count_image = 0;
    
    // create an uninitalized file pointer to output the data recieved from the input file
    FILE *output_file = NULL;
    
    // Allocate enough memory with malloc to store the files that have been written
    char *filename = malloc(8 * sizeof(char));
    
    // Read the the 512 bytes from the input file and store them in the buffer array
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // Check to see if the first 4 bytes are the start of a JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // write the JPEG into the file name in forms of jpg.001, jpg.002, etc
            sprintf(filename, "%03i.jpg", count_image);
            
            // Open the output file in to now write the array from buffer into it
            output_file = fopen(filename, "w");
            
            //count the number of images that were found
            count_image++;
        }
        
        // Check to see if the output file has been used and is ready for valid input
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }
    
    free(filename);
    fclose(output_file);
    fclose(input_file);
    
    return 0;
}