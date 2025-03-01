#!/bin/bash

# Update prelude includes
find prelude -type f -name "*.h" -o -name "*.c" | while read file; do
  # Update includes for prelude
  sed -i '' 's/#include "prelude\//#include <prelude\//g' "$file"
  # Update includes for ast
  sed -i '' 's/#include "ast\//#include <ptx_ast\//g' "$file"
  # Update includes for parse
  sed -i '' 's/#include "parse\//#include <ptx_parse\//g' "$file"
  # Update relative includes
  sed -i '' 's/#include "..\/ast\//#include <ptx_ast\//g' "$file"
  # Close angle brackets
  sed -i '' 's/\.h"/\.h>/g' "$file"
done

# Update ptx_ast includes
find ptx_ast -type f -name "*.h" -o -name "*.c" | while read file; do
  # Update includes for prelude
  sed -i '' 's/#include "prelude\//#include <prelude\//g' "$file"
  # Update includes for ast
  sed -i '' 's/#include "ast\//#include <ptx_ast\//g' "$file"
  # Update includes for parse
  sed -i '' 's/#include "parse\//#include <ptx_parse\//g' "$file"
  # Update relative includes
  sed -i '' 's/#include "..\/ast\//#include <ptx_ast\//g' "$file"
  # Update local includes with just filenames
  sed -i '' 's/#include "ptx_/#include <ptx_ast\/ptx_/g' "$file"
  # Handle c_directive and comment headers
  sed -i '' 's/#include "c_directive.h"/#include <ptx_ast\/c_directive.h>/g' "$file"
  sed -i '' 's/#include "comment.h"/#include <ptx_ast\/comment.h>/g' "$file"
  # Close angle brackets
  sed -i '' 's/\.h"/\.h>/g' "$file"
done

# Update ptx_parse includes
find ptx_parse -type f -name "*.h" -o -name "*.c" | while read file; do
  # Update includes for prelude
  sed -i '' 's/#include "prelude\//#include <prelude\//g' "$file"
  # Update includes for ast
  sed -i '' 's/#include "ast\//#include <ptx_ast\//g' "$file"
  # Update includes for parse
  sed -i '' 's/#include "parse\//#include <ptx_parse\//g' "$file"
  # Update relative includes
  sed -i '' 's/#include "..\/ast\//#include <ptx_ast\//g' "$file"
  # Close angle brackets
  sed -i '' 's/\.h"/\.h>/g' "$file"
done

# Update test files
find tests -type f -name "*.c" | while read file; do
  # Update includes for prelude
  sed -i '' 's/#include "prelude\//#include <prelude\//g' "$file"
  # Update includes for ast
  sed -i '' 's/#include "ast\//#include <ptx_ast\//g' "$file"
  # Update includes for parse
  sed -i '' 's/#include "parse\//#include <ptx_parse\//g' "$file"
  # Close angle brackets
  sed -i '' 's/\.h"/\.h>/g' "$file"
done

echo "Include statements updated." 