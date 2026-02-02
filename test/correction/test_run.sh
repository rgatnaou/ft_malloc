#!/bin/bash
# Usage: ./test_run.sh [test_file.c]
# If no argument is given, runs all test*.c in this directory

INC_PATH="../../includes"
LIB_PATH="../.."
LIB_NAME="libft_malloc_x86_64_Linux"

run_test() {
    src_file="$1"
    exe_file="${src_file%.c}.out"
    echo "Compiling $src_file..."
    gcc -o "$exe_file" "$src_file" -I"$INC_PATH" -L"$LIB_PATH" -lft_malloc_x86_64_Linux
    if [ $? -ne 0 ]; then
        echo "Compilation failed for $src_file"
        return 1
    fi
    echo "Running $exe_file..."
    LD_LIBRARY_PATH="$LIB_PATH" "./$exe_file"
    result=$?
    rm -f "$exe_file"
    return $result
}

if [ $# -eq 1 ]; then
    run_test "$1"
else
    for f in test*.c; do
        run_test "$f"
        echo "---"
    done
fi
