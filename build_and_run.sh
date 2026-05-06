#!/bin/bash

# DMRC 3D Visualizer - Build & Run Script
# This script compiles the C++ engine to WebAssembly and starts a local server.

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== DMRC 3D Visualizer Build System ===${NC}"

# 1. Check for Emscripten
if ! command -v emcc &> /dev/null
then
    echo -e "${RED}Error: emcc (Emscripten) not found.${NC}"
    echo "Please install emsdk and run 'source emsdk_env.sh' before running this script."
    exit 1
fi

# 2. Compile WASM Engine
echo -e "${BLUE}Compiling C++ Engine to WebAssembly...${NC}"

emcc -O3 -std=c++20 \
    -Iinclude \
    src/Graph.cpp \
    src/Router.cpp \
    src/WasmBridge.cpp \
    --bind \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME='DMRCModule' \
    -s WASM=1 \
    -o dmrc_engine.js

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build Successful! Generated dmrc_engine.js and dmrc_engine.wasm${NC}"
else
    echo -e "${RED}Build Failed.${NC}"
    exit 1
fi

# 3. Start Local Server
echo -e "${BLUE}Starting Local Server at http://localhost:8000 ...${NC}"
echo -e "${BLUE}Press Ctrl+C to stop.${NC}"

# Try python3 first, then python, then node
if command -v python3 &> /dev/null
then
    python3 -m http.server 8000
elif command -v python &> /dev/null
then
    python -m SimpleHTTPServer 8000
elif command -v npx &> /dev/null
then
    npx serve -p 8000 .
else
    echo -e "${RED}No local server found. Please run a web server in this directory manually.${NC}"
fi
