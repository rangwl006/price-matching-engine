#!/bin/bash

cmake build -S /builder -B /builder/build
cmake --build /builder/build
cmake --install /builder/build
