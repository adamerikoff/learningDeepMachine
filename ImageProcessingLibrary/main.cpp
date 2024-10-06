#include <iostream>
#include "./lib/ImageProcessing.h"

using namespace std;

int main() {
    int imgWidth, imgHeight, imgBitDepth;
    unsigned char imgHeader[BMP_HEADER_SIZE];
    unsigned char imgColorTable[BMP_COLOUR_TABLE_SIZE];
    unsigned char imgInBuffer[_512by512_IMG_SIZE];
    unsigned char imgOutBuffer[_512by512_IMG_SIZE];

    const char imgName[] ="./cameraman.bmp";
    const char newImgName[] ="./cameraman_copy.bmp";

    ImageProcessing *myImage  = new ImageProcessing(imgName,
                                                    newImgName,
                                                    &imgHeight,
                                                    &imgWidth,
                                                    &imgBitDepth,
                                                    &imgHeader[0],
                                                    &imgColorTable[0],
                                                    &imgInBuffer[0],
                                                    &imgOutBuffer[0]
                                                    );

     myImage->readImage();
     myImage->copyImageData(imgInBuffer,imgOutBuffer,_512by512_IMG_SIZE);
     myImage->writeImage();

     cout<<"Success !"<<endl;
     cout<<"Image Height : "<<imgHeight<<endl;
     cout<<"Image Width  : "  <<imgWidth<<endl;

    return 0;
}
