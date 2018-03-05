#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{

    char *inputImageFile = argv[1];
    char *inputAudioFile = argv[2];

    // Read input image
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    int totPixels = width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char * inputImageData = ppmTochar(inData, width, height);
    unsigned char * outputImageData = (unsigned char *)malloc(totPixels * 3ll);
    //--------------------------------------------------------------------------//

    // Read input audio file
    MP3File *inpAudio = readMP3(inputAudioFile);
    char *audioData = inpAudio->data;
    int size = inpAudio->size;
    //--------------------------------------------------------------------------//

    cout << "Size of audio file = " << inpAudio->size << " bytes ("
         << (inpAudio->size * 8) << " bits)\n";
    cout << "Size of image file = " << totPixels * 3 << " bytes\n";

    // Steganography
    long long imgCurrentPixel = 0;
    // Storing the size of the audio in the corners of the image
    int LT = 0, RT = width * 3 - 3, LB = width*(height-1) * 3, RB = sizeof(outputImageData) - 3;
    outputImageData[LT] = (char)(255 & size);
    outputImageData[RT] = (char)((65280 & size) >> 8);
    outputImageData[LB] = (char)((16711680 & size) >> 16);
    outputImageData[RB] = (char)((4278190080 & size) >> 24);
    std::cout << (int)outputImageData[LT] << " " << (int)outputImageData[RT] << " " << (int)outputImageData[LB] << " " << (int)outputImageData[RB];
    for (long int i = 0; i < size; i++)
    {
        unsigned char x = (unsigned char)audioData[i];
        std::bitset<8> audioByte(x);

        int bitC = 7;
        while (bitC >= 0)
        {
            if((imgCurrentPixel != LT) && (imgCurrentPixel != RT) && (imgCurrentPixel != LB) && (imgCurrentPixel != RB))
            {
                outputImageData[imgCurrentPixel] = (inputImageData[imgCurrentPixel] | 1) & (254 + audioByte[bitC]);
                bitC -= 1;
            }
            imgCurrentPixel += 1;
        }
    }
    while (imgCurrentPixel < totPixels * 3)
    {
        if((imgCurrentPixel != LT) && (imgCurrentPixel != RT) && (imgCurrentPixel != LB) && (imgCurrentPixel != RB))
            outputImageData[imgCurrentPixel] = inputImageData[imgCurrentPixel];
        imgCurrentPixel++;
    }

    // Writing back output image
    char outputImageFile[] = "Dataset/output.ppm";
    writePPM(outputImageFile, outputImageData, inpImg->width, inpImg->height, 3);
    //--------------------------------------------------------------------------//

    free(audioData);
}