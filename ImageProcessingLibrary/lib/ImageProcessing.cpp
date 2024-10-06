#include "./ImageProcessing.h"
#include <cmath>
#include <cstdio>

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

// Helper function to count pixels above the threshold
int ImageProcessing::countPixelsAboveThreshold(unsigned char *data, int size, float threshold) {
    return std::count_if(data, data + size, [threshold](unsigned char pixel) {
        return pixel > threshold;
    });
}

// Helper function to calculate the mean of a group based on threshold
float ImageProcessing::calculateGroupMean(unsigned char *data, int size, float threshold, bool aboveThreshold, int groupCount) {
    if (groupCount == 0) return 0.0f;

    return std::accumulate(data, data + size, 0.0f, [threshold, aboveThreshold](float sum, unsigned char pixel) {
        return (aboveThreshold ? (pixel > threshold) : (pixel <= threshold)) ? sum + pixel : sum;
    }) / groupCount;
}


void ImageProcessing::iterativelyBinarizeImageData(unsigned char *inputImageData, unsigned char *outputImageData, int imageSize) {
    // Initialize random threshold between 0 and 255
    float currentThreshold = rand() % 256;
    float previousThreshold;
    const float thresholdConvergence = 1e-6; // Convergence limit
    float deltaThreshold = std::numeric_limits<float>::infinity(); // Infinite difference for the first loop

    while (deltaThreshold > thresholdConvergence) {
        int group1Count = countPixelsAboveThreshold(inputImageData, imageSize, currentThreshold);
        int group2Count = imageSize - group1Count;

        float group1Mean = calculateGroupMean(inputImageData, imageSize, currentThreshold, true, group1Count);
        float group2Mean = calculateGroupMean(inputImageData, imageSize, currentThreshold, false, group2Count);

        previousThreshold = currentThreshold;
        currentThreshold = 0.5f * (group1Mean + group2Mean);
        deltaThreshold = std::abs(currentThreshold - previousThreshold); // Difference between thresholds
    }

    // Binarize the image with the final threshold
    binarizeImageData(inputImageData, outputImageData, imageSize, static_cast<int>(currentThreshold));
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

void ImageProcessing::computeImageHistogram(unsigned char *_imageData, int imageRows, int imageColumns, float hist[]) {
    FILE *fptr;
    fptr = fopen("./histogram.txt", "w");
    if (fptr == nullptr) {
        printf("Error opening file!\n");
        return;
    }
    int x, y, i;
    long int histogram_index[256] = {0}; // Initialize to zero
    long int sum = 0;
    // Build histogram
    for (y = 0; y < imageRows; y++) {
        for (x = 0; x < imageColumns; x++) {
            int pixelValue = _imageData[x + y * imageColumns]; // Correct pixel access
            histogram_index[pixelValue]++;  // Increment corresponding bin
            sum++;
        }
    }
    // Normalize the histogram and write to output array
    for (i = 0; i <= 255; i++) {
        hist[i] = (float)histogram_index[i] / (float)sum; // Store normalized values in hist
    }
    // Write the histogram to the file
    for (i = 0; i <= 255; i++) {
        fprintf(fptr, "\n%f", hist[i]); // Corrected to print normalized histogram
    }
    fclose(fptr); // Ensure file is closed
}

void ImageProcessing::computeImageHistogramFile(unsigned char * _imageData, int imageRows, int imageColumns, float hist[],const char *histFile) {
    FILE *fptr;
    fptr = fopen(histFile, "w");
    if (fptr == nullptr) {
        printf("Error opening file!\n");
        return;
    }
    int x, y, i;
    long int histogram_index[256] = {0}; // Initialize to zero
    long int sum = 0;
    // Build histogram
    for (y = 0; y < imageRows; y++) {
        for (x = 0; x < imageColumns; x++) {
            int pixelValue = _imageData[x + y * imageColumns]; // Correct pixel access
            histogram_index[pixelValue]++;  // Increment corresponding bin
            sum++;
        }
    }
    // Normalize the histogram and write to output array
    for (i = 0; i <= 255; i++) {
        hist[i] = (float)histogram_index[i] / (float)sum; // Store normalized values in hist
    }
    // Write the histogram to the file
    for (i = 0; i <= 255; i++) {
        fprintf(fptr, "\n%f", hist[i]); // Corrected to print normalized histogram
    }
    fclose(fptr); // Ensure file is closed
}

void ImageProcessing::equalizeImageHistogram(unsigned char *_inputImageData, unsigned char *_outImageData, int imageRows, int imageColumns) {
    int x, y, i, j;
    int equalizedHistogram[256];
    float histogram[256];
    float sum;

    const char initialHistogram[] ="init_hist.txt";
    const char finalHistogram[] = "final_hist.txt";

    computeImageHistogramFile(&_inputImageData[0], imageRows, imageColumns, &histogram[0], initialHistogram);
    for (i = 0; i <= 255; i++) {
        sum = 0.0;
        for (j = 0; j <= i; j++){
            sum = sum + histogram[j];
        }
        equalizedHistogram[i] =  (int)(255 * sum + 0.5);

    }
    for (y = 0; y < imageRows; y++)
    {
        for (x = 0; x < imageColumns; x++)
        {
            *(_outImageData + x + y * imageColumns) = equalizedHistogram[*(_inputImageData + x + y * imageColumns)];
        }
    }
    computeImageHistogramFile(&_outImageData[0], imageRows, imageColumns, &histogram[0], finalHistogram);
}
