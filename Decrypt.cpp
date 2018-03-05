#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{

    char *inputImageFile = argv[1];
    int audioSize;// = atoi(argv[2]);

    // Read input image
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    int totPixels = width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char *inputImageData = ppmTochar(inData, width, height);
    int LT = 0, RT = width * 3 - 3, LB = width * (height - 1) * 3, RB = sizeof(inputImageData) - 3;

    //--------------------------------------------------------------------------//

    // Extract audio size
    audioSize = (int)inputImageData[LT] + (((int)inputImageData[RT]) << 8) + (((int)inputImageData[LB]) << 16) + (((int)inputImageData[RB]) << 24);//--------------------------------------------------------------------------//
    //--------------------------------------------------------------------------//

    // Steganography
    char *extractedAudioData = (char *)malloc(audioSize);
    long long imgCurrentPixel = 0;
    long long audioCurrentByte = 0;
    for (long int i = 0; i < audioSize; i++)
    {
        std::bitset<8> audioByte(0);

        int bitC = 7;
        while (bitC >= 0)
        {
            if ((imgCurrentPixel != LT) && (imgCurrentPixel != RT) && (imgCurrentPixel != LB) && (imgCurrentPixel != RB))
            {            
                audioByte[bitC] = inputImageData[imgCurrentPixel] & 1;
                bitC -= 1;
            }
            imgCurrentPixel += 1;
        }
        extractedAudioData[audioCurrentByte++] = static_cast<unsigned char>(audioByte.to_ulong());
    }

    // Writing back audio file
    char outputAudioFile[] = "Dataset/output.mp3";
    writeMP3(outputAudioFile, extractedAudioData, audioSize);
    //--------------------------------------------------------------------------//
}