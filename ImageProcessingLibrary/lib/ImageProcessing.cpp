#include "./ImageProcessing.h"

ImageProcessing::ImageProcessing(
    const char *_inImageName,
    const char *_outImageName,
    int *_height,
    int *_width,
    int *_bitDepth,
    unsigned char *_header,
    unsigned char *_colourTable,
    unsigned char *_inBuffer,
    unsigned char *_outBuffer)
{
    inImageName = _inImageName;
    outImageName = _outImageName;
    height = _height;
    width = _width;
    bitDepth = _bitDepth;
    header = _header;
    colourTable = _colourTable;
    inBuffer = _inBuffer;
    outBuffer = _outBuffer;
}

ImageProcessing::~ImageProcessing() {
}

void ImageProcessing::readImage() {
    // int i;
    FILE *streamIN;
    streamIN = fopen(inImageName, "rb");
    if (streamIN == NULL) {
        std::cout << "Unable to open file! File might not exist!" << std::endl;
        exit(0);  // Exit the function if the file cannot be opened
    }
    // Read header
    for (int i = 0; i < 54; i++) {
        header[i] = getc(streamIN);
    }
    // Extract width, height, and bit depth from the header
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];
    *bitDepth = *(int*)&header[28];
    // If bit depth <= 8, read the color table
    if (*bitDepth <= 8) {
        fread(colourTable, sizeof(unsigned char), BMP_COLOUR_TABLE_SIZE, streamIN);
    }
    // Read the image data
    fread(inBuffer, sizeof(unsigned char), _512by512_IMG_SIZE, streamIN);

    fclose(streamIN);
}

void ImageProcessing::writeImage() {
    FILE *fo = fopen(outImageName, "wb");
    if (fo == NULL) {
        std::cout << "Unable to open file for writing!" << std::endl;
        exit(0);  // Exit the function if the file cannot be opened
    }
    // Write header
    fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, fo);
    // If bit depth <= 8, write the color table
    if (*bitDepth <= 8) {
        fwrite(colourTable, sizeof(unsigned char), BMP_COLOUR_TABLE_SIZE, fo);
    }
    // Write the image data
    fwrite(outBuffer, sizeof(unsigned char), _512by512_IMG_SIZE, fo);
    fclose(fo);
}

void ImageProcessing::copyImageData(unsigned char *_sourceBuffer, unsigned char *_destinationBuffer, int bufferSize) {
    for (int i = 0; i < bufferSize; i++) {
        _destinationBuffer[i] = _sourceBuffer[i];
    }
}

void ImageProcessing::binarizeImageData(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int threshold) {
    for (int i = 0; i < imageSize; i++) {
        _outImageData[i] = (_inputImageData[i] > threshold) ? COLOUR_WHITE : COLOUR_BLACK;
    }
}

void ImageProcessing::increaseBrightness(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int brightness) {
    for (int i = 0; i < imageSize; i++) {
        int temp = _inputImageData[i] + brightness;
        _outImageData[i] = (temp > MAX_COLOUR) ? MAX_COLOUR : temp;
    }
}

void ImageProcessing::decreaseBrightness(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int brightness) {
    for (int i = 0; i < imageSize; i++) {
        int temp = _inputImageData[i] - brightness;
        _outImageData[i] = (temp > MIN_COLOUR) ? MIN_COLOUR : temp;
    }
}
