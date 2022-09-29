#!/bin/bash
FORMAT_CMD="clang-format-14 -i -style=LLVM"
echo ${FORMAT_CMD} 
${FORMAT_CMD} include/* src/*
pushd test
${FORMAT_CMD} *.c
popd
