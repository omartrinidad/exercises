set terminal jpeg
set output 'plot.jpg'
set grid
set title "Predator-Prey Model"
set xlabel "Steps"
set ylabel "Population"

#set arrow from 100,0.2 to 100,2.2 nohead lc blue
#set arrow from 100,0.2 to 100,2.2 nohead lc blue

plot 'results.csv' u 1:2 with lines t 'prey',\
     'results.csv' u 1:3 with lines t 'predator'

pause -1
