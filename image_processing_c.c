#include "ppm.h"
#include <math.h>
#include <stdlib.h>
#include <inttypes.h>

static void blur32(const PPMImage *imageIn, int size, uint32_t **bufs)
{
    int bufIndex = 0;
    for (int i = 0; i < 5; ++i)
    {
        // printf("%d, %d, %d, %d\n", imageIn->pixels[1920 * 1080 * 3 - 4], imageIn->pixels[1920 * 1080 * 3 - 3], imageIn->pixels[1920 * 1080 * 3 - 2], imageIn->pixels[1920 * 1080 * 3 - 1]);
        for (int senterX = 0; senterX < imageIn->width; senterX++)
        {
            for (int senterY = 0; senterY < imageIn->height; senterY++)
            {
                uint64_t sumRed = 0;
                uint64_t sumGreen = 0;
                uint64_t sumBlue = 0;
                int countIncluded = 0;
                for (int x = -size; x <= size; x++)
                {

                    for (int y = -size; y <= size; y++)
                    {
                        int currentX = senterX + x;
                        int currentY = senterY + y;

                        // Check if we are outside the bounds
                        if (currentX < 0)
                            continue;
                        if (currentX >= imageIn->width)
                            continue;
                        if (currentY < 0)
                            continue;
                        if (currentY >= imageIn->height)
                            continue;

                        // Now we can begin
                        int offsetOfThePixel = (imageIn->width * currentY + currentX);
                        if (i == 0)
                        {
                            sumRed += (uint32_t)(imageIn->pixels[offsetOfThePixel * 3]) * (UINT32_MAX / UINT8_MAX);
                            sumGreen += (uint32_t)(imageIn->pixels[offsetOfThePixel * 3 + 1]) * (UINT32_MAX / UINT8_MAX);
                            sumBlue += (uint32_t)(imageIn->pixels[offsetOfThePixel * 3 + 2]) * (UINT32_MAX / UINT8_MAX);
                        }
                        else
                        {
                            sumRed += bufs[~bufIndex & 1][offsetOfThePixel * 3];
                            sumGreen += bufs[~bufIndex & 1][offsetOfThePixel * 3 + 1];
                            sumBlue += bufs[~bufIndex & 1][offsetOfThePixel * 3 + 2];
                        }

                        // Keep track of how many values we have included
                        countIncluded++;
                    }
                }

                // Update the output image
                int offsetOfThePixel = (imageIn->width * senterY + senterX);
                bufs[bufIndex][offsetOfThePixel * 3] = (sumRed + countIncluded / 2) / countIncluded;
                bufs[bufIndex][offsetOfThePixel * 3 + 1] = (sumGreen + countIncluded / 2) / countIncluded;
                bufs[bufIndex][offsetOfThePixel * 3 + 2] = (sumBlue + countIncluded / 2) / countIncluded;
            }
        }
        // printf("%f, %f, %f, %f\n", bufs[bufIndex][1920 * 1080 * 3 - 4], bufs[bufIndex][1920 * 1080 * 3 - 3], bufs[bufIndex][1920 * 1080 * 3 - 2], bufs[bufIndex][1920 * 1080 * 3 - 1]);
        bufIndex = ~bufIndex & 1;
    }
}

static void blur2(const PPMImage *imageIn, int size, double **bufs)
{
    int bufIndex = 0;
    for (int i = 0; i < 5; ++i)
    {
        // printf("%d, %d, %d, %d\n", imageIn->pixels[1920 * 1080 * 3 - 4], imageIn->pixels[1920 * 1080 * 3 - 3], imageIn->pixels[1920 * 1080 * 3 - 2], imageIn->pixels[1920 * 1080 * 3 - 1]);
        for (int senterX = 0; senterX < imageIn->width; senterX++)
        {
            for (int senterY = 0; senterY < imageIn->height; senterY++)
            {
                double sumRed = 0.0;
                double sumGreen = 0.0;
                double sumBlue = 0.0;
                int countIncluded = 0;
                for (int x = -size; x <= size; x++)
                {

                    for (int y = -size; y <= size; y++)
                    {
                        int currentX = senterX + x;
                        int currentY = senterY + y;

                        // Check if we are outside the bounds
                        if (currentX < 0)
                            continue;
                        if (currentX >= imageIn->width)
                            continue;
                        if (currentY < 0)
                            continue;
                        if (currentY >= imageIn->height)
                            continue;

                        // Now we can begin
                        int offsetOfThePixel = (imageIn->width * currentY + currentX);
                        if (i == 0)
                        {
                            sumRed += imageIn->pixels[offsetOfThePixel * 3];
                            sumGreen += imageIn->pixels[offsetOfThePixel * 3 + 1];
                            sumBlue += imageIn->pixels[offsetOfThePixel * 3 + 2];
                        }
                        else
                        {
                            sumRed += bufs[~bufIndex & 1][offsetOfThePixel * 3];
                            sumGreen += bufs[~bufIndex & 1][offsetOfThePixel * 3 + 1];
                            sumBlue += bufs[~bufIndex & 1][offsetOfThePixel * 3 + 2];
                        }

                        // Keep track of how many values we have included
                        countIncluded++;
                    }
                }

                // Update the output image
                int offsetOfThePixel = (imageIn->width * senterY + senterX);
                bufs[bufIndex][offsetOfThePixel * 3] = sumRed / countIncluded;
                bufs[bufIndex][offsetOfThePixel * 3 + 1] = sumGreen / countIncluded;
                bufs[bufIndex][offsetOfThePixel * 3 + 2] = sumBlue / countIncluded;
            }
        }
        // printf("%f, %f, %f, %f\n", bufs[bufIndex][1920 * 1080 * 3 - 4], bufs[bufIndex][1920 * 1080 * 3 - 3], bufs[bufIndex][1920 * 1080 * 3 - 2], bufs[bufIndex][1920 * 1080 * 3 - 1]);
        bufIndex = ~bufIndex & 1;
    }
}

static void diff(PPMImage *result, double **bufs)
{
    for (int i = 0; i < result->width * result->height * 3; i++)
    {
        result->pixels[i] = bufs[1][i] - bufs[0][i];
    }
}

static void diff32(PPMImage *result, uint32_t **bufs)
{
    for (int i = 0; i < result->width * result->height * 3; i++)
    {
        result->pixels[i] = (bufs[1][i] - bufs[0][i]) / (UINT32_MAX / UINT8_MAX);
    }
}

int main(int argc, char **argv)
{

    PPMImage image;
    // select where to read the image from
    if (argc > 1)
    {
        // from file for debugging (with argument)
        FILE *file = fopen("flower.ppm", "rb");
        readStreamPPM(&image, file);
        fclose(file);
    }
    else
    {
        // from stdin for cmb
        readStreamPPM(&image, stdin);
    }

    PPMImage result;
    result.height = image.height;
    result.width = image.width;
    result.pixels = malloc(result.height * result.width * 3);

    double *bufs[3];
    double *tmp;
    int bufIndex = 0;
    bufs[0] = malloc(sizeof(bufs[0][0]) * 3 * image.height * image.width * 3);
    bufs[1] = bufs[0] + 3 * image.height * image.width;
    bufs[2] = bufs[1] + 3 * image.height * image.width;

    // Process the tiny case:

    blur2(&image, 2, bufs);
    blur2(&image, 3, bufs + 1);
    diff(&result, bufs);
    // return 0;

    if (argc > 1)
    {
        FILE *file = fopen("flower_tiny.ppm", "wb");
        writeStreamPPM(&result, file);
        fclose(file);
    }
    else
    {
        writeStreamPPM(&result, stdout);
    }

    tmp = bufs[0];
    bufs[0] = bufs[1];
    bufs[1] = tmp;
    blur2(&image, 5, bufs + 1);
    diff(&result, bufs);

    if (argc > 1)
    {
        FILE *file = fopen("flower_small.ppm", "wb");
        writeStreamPPM(&result, file);
        fclose(file);
    }
    else
    {
        writeStreamPPM(&result, stdout);
    }

    tmp = bufs[0];
    bufs[0] = bufs[1];
    bufs[1] = tmp;
    blur2(&image, 8, bufs + 1);
    diff(&result, bufs);
    if (argc > 1)
    {
        FILE *file = fopen("flower_medium.ppm", "wb");
        writeStreamPPM(&result, file);
        fclose(file);
    }
    else
    {
        writeStreamPPM(&result, stdout);
    }
}
