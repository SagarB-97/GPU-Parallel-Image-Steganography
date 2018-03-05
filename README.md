# GPU Parallel Image Steganography

The goal of the project is to conceal audio in image while also trying to maintain the noise in the image to a minimum.

## Serial Implementation
### Encrypting / Concealing Audio

```
g++ Serial/Encrypt.cpp -o encrypt
./encrypt Dataset/test.ppm Dataset/test.mp3
```

The resultant image named `serial_output.ppm` will be saved in `Dataset/`<br>

Where,<br>
Dataset/test.ppm is the image in which audio must be concealed<br>
Dataset/test.mp is the audio file to be concealed<br>

### Decrypting / Retrieving Audio

```
g++ Serial/Decrypt.cpp -o decrypt
./decrypt Dataset/output.ppm 25823
```
The resultant audio named `serial_output.mp3` will be saved in `Dataset/`<br>

Where,<br>
Dataset/output.ppm is the image in which audio is concealed<br>
25823 is the size of the audio in bytes<br>

## Parallel Implementation
### Encrypting / Concealing Audio

```
nvcc Parallel/Encrypt.cu -o encrypt
./encrypt Dataset/test.ppm Dataset/test.mp3
```
The resultant image named `parallel_output.ppm` will be saved in `Dataset/`<br>

Where,<br>
Dataset/test.ppm is the image in which audio must be concealed<br>
Dataset/test.mp is the audio file to be concealed<br>

### Decrypting / Retrieving Audio

```
nvcc Parallel/Decrypt.cu -o decrypt
./decrypt Dataset/output.ppm 25823
```
The resultant audio named `parallel_output.mp3` will be saved in `Dataset/`<br>

Where,<br>
Dataset/output.ppm is the image in which audio is concealed<br>
25823 is the size of the audio in bytes<br>