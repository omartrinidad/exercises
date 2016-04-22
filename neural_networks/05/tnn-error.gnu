#!/usr/bin/gnuplot

reset

#set terminal wxt size 350,262 enhanced font 'Verdana,10' persist
set terminal latex
set output "learning.tex"

# Line styles
set border linewidth 1.5
set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2  # blue
set style line 2 linecolor rgb '#dd181f' linetype 1 linewidth 2  # red

# Legend
#set key at 6.1, 1.3
set nokey

# Axes label 
set xlabel 'time t, or number of patterns'
set ylabel 'E(t)'

# axis ranges
set xrange[0:3500]
set yrange[0.6:1.6]

set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 0 ps 1.5  # blue
plot 'points.dat' with linespoints ls 1
