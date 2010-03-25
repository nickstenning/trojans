#!/bin/bash
if [[ -n "${1}" ]]; then 
  exec gnuplot -e "d='${1}'" gnuplot/movie
else
  echo "Specify a data directory!"
  exit 1
fi
