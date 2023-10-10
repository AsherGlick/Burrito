#!/bin/bash

ATTRIBUTE_TEST_CASES_FOLDER_PATHS=("./test_cases/can_fade_tests")

ATTRIBUTE_TEST_CASES=()

for ATTRIBUTE_TEST_CASES_FOLDER_PATH in "${ATTRIBUTE_TEST_CASES_FOLDER_PATHS[@]}"; do
    if [ ! -d "$ATTRIBUTE_TEST_CASES_FOLDER_PATH" ]; then
        echo "Folder '$ATTRIBUTE_TEST_CASES_FOLDER_PATH' does not exist."
        exit 1
    fi

    attribute_name=$(basename "$ATTRIBUTE_TEST_CASES_FOLDER_PATH")
    TEST_OUTPUT_DIRECTORY="./test_cases/test_output/$attribute_name"

    if [ ! -d "$TEST_OUTPUT_DIRECTORY" ]; then
        mkdir "$TEST_OUTPUT_DIRECTORY"
    fi

    while IFS= read -r -d $'\0' file; do
        ATTRIBUTE_TEST_CASES+=("$file")
    done < <(find "$ATTRIBUTE_TEST_CASES_FOLDER_PATH" -type f -print0)
     
    for test_case in "${ATTRIBUTE_TEST_CASES[@]}"; do
        file_name=$(basename "$test_case")
        ./build/xml_converter --input-taco-path "$test_case" --output-taco-path "$TEST_OUTPUT_DIRECTORY/$file_name" 1> /dev/null
    done
done

./build/xml_converter_tests
