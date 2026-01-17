#!/bin/bash
# Run the tests for the project

set -e
make -C ..
gcc test_basic.c -I ../includes -L.. -lft_malloc_x86_64_Darwin -o test_basic
DYLD_LIBRARY_PATH=.. ./test_basic
echo "All tests passed!"
