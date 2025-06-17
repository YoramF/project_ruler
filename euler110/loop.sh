#!/bin/bash

FILE="$1"

# Check if the file exists
if [ ! -f "$FILE" ]; then
  echo "Error: File '$FILE' not found!"
  exit 1
fi

echo "--- Processing lines from '$FILE' ---"

# Loop over each line in the file
# IFS=         : Prevents leading/trailing whitespace trimming.
# read -r line : Reads a line into the 'line' variable.
#                -r prevents backslash escapes from being interpreted.
# < "$FILE"    : Redirects the content of the file as standard input to the while loop.
while IFS= read -r line || [ -n "$line" ]; do
  # Print the line and its length to show that spaces are preserved
  echo "  executing: euler110 \"$line\""
  echo "  executing: euler110 \"$line\"" >> $2
  ./euler110 $line >> $2
  echo "-------------" >> $2
done < "$FILE"

echo "--- Finished processing lines ---"

