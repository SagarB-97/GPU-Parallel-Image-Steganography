#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{

    char *inputImageFile = argv[1];
    int audioSize = atoi(argv[2]);

    // Read input image
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    int totPixels = width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char *inputImageData = ppmTochar(inData, width, height);
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
            audioByte[bitC] = inputImageData[imgCurrentPixel] & 1;
            bitC -= 1;
            imgCurrentPixel += 1;
        }
        extractedAudioData[audioCurrentByte++] = static_cast<unsigned char>(audioByte.to_ulong());
    }

    // Writing back audio file
    char outputAudioFile[] = "Dataset/output.txt";
    writeMP3(outputAudioFile, extractedAudioData, audioSize);
    //--------------------------------------------------------------------------//
}