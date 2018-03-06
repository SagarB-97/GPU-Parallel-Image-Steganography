rm times_sameImage.txt
for filename in Dataset/audio/*.ogg; do
    echo "$filename" >> times_sameImage.txt
    ./encrypt "Dataset/images/falls_1.ppm" "$filename" >> times_sameImage.txt
    echo -e "\n" >>times_sameImage.txt
done