# GPU Parallel Image Steganography

The goal of the project is to conceal audio in image while also trying to maintain the noise in the image to a minimum.

## Encrypting / Concealing Audio

```
g++ Encrypt.cpp -o encrypt
./encrypt Dataset/test.ppm Dataset/test.mp3
```

Where,<br>
Dataset/test.ppm is the image in which audio must be concealed<br>
Dataset/test.mp is the audio file to be concealed<br>

## Decrypting / Retrieving Audio

```
g++ Decrypt.cpp -o decrypt
./decrypt Dataset/output.ppm 25823
```

Where,<br>
Dataset/output.ppm is the image in which audio is concealed<br>
25823 is the size of the audio in bytes<br>