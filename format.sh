#!/bin/bash
FORMAT_CMD="clang-format-14 -i -style=LLVM"
${FORMAT_CMD} include/* src/*
pushd test
${FORMAT_CMD} *.c
popd
