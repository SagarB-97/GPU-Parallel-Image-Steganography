#include "ppmHelper.h"
#include<bits/stdc++.h>

using namespace std;

int main()
{
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    char inputImageFile[] = "texture.ppm";
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    int totPixels = width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);


    int imgCounter = 0;

    pFile = fopen("Naruto Shippuden OST 1 - Track 02 - Douten ( Heaven Shaking Event ).mp3", "rb");

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);

    // copy the file into the buffer:
    //cout << sizeof(buffer) <<endl;
    int x;
    result = fread(buffer, 1, lSize, pFile);
    for (long int i = 0; i < 70000; i++){
        x = (unsigned char)buffer[i];
        std::bitset<8> a(x);
        // std::cout << a;

        int bitC = 0;
        while(bitC<8) {
            outData[imgCounter].b = (inData[imgCounter].b|1)&(254+a[bitC]);
            bitC+=1;
            // if (bitC == 8)
            // {
            //     imgCounter++;
            //     break;
            // }
            outData[imgCounter].g = (inData[imgCounter].g | 1) & (254 + a[bitC]);
            bitC+=1;
            // if (bitC == 8) {
            //     imgCounter++;
            //     break;
            // }
            outData[imgCounter].r = (inData[imgCounter].r | 1) & (254 + a[bitC]);
            bitC+=1;
            if (bitC == 8)
            {
                imgCounter++;
                break;
            }
            imgCounter++;
        }

        //cout << buffer[i];
    }
    while(imgCounter < totPixels) {
        outData[imgCounter] = inData[imgCounter];
        imgCounter++;
    }
    /* the whole file is now loaded in the memory buffer. */

    // terminate
    fclose(pFile);
    free(buffer);
    // while (getline(music, line))
    // {
    //     for (int i = 0; i < line.size();i++){
    //         //printf("%02xu", line[i]);
    //         x = (unsigned char)line[i];
    //         std::bitset<8> a(x);
    //         std::cout << a[0];
    //     }
    // }
    // music.close();
    char outputImageFile[] = "output.ppm";
    writePPM(outputImageFile, ppmTochar(outData, width, height), inpImg->width, inpImg->height, 3);
}

/*
#include "ppmHelper.h"
#include<bits/stdc++.h>

using namespace std;

int main()
{
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;

    char inputImageFile[] = "texture.ppm";
    PPMimg *inpImg = readPPM(inputImageFile);
    int width = inpImg->width;
    int height = inpImg->height;
    int totPixels = width * height;

    PPMpixel *inData = inpImg->data;
    PPMpixel *outData = (PPMpixel *)malloc(sizeof(PPMpixel) * totPixels);


    int imgCounter = 0;

    pFile = fopen("Naruto Shippuden OST 1 - Track 02 - Douten ( Heaven Shaking Event ).mp3", "rb");

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);

    // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char) * lSize);

    // copy the file into the buffer:
    //cout << sizeof(buffer) <<endl;
    int x;
    result = fread(buffer, 1, lSize, pFile);
    for (long int i = 0; i < 70000; i++){
        x = (unsigned char)buffer[i];
        std::bitset<8> a(x);
        // std::cout << a;

        int bitC = 0;
        while(bitC<8) {
            outData[imgCounter].b = (inData[imgCounter].b|1)&(126+a[bitC] + a[bitC+1]<<7);
            bitC+=2;
            if (bitC == 8)
            {
                imgCounter++;
                break;
            }
            outData[imgCounter].g = (inData[imgCounter].g|1)&(126+a[bitC] + a[bitC+1]<<7);
            bitC+=2;
            if (bitC == 8) {
                imgCounter++;
                break;
            }
            outData[imgCounter].r = (inData[imgCounter].r | 1)&(126 + a[bitC] + a[bitC+1]<<7);
            bitC+=2;
            if (bitC == 8)
            {
                imgCounter++;
                break;
            }
            imgCounter++;
        }

        //cout << buffer[i];
    }
    while(imgCounter < totPixels) {
        outData[imgCounter] = inData[imgCounter];
        imgCounter++;
    }
    /* the whole file is now loaded in the memory buffer. 

// terminate
fclose(pFile);
free(buffer);
// while (getline(music, line))
// {
//     for (int i = 0; i < line.size();i++){
//         //printf("%02xu", line[i]);
//         x = (unsigned char)line[i];
//         std::bitset<8> a(x);
//         std::cout << a[0];
//     }
// }
// music.close();
char outputImageFile[] = "output.ppm";
writePPM(outputImageFile, ppmTochar(outData, width, height), inpImg->width, inpImg->height, 3);
}
*/