# #!/bin/bash
# # Run the tests for the project

# set -e
# make -C ..
# # gcc test_basic.c -I ../includes -L.. -lft_malloc_x86_64_Darwin -o test_basic
# gcc test_basic.c -I ../includes -L.. -lft_malloc_x86_64_Linux -o test_basic

# DYLD_LIBRARY_PATH=.. ./test_basic
# echo "All tests passed!"
#!/bin/bash
# Run the tests for the project

set -e
make -C ..
gcc test_basic.c -I ../includes -L.. -lft_malloc_x86_64_Linux -o test_basic
export LD_LIBRARY_PATH=..:$LD_LIBRARY_PATH
./test_basic
echo "All tests passed!"
