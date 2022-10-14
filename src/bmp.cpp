#include <stdio.h>
#include <stdlib.h>


void bgr2rgb(unsigned char *img, int w, int h) {
    for (int i = 0; i < w * h * 3; i = i + 3) {
        unsigned char temp;
        temp = img[i];
        img[i] = img[i + 2];
        img[i + 2] = temp;
    }
}

void WriteBMP(unsigned char *img, const char *filename, int w, int h) {
    bgr2rgb(img, w, h);
    int l = (w * 3 + 3) / 4 * 4;
    int bmi[] = {l * h + 54, 0, 54, 40, w, h, 1 | 3 * 8 << 16, 0, l * h, 0, 0, 100, 0};
    FILE *fp = fopen(filename, "wb");
    fprintf(fp, "BM");
    fwrite(&bmi, 52, 1, fp);
    fwrite(img, 1, l * h, fp);
    fclose(fp);
}