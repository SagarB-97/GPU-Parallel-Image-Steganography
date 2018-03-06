rm times_sameAudio.txt
for filename in Dataset/images/*.ppm; do
    if [ "$filename" != "serial_output.ppm" ]
    then
        echo "$filename" >> times_sameAudio.txt
        ./encrypt "$filename" "Dataset/audio/test.mp3" >> times_sameAudio.txt
        echo -e "\n" >>times_sameAudio.txt
    fi
done