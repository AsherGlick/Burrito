#!/bin/bash

# Base testcase directory
mkdir -p test_cases/$1

# Testcase configuration
touch test_cases/$1/testcase.yaml
echo 'input_paths: ' >> test_cases/$1/testcase.yaml
echo '    "pack": "xml"' >> test_cases/$1/testcase.yaml
echo 'expected_stdout: |' >> test_cases/$1/testcase.yaml
echo 'expected_stderr: |' >> test_cases/$1/testcase.yaml
echo 'expected_returncode: 0' >> test_cases/$1/testcase.yaml

# Input folder and test marker pack
mkdir -p test_cases/$1/input/pack

# Output folders and blank output files
mkdir -p test_cases/$1/output_proto
touch test_cases/$1/output_proto/markers.bin
mkdir -p test_cases/$1/output_xml
touch test_cases/$1/output_xml/xml_file.xml
