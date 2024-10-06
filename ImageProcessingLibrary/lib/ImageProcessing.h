#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <iostream>

static const int _512by512_IMG_SIZE = 262144;
static const int BMP_COLOUR_TABLE_SIZE = 1024;
static const int BMP_HEADER_SIZE = 54;
static const int MAX_COLOUR = 255;
static const int MIN_COLOUR = 0;
static const int COLOUR_WHITE = MAX_COLOUR;
static const int COLOUR_BLACK = MIN_COLOUR;


class ImageProcessing {
    public:
        ImageProcessing();
        ImageProcessing(
            const char *_inImageName,
            const char *_outImageName,
            int *_height,
            int *_width,
            int *_bitDepth,
            unsigned char *_header,
            unsigned char *_colourTable,
            unsigned char *_inBuffer,
            unsigned char *_outBuffer);

        virtual ~ImageProcessing();

        void readImage();
        void writeImage();
        void copyImageData(unsigned char *_sourceBuffer, unsigned char *_destinationBuffer, int bufferSize);
        void binarizeImageData(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int threshold);
        void increaseBrightness(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int brightness);
        void decreaseBrightness(unsigned char *_inputImageData, unsigned char *_outImageData, int imageSize, int brightness);


    protected:

    private:
        const char *inImageName;
        const char *outImageName;
        int *height;
        int *width;
        int *bitDepth;
        unsigned char *header;
        unsigned char *colourTable;
        unsigned char *inBuffer;
        unsigned char *outBuffer;
};

#endif
