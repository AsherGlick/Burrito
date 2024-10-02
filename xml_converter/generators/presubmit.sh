#!/bin/bash

error_count=0

source ../venv/bin/activate

readarray -d '' FILES < <(find . -type f -name "*.py" -not -path "*/venv/*"  -print0)

# Lint Python Files
flake8 --ignore=E501,E266,W503 "${FILES[@]}"
if (( $? > 0 )); then
	echo "Flake8 Error"
    error_count=`expr $error_count + 1`
fi

# Type Check Python Files
mypy --strict "${FILES[@]}"
if (( $? > 0 )); then
	echo "mypy error"
    error_count=`expr $error_count + 1`
fi


exit $error_count
