set terminal png enhanced

# DISTANCE BETWEEN TROJAN/JUPITER

set output 'png/jup_troj_distance.png'

set xlabel "time / years"
set ylabel "distance from Trojan to Jupiter / a.u."

set nokey

plot 'jup_troj_positions' u 1:(sqrt(($2 - $4)**2 + ($3 - $5)**2)) w l

# THETA FOR TROJAN/JUPITER

set key

set output 'png/jup_troj_xpositions.png'

set xlabel "time / years"
set ylabel "r_x / a.u."

set xrange [0:60]
set yrange [-8:8]

plot 'jup_troj_positions' u 1:($2) w l t "Jupiter", \
     'jup_troj_positions' u 1:($4) w l t "Trojans"