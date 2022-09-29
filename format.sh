#!/bin/bash
FORMAT_CMD="clang-format-14 -i -style=GNU"
echo ${FORMAT_CMD} 
${FORMAT_CMD} include/* src/*
pushd test
${FORMAT_CMD} *.c
popd
