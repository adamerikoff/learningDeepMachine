#include <iostream>
#include "./lib/ImageProcessing.h"

using namespace std;

int main() {
    float imageHistogram[N_GRAYLEVELS];

    int imageWidth, imageHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOUR_TABLE_SIZE];
    unsigned char imgInBuffer[_512by512_IMG_SIZE];
    unsigned char imgOutBuffer[_512by512_IMG_SIZE];

    const char imgName[] ="./images/cameraman.bmp";
    const char newImgName[] ="./images/cameraman_bright.bmp";

    ImageProcessing *myImage  = new ImageProcessing(imgName,
                                                    newImgName,
                                                    &imageHeight,
                                                    &imageWidth,
                                                    &imgBitDepth,
                                                    &imgHeader[0],
                                                    &imgColorTable[0],
                                                    &imgInBuffer[0],
                                                    &imgOutBuffer[0]
                                                    );

    myImage->readImage();
    // myImage->iterativelyBinarizeImageData(imgInBuffer, imgOutBuffer, _512by512_IMG_SIZE);
    // myImage->increaseBrightness(imgInBuffer, imgOutBuffer, _512by512_IMG_SIZE, 100);
    // myImage->computeImageHistogram(imgInBuffer, imageHeight, imageWidth, imageHistogram);
    myImage->equalizeImageHistogram(imgInBuffer, imgOutBuffer, imageHeight, imageWidth);
    myImage->writeImage();

    cout << "Success !" << endl;
    cout << "Image Height : " << imageHeight << endl;
    cout << "Image Width  : " << imageWidth << endl;

    return 0;
}
