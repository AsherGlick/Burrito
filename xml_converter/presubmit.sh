#!/bin/bash

cpplint --quiet --recursive --exclude="src/rapidxml-1.13" --filter=-whitespace/newline,-whitespace/line_length,-readability/braces,-legal/copyright,-build/namespaces,-readability/casting,-build/c++11 src/
# TODO: remove readability/casting from the filter. It was temporarily added
# because the changes required would need testing unfitting of the original
# style check update commit.

# Run Include What You Use
iwyu_tool -p .

# Run the python presubmit for the "generators" subdirectory.
pushd generators
./presubmit.sh
popd
