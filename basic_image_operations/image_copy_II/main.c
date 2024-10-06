#include <stdio.h>

#define BMP_HEADER_SIZE       54
#define BMP_COLOUR_TABLE_SIZE 1024
#define CUSTOM_IMG_SIZE       (256 * 256)  // Add parentheses to ensure the correct multiplication

void ImageReader(
    const char* name,
    int* height,
    int* width,
    int* bitDepth,
    unsigned char* header,
    unsigned char* colourTable,
    unsigned char* buffer);

void ImageWriter(
    const char* name,
    int bitDepth,
    unsigned char* header,
    unsigned char* colorTable,
    unsigned char* buffer);

int main() {
    int imageWidth, imageHeight, imageBitDepth;
    unsigned char imageHeader[BMP_HEADER_SIZE];
    unsigned char imageColourTable[BMP_COLOUR_TABLE_SIZE];  // Fix size
    unsigned char imageBuffer[CUSTOM_IMG_SIZE];  // Fix size

    const char imageName[] = "./cameraman.bmp";
    const char newImageName[] = "./new_cameraman.bmp";

    ImageReader(imageName, &imageHeight, &imageWidth, &imageBitDepth, &imageHeader[0], &imageColourTable[0], &imageBuffer[0]);
    ImageWriter(newImageName, imageBitDepth, imageHeader, imageColourTable, imageBuffer);

    printf("Width: %d Height: %d\n", imageWidth, imageHeight);
    return 0;
}

void ImageReader(
    const char* name,
    int* height,
    int* width,
    int* bitDepth,
    unsigned char* header,
    unsigned char* colourTable,
    unsigned char* buffer)
{
    FILE *streamIN;
    streamIN = fopen(name, "rb");

    if (streamIN == NULL) {
        printf("Unable to open file!\n");
        return;  // Exit the function if the file cannot be opened
    }

    // Read header
    fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, streamIN);

    // Extract width, height, and bit depth from the header
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];
    *bitDepth = *(short*)&header[28];  // The bit depth is 2 bytes, use short

    // If bit depth <= 8, read the color table
    if (*bitDepth <= 8) {
        fread(colourTable, sizeof(unsigned char), BMP_COLOUR_TABLE_SIZE, streamIN);
    }

    // Read the image data
    fread(buffer, sizeof(unsigned char), CUSTOM_IMG_SIZE, streamIN);

    fclose(streamIN);
}

void ImageWriter(
    const char* name,
    int bitDepth,
    unsigned char* header,
    unsigned char* colourTable,
    unsigned char* buffer)
{
    FILE *fo = fopen(name, "wb");

    if (fo == NULL) {
        printf("Unable to open file for writing!\n");
        return;  // Exit the function if the file cannot be opened
    }

    // Write header
    fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, fo);

    // If bit depth <= 8, write the color table
    if (bitDepth <= 8) {
        fwrite(colourTable, sizeof(unsigned char), BMP_COLOUR_TABLE_SIZE, fo);
    }

    // Write the image data
    fwrite(buffer, sizeof(unsigned char), CUSTOM_IMG_SIZE, fo);

    fclose(fo);
}
