source ./venv/bin/activate

readarray -d '' FILES < <(find . -type f -name "*.py" -not -path "*/venv/*"  -print0)

# Lint Python Files
flake8 --ignore=E501,E266,W503 "${FILES[@]}"

# Type Check Python Files
mypy --strict "${FILES[@]}"