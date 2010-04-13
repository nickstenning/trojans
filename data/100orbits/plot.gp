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

plot 'jup_troj_positions' u 1:(sqrt(($2 - $4)**2 + ($3 - $5)**2)) w l

# DISTANCE BETWEEN TROJAN/SUN

set output 'png/jup_troj_distance.png'

set xlabel "time / years"
set ylabel "distance from Trojan to Jupiter / a.u."

set xrange [0:400]

plot 'jup_troj_positions' u 1:(sqrt(($2 - $4)**2 + ($3 - $5)**2)) w l