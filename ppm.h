#include <stdio.h>

typedef struct
{
    int width;
    int height;
    unsigned char *pixels;
} PPMImage;

int readStreamPPM(PPMImage *result, FILE *fp);
int writeStreamPPM(PPMImage *img, FILE *fp);
