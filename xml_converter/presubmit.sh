#!/bin/bash

error_count=0

# Run cpplint
filters=""
  # Automated line length restrictions often cause more pain then they solve.
  # Lines should be kept in check via code review.
  filters+="-whitespace/line_length,"
  # These three are just wrong brace styling, fight me.
  filters+="-readability/braces,"
  filters+="-whitespace/parens,"
  filters+="-whitespace/newline,"
  # Copyright is handled on the repo level.
  # An argument could be made this should be uncommented.
  filters+="-legal/copyright,"
  # This is probably a good thing to have but there is no way to only allow
  # `using namespace std` in `.cpp` files. So we are allowing it everywhere
  # hoping that iwyu makes it annoying to use it in header files.
  # An argument could be made that this should be uncommented.
  filters+="-build/namespaces,"
  # Something like this is useful but the error message should say
  # "issue number" not "username". There is no clear method for how that could
  # be changed  use it right now
  filters+="-readability/todo,"
  # We do not have the same library usage restrictions in C++11 as Google so ignore them
  filters+="-build/c++11,"
  # Include subdir might make sense in google's blaze build system but not in ours
  filters+="-build/include_subdir,"

echo "cpplint"
echo "-------"
cpplint --quiet --recursive --exclude="src/rapidxml-1.13" --exclude="src/miniz-3.0.2" --filter=${filters} src/
if (( $? > 0 )); then
    error_count=`expr $error_count + 1`
fi


# IWYU is too noisy right now and not always completely correct. It is going
# to be disabled for now but later will be re-enabled once it is wrangled better
# # Run Include What You Use
# #
# # TODO: When this or newer versions of iwyu_tool that carry over the exit codes
# # from the include-what-you-use command calls are more widely standard this can
# # be replaced with just a call to iwyu_tool instead.
# echo "Include What You Use"
# echo "--------------------"
# ../third_party/iwyu_tool.py -p . -o quiet
# # include-what-you-use has a "success code" of 2 for a legacy reason.
# if [[ $? -ne 2 ]]; then
#     error_count=`expr $error_count + 1`
# fi


# Validate that clang-format would make no changes
echo "Clang Format"
echo "------------"
readarray -d '' FILES < <(find src/ -type f \( -name "*.cpp" -o -name "*.hpp" \) -not -path "*/rapidxml-1.13/*" -not -path "*/miniz-3.0.2/*" -print0)
clang-format -Werror --dry-run -i "${FILES[@]}"
if (( $? > 0 )); then
    error_count=`expr $error_count + 1`
fi


# Run the python presubmit for the "generators" subdirectory.
pushd generators
./presubmit.sh
if (( $? > 0 )); then
    error_count=`expr $error_count + 1`
fi
popd

# Run the python presubmit for the "integration_tests" subdirectory.
pushd integration_tests
./presubmit.sh
if (( $? > 0 )); then
    error_count=`expr $error_count + 1`
fi
popd


exit $error_count
