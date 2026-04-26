#include "ppm.h"
#include <stdlib.h>

int readStreamPPM(PPMImage *result, FILE *fp)
{
    if (fgetc(fp) != 'P')
    {
        return -1;
    }
    if (fgetc(fp) != '6')
    {
        return -1;
    }

    int maxColorVal;
    fscanf(fp, "\n%d %d %d", &result->width, &result->height, &maxColorVal);

    fseek(fp, 17, SEEK_SET);

    result->pixels = malloc(result->width * result->height * 3);
    fread(result->pixels, result->width * result->height * 3, 1, fp);
}
int writeStreamPPM(PPMImage *img, FILE *fp)
{
    fwrite("P6\n", sizeof("P6\n") - 1, 1, fp);
    fprintf(fp, "%d %d\n", img->width, img->height);
    fwrite("255\n", sizeof("255\n") - 1, 1, fp);
    fwrite(img->pixels, 3 * img->width * img->height, 1, fp);
}