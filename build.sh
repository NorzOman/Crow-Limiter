#!/bin/bash

FOLDER="crow-limiter"
ZIP_OUTPUT="crow-limiter.zip"
SEVENZ_OUTPUT="crow-limiter.7z"

[ ! -d "$FOLDER" ] && echo "Error: Folder '$FOLDER' does not exist!" && exit 1

rm -f "$ZIP_OUTPUT" "$SEVENZ_OUTPUT"

zip -r "$ZIP_OUTPUT" "$FOLDER" || { echo "Failed to create ZIP"; exit 1; }

7z a "$SEVENZ_OUTPUT" "$FOLDER" || { echo "Failed to create 7z"; exit 1; }

echo "Build completed successfully: $ZIP_OUTPUT, $SEVENZ_OUTPUT"
