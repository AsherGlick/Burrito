#!/bin/bash

if [[ $# -eq 0 || "$1" == "-h" || "$1" == "--help" ]]; then
    echo "make_test.sh is used to create a new integration test. It creates a skeleton of"
    echo "the files that are necessary for an integration test so that they can be easily"
    echo "edited, instead of needing to remember all of the files that must be created"
    echo "for each new integration test."
    echo ""
    echo "Usage:"
    echo "    ./make_test.sh <test_name>"
    echo "    ./make_test.sh my_new_test"
    echo "    ./make_test.sh [-h,--help]"
fi

# Fix the working directory if the script is being executed from somewhere
# other than its own directory.
cd "$(dirname "$0")"

# Base testcase directory
mkdir -p test_cases/$1


# Configuration Data
yaml_file="test_cases/${1}/testcase.yaml"
if [ ! -f "$yaml_file" ]; then
    touch "$yaml_file"
    echo 'input_paths: ' >> "$yaml_file"
    echo '    "pack": "xml"' >> "$yaml_file"
    echo 'expected_stdout: |' >> "$yaml_file"
    echo 'expected_stderr: |' >> "$yaml_file"
    echo 'expected_returncode: 0' >> "$yaml_file"
else
    echo "$yaml_file already exists, skipping setting it up"
fi


# Input folder and test marker pack
mkdir -p test_cases/$1/input/pack


# Output folders and blank output files
mkdir -p test_cases/$1/output_proto
touch test_cases/$1/output_proto/markers.bin
mkdir -p test_cases/$1/output_xml
touch test_cases/$1/output_xml/xml_file.xml
