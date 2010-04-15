set terminal png enhanced

# ENERGY OVER 400 YEARS

set output 'png/energy_400y.png'

initialEnergy = `cat system | grep -v '^#' | cut -f2 | head -1`

set nokey
set xlabel "time / years"
set ylabel "E_{system} / initial E_{system}"

set xrange [0:400]

plot 'system' u 1:($2 / initialEnergy) w l

# DISTANCE BETWEEN TROJAN/JUPITER

set output 'png/jup_troj_distance.png'

set xlabel "time / years"
set ylabel "distance from Trojan to Jupiter / a.u."

set xrange [0:400]

plot 'jup_ast_positions' u 1:(sqrt(($2 - $4)**2 + ($3 - $5)**2)) w l

# THETA FOR TROJAN/JUPITER

set key

set output 'png/jup_ast_xpositions.png'

set xlabel "time / years"
set ylabel "r_x / a.u."

set xrange [0:60]
set yrange [-8:8]

plot 'jup_ast_positions' u 1:($2) w l t "Jupiter", \
     'jup_ast_positions' u 1:($4) w l t "Trojans", \
     'jup_ast_positions' u 1:($6) w l t "Greeks"