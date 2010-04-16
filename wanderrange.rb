#!/usr/bin/env ruby

lines = STDIN.readlines.map { |x| x.split.map { |y| y.to_f }}

jup_to_troj_distance = lambda { |x| Math.sqrt((x[1] - x[3])**2 + (x[2] - x[4])**2) }

min, max = lines.minmax_by &jup_to_troj_distance

p jup_to_troj_distance.call(max) - jup_to_troj_distance.call(min)