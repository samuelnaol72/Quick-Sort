#!/bin/bash

# Path to the test folder
folder="test_folder"

# Loop through each file in the test folder
for file in "$folder"/*.txt
do
    echo "Running ./test with input from $file"
    
    # Run the ./test program with the content of the file as input
    ./test < "$file"
    
    # Optional: Capture the output for each file
    # ./test < "$file" > "${file%.txt}_output.txt"
    
    # Optional: Add a delay if needed to prevent overwhelming system resources
    # sleep 0.1
done

echo "All tests completed."

