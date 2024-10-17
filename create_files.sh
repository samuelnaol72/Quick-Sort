#!/bin/bash

# Create the test_folder if it doesn't already exist
mkdir -p test_folder

# Loop to create 1000 files
for i in {1..1000}
do
    # Generate a random file size between 10 and 1000 integers
    file_size=$((RANDOM % 991 + 10))  # Random number between 10 and 1000
    
    # Generate the filename with padded zeros for sorting
    filename=$(printf "test_folder/input_%04d.txt" $i)
    
    # Use shuf to create a file with the specified number of random integers
    shuf -i 1-10000 -n "$file_size" > "$filename"
    
    echo "Created $filename with $file_size random integers."
done

echo "All files created in test_folder."

