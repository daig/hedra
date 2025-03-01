#!/bin/bash

# Fix erroneous includes created by the previous script
find . -type f -name "*.h" -o -name "*.c" | xargs grep -l "#include <.*/>.*>" | while read file; do
  echo "Fixing includes in $file"
  sed -i '' 's/#include <\([^/]*\)\/>\([^>]*\)>/#include <\1\/\2>/g' "$file"
done

echo "Include statements fixed" 