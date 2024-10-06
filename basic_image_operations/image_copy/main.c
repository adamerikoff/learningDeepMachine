#include <stdio.h>

int main() {
    FILE *streamIN = fopen("./cameraman.bmp", "rb");
    FILE *fo = fopen("new_cameraman.bmp", "wb");

    if(streamIN == (FILE*)0) {
        printf("Unable to open file!");
    }

    unsigned char header[54];
    unsigned char colorTable[1024];

    for (int i = 0; i < 54; i++) {
        header[i] = getc(streamIN);
    }

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bit_depth = *(int*)&header[28];

    if (bit_depth <= 8) {
        fread(colorTable, sizeof(unsigned char), 1024, streamIN);
    }
    fwrite(header, sizeof(unsigned char), 54, fo);


    unsigned char buf[height * width];
    fread(buf, sizeof(unsigned char), (height * width), streamIN);
    if (bit_depth <= 8) {
        fwrite(colorTable, sizeof(unsigned char), 1024, fo);
    }
    fwrite(buf, sizeof(unsigned char), (height * width), fo);

    fclose(streamIN);
    fclose(fo);
    printf("Width: %d Height: %d\n", width, height);
    return 0;
}
