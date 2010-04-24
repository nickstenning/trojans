#!/bin/zsh

if [[ -n "${1}" ]]; then 
  pushd "${1}"
  join <(join jupiter trojans | cut -d' ' -f 1,2,3,9,10) greeks | cut -d' ' -f1-7 > positions-only-tmp
  join system positions-only-tmp > positions-only
  rm positions-only-tmp
  popd
else
  echo "Usage: ./gen-positions-only.sh <datadir>!"
  exit 1
fi
