#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{

    char *inputImageFile = argv[1];
    long long audioSize = atoi(argv[2]);
    char *outputFileExtension = argv[3];    

    // Read input image
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    long long totPixels = (long long)width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char *inputImageData = ppmTochar(inData, width, height);
    //--------------------------------------------------------------------------//

    // Steganography
    char *extractedAudioData = (char *)malloc(audioSize);
    long long imgCurrentPixel = 0;
    long long audioCurrentByte = 0;    
    
    clock_t startTime = clock();
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
    clock_t endTime = clock();


    // Writing back audio file
    char outputAudioFile[] = "././Dataset/serial_output.";
    strcat(outputAudioFile, outputFileExtension);
    writeMP3(outputAudioFile, extractedAudioData, audioSize);
    //--------------------------------------------------------------------------//

    // Time calculation
    double cpuTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "CPU Time taken (decrypt) = " << cpuTime * 1000 << " ms\n";
}