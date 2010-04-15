load '../../gnuplot/_anim'
load '../../gnuplot/_styles'

set term gif enhanced x000000 xffffff x333333 xffff00 xff9f00 xff003f x9400ff x3333ff xff00ff x00ffea x3fff50 animate optimize
set size ratio -1 # scale 1:1

set nokey
set nomouse
set macros

period = 11.8614510204082
w = -2 * pi / period

set xrange [-0.2:6]
set yrange [-4:0.2]

t = 0
T = 10000 # Should match numFrames
dt = 10
pauseTime = 0

# 'every 1::t::t' plots only the 't'th point.
# 'every 10::0::t' plots every 10th point from 0 to the 't'th point.

jrotx = "($2 * cos(w * $1) + $3 * sin(w * $1))"
jroty = "(-$2 * sin(w * $1) + $3 * cos(w * $1))"
trotx = "($4 * cos(w * $1) + $5 * sin(w * $1)) - @jrotx"
troty = "(-$4 * sin(w * $1) + $5 * cos(w * $1)) - @jroty"

tpoint = "every 1::t::t u @trotx:@troty w p"
ttrajectory = "every 1::(t > 1000 ? t - 1000 : 0)::t u @trotx:@troty w l"

set xlabel "r_x / a.u."
set ylabel "r_y / a.u."

set output "angular-displacement.gif"

set title "t: ".t

plot "<echo 0 0" u 1:2 ls 2, \
     'jup_troj_positions' @tpoint ls 3, \
     'jup_troj_positions' @ttrajectory ls 3

load '../../gnuplot/_timestep'
