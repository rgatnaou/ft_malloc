#!/bin/bash
# Run the tests for the project

set -e
make
export LD_LIBRARY_PATH=..:$LD_LIBRARY_PATH

# Run the already compiled test binary
./test
echo "All tests passed!"
