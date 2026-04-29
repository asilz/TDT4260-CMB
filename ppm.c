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

    while (fgetc(fp) != '\n')
        ;

    result->pixels = malloc(result->width * result->height * 3);
    fread(result->pixels, result->width * result->height * 3, 1, fp);

    return 0;
}
int writeStreamPPM(const PPMImage *img, FILE *fp)
{
    fwrite("P6\n", sizeof("P6\n") - 1, 1, fp);
    fprintf(fp, "%d %d\n", img->width, img->height);
    fwrite("255\n", sizeof("255\n") - 1, 1, fp);
    fwrite(img->pixels, 3 * img->width * img->height, 1, fp);

    return 0;
}