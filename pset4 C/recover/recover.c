#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // check if user provided argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open memory card and check if it is opened
    FILE *file = fopen(argv[1], "r");
    // error handling if file  was not opened
    if (file == NULL)
    {
        printf("Could not open file %s\n", argv[1]);
        return 1;
    }

    FILE *img;                                  // pointer to a file
    char filename[7];                           // array for filename 000.jpg
    unsigned char *dataStorage = malloc(512);   // pointer to allocate memory for FAT file block
    int counter = 0;

    // read file 512 at the time and store to buffer
    while (fread(dataStorage, 512, 1, file))
    {
        // if jpg file found
        // bitwise arithmetic. (dataStorage[3] & 0xf0) just look to the first 4 bits, ignore the rest 4
        if (dataStorage[0] == 0xff && dataStorage[1] == 0xd8 && dataStorage[2] == 0xff && (dataStorage[3] & 0xf0) == 0xe0)
        {
            // when new jpg starts, close the previous if there is one
            if (counter > 0)
            {
                fclose(img);
            }

            // print filename into array
            sprintf(filename, "%03d.jpg", counter);
            // create and open new image file with name from filename array
            img = fopen(filename, "w");

            // error handling if jpg was not created
            if (img == NULL)
            {
                fclose(file);
                free(dataStorage);
                printf("Could not create output JPG %s", filename);
                return 2;
            }

            counter++;
        }

        //if not a new jpg, keep writing on currently opened one
        if (counter > 0)
        {
            fwrite(dataStorage, 512, 1, img);
        }
    }

    //frees memory and closes files
    fclose(img);
    fclose(file);
    free(dataStorage);
    return 0;
}