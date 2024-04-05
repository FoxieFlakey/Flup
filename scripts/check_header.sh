#!/usr/bin/env sh

if [ $# -lt 1 ]; then
  2>&1 echo "Usage: $0 <headers to check ...>"
  2>&1 echo ""
  2>&1 echo "Checks if system headers exists or not"
  2>&1 echo ""
  2>&1 echo "This script outputs 'y' or 'n' depends on"
  2>&1 echo "the result so if any header not found it"
  2>&1 echo "outputs 'n', else if all found outputs 'y'"
  exit 1
fi

for header in "$@"; do
  echo "#include <$header>" | cc -fsyntax-only -xc - 2>&1 | grep -v ''
  if [ $? -ne 0 ]; then
    echo 'n'
    exit 0
  fi
done

echo 'y'
exit 0
