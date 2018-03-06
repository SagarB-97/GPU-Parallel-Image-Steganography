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
    long long totPixels = (long long)width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char *inputImageData = ppmTochar(inData, width, height);
    unsigned char *outputImageData = (unsigned char *)malloc(totPixels * 3ll);
    //--------------------------------------------------------------------------//

    // Read input audio file
    MP3File *inpAudio = readMP3(inputAudioFile);
    char *audioData = inpAudio->data;
    //--------------------------------------------------------------------------//

    cout << "Size of audio file = " << inpAudio->size << " bytes ("
         << (inpAudio->size * 8) << " bits)\n";
    cout << "Size of image file = " << totPixels * 3 << " bytes\n";

    // Check if audio file can be hidden in the given image
    if (totPixels * 3 < inpAudio->size * 8)
    {
        cout << "The image size is too small to hide the image";
        exit(0);
    }

    // Steganography
    long long imgCurrentPixel = 0;
    clock_t startTime = clock();
    for (long int i = 0; i < inpAudio->size ; i++)
    {
        unsigned char x = (unsigned char)audioData[i];
        std::bitset<8> audioByte(x);

        int bitC = 7;
        while (bitC >= 0)
        {
            outputImageData[imgCurrentPixel] = (inputImageData[imgCurrentPixel] | 1) & (254 + audioByte[bitC]);
            bitC -= 1;
            imgCurrentPixel += 1;
        }
    }
    while (imgCurrentPixel < totPixels * 3)
    {
        outputImageData[imgCurrentPixel] = inputImageData[imgCurrentPixel];
        imgCurrentPixel++;
    }
    clock_t endTime = clock();

    // Writing back output image
    char outputImageFile[] = "././Dataset/serial_output.ppm";
    writePPM(outputImageFile, outputImageData, inpImg->width, inpImg->height, 3);
    //--------------------------------------------------------------------------//

    // Time calculation
    double cpuTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "CPU Time taken (encrypt) = " << cpuTime * 1000 << " ms\n";

    free(audioData);
}