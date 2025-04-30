mv xml_converter burrito_converter
sed -i 's|Download XML Converter|Download Burrito Converter|g' .github/workflows/main.yml
sed -i 's|xml_converter|burrito_converter|g' .github/workflows/diff_protobin.yml
sed -i 's|xml_converter|burrito_converter|g' .github/workflows/main.yml
sed -i 's|xml_converter|burrito_converter|g' .github/workflows/release.yml
sed -i 's|"./xml_converter"|"./burrito_converter"|g' FileHandler.gd
sed -i 's|call_xml_converter|call_burrito_converter|g' FileHandler.gd
sed -i 's|"/build/xml_converter"|"/build/burrito_converter"|g' FileHandler.gd
sed -i 's|call_xml_converter|call_burrito_converter|g' ImportPackDialog.gd
sed -i 's|call_xml_converter|call_burrito_converter|g' Spatial.gd
sed -i 's|# Parses the stdout of the xml_converter|# Parses the stdout of burrito_converter|g' identify_duplicate_categories.gd

cd burrito_converter
mv src/xml_converter.cpp src/burrito_converter.cpp
sed -i 's|xml_converter_binary_path|burrito_converter_binary_path|g' integration_tests/run_tests.py
sed -i 's|"../build/xml_converter"|"../build/burrito_converter"|g' integration_tests/run_tests.py
sed -i 's|run_xml_converter|run_burrito_converter|g' integration_tests/run_tests.py
sed -i 's|rebuild_xml_converter_binary|rebuild_burrito_converter_binary|g' integration_tests/run_tests.py
sed -i 's|Nonzero return code from xml_converter|Nonzero return code from burrito_converter|g' integration_tests/run_tests.py
sed -i 's|# Prints the results of xml_converter|# Prints the results of burrito_converter|g' integration_tests/run_tests.py
sed -i 's|Do not automatically build xml_converter before running the integration tests.|Do not automatically build burrito_converter before running the integration tests.|g' integration_tests/run_tests.py
sed -i 's|set(TARGET_NAME xml_converter)|set(TARGET_NAME burrito_converter)|g' CMakeLists.txt
sed -i 's|src/xml_converter.cpp|src/burrito_converter.cpp|g' CMakeLists.txt
sed -i 's|set(TEST_TARGET_NAME test_xml_converter)|set(TEST_TARGET_NAME test_burrito_converter)|g' CMakeLists.txt
sed -i 's|(char\*)"./xml_converter",|(char*)"./burrito_converter",|g' tests/test_argument_parser.cpp
sed -i 's|usage: ./xml_converter|usage: ./burrito_converter|g' tests/test_argument_parser.cpp
sed -i 's|usage: ./xml_converter|usage: ./burrito_converter|g' src/argument_parser.cpp
sed -i 's|This might be a bug in xml_converter|This might be a bug in burrito_converter|g' src/packaging_xml.cpp
sed -i 's|# Recompiles the XML Converter binary.|# Recompiles the Burrito Converter binary.|g' integration_tests/run_tests.py
sed -i 's|print("Building XML Converter Binary")|print("Building Burrito Converter Binary")|g' integration_tests/run_tests.py
sed -i 's|// The universal entrypoint into the xml converter functionality.|// The universal entrypoint into burrito converter functionality.|g' src/burrito_converter.cpp
sed -i 's|// Main is the CLI entrypoint to the xml converter.|// Main is the CLI entrypoint to burrito converter.|g' src/burrito_converter.cpp
sed -i 's|The XML Converter Module is tasked with parsing XML marker files and loading them into Burrito.|The Burrito Converter Module is tasked with parsing various types of marker files and converting them into other types of marker files.|g' README.md
