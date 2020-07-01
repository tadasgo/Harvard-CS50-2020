#include "helpers.h"
#include <math.h>
#include <stdlib.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float rgbGrey;

    // loop rows
    for (int i = 0; i < height; i++)
    {
        // loop individual pixels in row
        for (int j = 0; j < width; j++)
        {
            // average color intensity, to know how dark or light grey color should be
            rgbGrey = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // set average intensity. b=g=r=grey
            image[i][j].rgbtBlue = rgbGrey;
            image[i][j].rgbtGreen = rgbGrey;
            image[i][j].rgbtRed = rgbGrey;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // temp for value swapping
    int temp[3];

    // loop rows
    for (int i = 0; i < height; i++)
    {
        // loop half of rows pixels
        for (int j = 0; j < width / 2; j++)
        {
            // save first value to temp
            temp[0] = image[i][j].rgbtBlue;
            temp[1] = image[i][j].rgbtGreen;
            temp[2] = image[i][j].rgbtRed;

            //swap pixels from right to left
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

            // swap pixels from temp (left) to right
            image[i][width - 1 - j].rgbtBlue = temp[0];
            image[i][width - 1 - j].rgbtGreen = temp[1];
            image[i][width - 1 - j].rgbtRed = temp[2];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float sumBlue, sumGreen, sumRed;
    int counter;

    // array to store blurred pixel values to for use AFTER all pixels are scanned
    RGBTRIPLE temp[height][width];

    // rows
    for (int i = 0; i < height; i++)
    {
        // pixels
        for (int j = 0; j < width; j++)
        {
            // reset values for each pixel
            sumBlue = 0.0;
            sumGreen = 0.0;
            sumRed = 0.0;
            counter = 0;

            // iterates neighboring pixels (y axis)
            for (int k = -1; k < 2; k++)
            {
                // skip iteration if outside of picture
                if (i + k < 0 || i + k > height - 1)
                {
                    continue;
                }
                // iterates neighboring pixels (x axis)
                for (int m = -1; m < 2; m++)
                {
                    if (j + m < 0 || j + m > width - 1)
                    {
                        continue;
                    }
                    // sums the value of all pixels
                    sumBlue += image[i + k][j + m].rgbtBlue;
                    sumGreen += image[i + k][j + m].rgbtGreen;
                    sumRed += image[i + k][j + m].rgbtRed;
                    counter++;
                }
            }
            // split the sum by the amount of pixels and set to temp variable
            temp[i][j].rgbtBlue = round(sumBlue / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtRed = round(sumRed / counter);
        }
    }
    // blur the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
}

int limit(int rgb);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //temp variables for calculations
    float sumBlueY, sumGreenY, sumRedY;
    float sumBlueX, sumGreenX, sumRedX;
    RGBTRIPLE temp[height][width];

    //  Sobel modifiers
    int modifierX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int modifierY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // rows
    for (int i = 0; i < height; i++)
    {
        // pixels
        for (int j = 0; j < width; j++)
        {
            // reset values
            sumBlueY = 0.0;
            sumGreenY = 0.0;
            sumRedY = 0.0;
            sumBlueX = 0.0;
            sumGreenX = 0.0;
            sumRedX = 0.0;

            // iterates neighboring pixels (y axis)
            for (int k = -1; k < 2; k++)
            {
                // skip iteration if outside of picture
                if (i + k < 0 || i + k > height - 1)
                {
                    continue;
                }
                // iterates neighboring pixels (x axis)
                for (int m = -1; m < 2; m++)
                {
                    if (j + m < 0 || j + m > width - 1)
                    {
                        continue;
                    }
                    // get pixel values apply Sobel modifiers and sum them up
                    sumBlueX += image[i + k][j + m].rgbtBlue * modifierX[k + 1][m + 1];
                    sumGreenX += image[i + k][j + m].rgbtGreen * modifierX[k + 1][m + 1];
                    sumRedX += image[i + k][j + m].rgbtRed * modifierX[k + 1][m + 1];

                    sumBlueY += image[i + k][j + m].rgbtBlue * modifierY[k + 1][m + 1];
                    sumGreenY += image[i + k][j + m].rgbtGreen * modifierY[k + 1][m + 1];
                    sumRedY += image[i + k][j + m].rgbtRed * modifierY[k + 1][m + 1];
                }
            }
            // combine X and Y pixel values
            temp[i][j].rgbtBlue = limit(round(sqrt(sumBlueX * sumBlueX + sumBlueY * sumBlueY)));
            temp[i][j].rgbtGreen = limit(round(sqrt(sumGreenX * sumGreenX + sumGreenY * sumGreenY)));
            temp[i][j].rgbtRed = limit(round(sqrt(sumRedX * sumRedX + sumRedY * sumRedY)));
        }
    }
    // copy values from temp table and apply edges
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
}

// limits values from going over 255
int limit(int rgb)
{
    if (rgb > 255)
    {
        rgb = 255;
    }
    return rgb;
}