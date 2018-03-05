#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

#define THREADS_PER_BLOCK 512

__global__
void encrypt(unsigned char * inputImageData, unsigned char * outputImageData, int width, int height,
        char * audioData, int audioSize) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if(index < audioSize){
        unsigned char audioByte = (unsigned char)audioData[index];

        outputImageData[index * 8 + 0] = (inputImageData[index * 8 + 0] | 1) & (254 + ((audioByte>>7) & 1));
        outputImageData[index * 8 + 1] = (inputImageData[index * 8 + 1] | 1) & (254 + ((audioByte>>6) & 1));
        outputImageData[index * 8 + 2] = (inputImageData[index * 8 + 2] | 1) & (254 + ((audioByte>>5) & 1));
        outputImageData[index * 8 + 3] = (inputImageData[index * 8 + 3] | 1) & (254 + ((audioByte>>4) & 1));
        outputImageData[index * 8 + 4] = (inputImageData[index * 8 + 4] | 1) & (254 + ((audioByte>>3) & 1));
        outputImageData[index * 8 + 5] = (inputImageData[index * 8 + 5] | 1) & (254 + ((audioByte>>2) & 1));
        outputImageData[index * 8 + 6] = (inputImageData[index * 8 + 6] | 1) & (254 + ((audioByte>>1) & 1));
        outputImageData[index * 8 + 7] = (inputImageData[index * 8 + 7] | 1) & (254 + ((audioByte>>0) & 1));
    }
}


int main(int argc, char *argv[]){

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

    // Copy input image to device memory
    unsigned char *d_inputImageData, *d_outputImageData;
    cudaMalloc((void**)&d_inputImageData, totPixels * 3ll);
    cudaMalloc((void**)&d_outputImageData, totPixels * 3ll);

    cudaMemcpy(d_inputImageData,inputImageData,totPixels * 3ll,cudaMemcpyHostToDevice);
    cudaMemcpy(d_outputImageData,inputImageData,totPixels * 3ll,cudaMemcpyHostToDevice);    
    //--------------------------------------------------------------------------//


    // Read input audio file
    MP3File *inpAudio = readMP3(inputAudioFile);
    char *audioData = inpAudio->data;

    // Copy audio file to device memory
    char *d_audioData;
    cudaMalloc((void**)&d_audioData, inpAudio->size);
    cudaMemcpy(d_audioData, audioData, inpAudio->size, cudaMemcpyHostToDevice);
    //--------------------------------------------------------------------------//

    cout << "Size of audio file = " << inpAudio->size << " bytes ("
         << (inpAudio->size * 8) << " bits)\n";
    cout << "Size of image file = " << totPixels * 3 << " bytes\n";

    // Invoke Kernel
    int audioSize = inpAudio -> size;
    dim3 blockDim(THREADS_PER_BLOCK, 1, 1);
    dim3 gridDim((audioSize-1)/THREADS_PER_BLOCK + 1, 1, 1);

    encrypt<<<blockDim, gridDim>>>(d_inputImageData, d_outputImageData, width, height, d_audioData, 
            audioSize);
    //--------------------------------------------------------------------------//

    // Writing result to host
    cudaMemcpy(outputImageData, d_outputImageData, totPixels * 3 ,cudaMemcpyDeviceToHost);

    // Writing back output image
    char outputImageFile[] = "././Dataset/parallel_output.ppm";
    writePPM(outputImageFile, outputImageData, inpImg->width, inpImg->height, 3);
    //--------------------------------------------------------------------------//

    // Free memory
    free(audioData);
    cudaFree(d_inputImageData);
    cudaFree(d_outputImageData);
    cudaFree(d_audioData);
}