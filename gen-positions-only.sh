#!/bin/zsh

if [[ -n "${1}" ]]; then 
  pushd "${1}"
  join <(join jupiter trojans | grep -v '^#' | cut -d' ' -f 1,2,3,9,10) greeks | cut -d' ' -f1-7 > positions-only 2> /dev/null
  popd
else
  echo "Usage: ./gen-positions-only.sh <datadir>!"
  exit 1
fi
