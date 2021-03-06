#include "ppmHelper.h"
#include "mp3Helper.h"

#include <bits/stdc++.h>

using namespace std;

#define THREADS_PER_BLOCK 512

__global__
void decrypt(unsigned char * inputImageData, int width, int height,
        char * audioData, long long audioSize) {
    long long index = blockIdx.x * blockDim.x + threadIdx.x;

    if(index < audioSize){
        unsigned char audioByte = 0;

        audioByte |= (inputImageData[index * 8 + 0] & 1) << 7;
        audioByte |= (inputImageData[index * 8 + 1] & 1) << 6;
        audioByte |= (inputImageData[index * 8 + 2] & 1) << 5;
        audioByte |= (inputImageData[index * 8 + 3] & 1) << 4;
        audioByte |= (inputImageData[index * 8 + 4] & 1) << 3;
        audioByte |= (inputImageData[index * 8 + 5] & 1) << 2;
        audioByte |= (inputImageData[index * 8 + 6] & 1) << 1;
        audioByte |= (inputImageData[index * 8 + 7] & 1) << 0;
        
        audioData[index] = audioByte;
    }
}


int main(int argc, char *argv[]){

    char *inputImageFile = argv[1];
    long long audioSize = atoi(argv[2]);
    char *outputFileExtension = argv[3];    
    
    // Create Cuda Events //
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    ////////////////////////////

    // Read input image
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    long long totPixels = (long long)width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);
    unsigned char *inputImageData = ppmTochar(inData, width, height);

    // Copy input image to device memory
    unsigned char *d_inputImageData;
    cudaMalloc((void**)&d_inputImageData, totPixels * 3ll);

    cudaMemcpy(d_inputImageData,inputImageData,totPixels * 3ll,cudaMemcpyHostToDevice);
    //--------------------------------------------------------------------------//

    // Memory allocation for extracted audio
    char *extractedAudioData = (char *)malloc(audioSize);
    char *d_extractedAudioData;
    cudaMalloc((void**)&d_extractedAudioData, audioSize);
    //--------------------------------------------------------------------------//

    // Invoke Kernel
    dim3 blockDim(THREADS_PER_BLOCK, 1, 1);
    dim3 gridDim((audioSize-1)/THREADS_PER_BLOCK + 1, 1, 1);

    cudaEventRecord(start);    
    decrypt<<<blockDim, gridDim>>>(d_inputImageData, width, height, d_extractedAudioData, audioSize);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float gpuTime = 0;
    cudaEventElapsedTime(&gpuTime, start, stop);
    //--------------------------------------------------------------------------//

    // Copying result to host
    cudaMemcpy(extractedAudioData, d_extractedAudioData, audioSize ,cudaMemcpyDeviceToHost);


    // Writing back audio file
    char outputAudioFile[] = "././Dataset/parallel_output.mp3";
    strcat(outputAudioFile, outputFileExtension);
    writeMP3(outputAudioFile, extractedAudioData, audioSize);
    //--------------------------------------------------------------------------//

    // Time Print
    cout<<"GPU Time taken (decrypt) = "<<gpuTime<<" ms\n";
}