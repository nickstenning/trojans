#!/bin/bash
if [[ -n "${1}" && -n "${2}" ]]; then 
  exec gnuplot -e "d='`pwd`'" -e "gif = 0" -e "cd '${1}'" -e "load d.'/gnuplot/${2}'"
else
  echo "Usage: ./plot.sh <datadir> <plottype>!"
  exit 1
fi
