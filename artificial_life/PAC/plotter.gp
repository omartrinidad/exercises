# set terminal jpeg
# set output 'plot.jpg'
set grid
set title "Predator-Prey Model"
set xrange [0:200]
set xlabel "Steps"
set ylabel "Population"
plot 'results.csv' u 1:2 w lp t 'prey',\
     'results.csv' u 1:3 w lp t 'predator'
pause -1
