#!/bin/bash

main="main.cpp"
imageProcessing="./lib/ImageProcessing.cpp"
output="output.o"

# Compile main.cpp and ImageProcessing.cpp
if g++ "$main" "$imageProcessing" -o "$output"; then
    ./"$output"
else
    echo "Compilation failed."
    exit 1
fi

# Clean up the generated binary
rm -f "$output"
echo "Compiled binary was removed."
