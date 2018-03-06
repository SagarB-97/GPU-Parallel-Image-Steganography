rm times.txt
for filename in Dataset/images/*.ppm; do
    if [ "$filename" != "Dataset/serial_output.ppm" ]
    then
        for filename2 in Dataset/audio/*.ogg; do
            echo "$filename" >> times.txt
            ./encrypt "$filename" "$filename2" >> times.txt
            echo -e "\n" >>times.txt
        done
    fi
done