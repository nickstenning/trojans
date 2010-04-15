#!/usr/bin/env ruby

if ARGV.length == 1
  at_time = ARGV.shift
end

float_re = /[-+]?[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?/

period = nil
crossings = []
greater_than_zarro = nil

def calc_period(times)
  sum = 0
  times.each_cons(2) { |a| sum += (a[1] - a[0]) }
  if sum > 0
    sum / (times.length - 1)
  end
end

STDIN.readlines.each do |line|
  if line =~ /^(#{float_re})\s+(#{float_re})$/
    t, x = $1.to_f, $2.to_f

    if x > 0 && greater_than_zarro == false
      crossings << t
        
      greater_than_zarro = true
    elsif x < 0
      greater_than_zarro = false
    end
    
  end
end

if at_time
  i = crossings.find_index { |x| at_time.to_f < x }
  i ||= crossings.length - 2
  puts crossings[i+1] - crossings[i]
else
  puts calc_period(crossings)
end
