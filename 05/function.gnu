#!/usr/bin/gnuplot

reset

# wxt
set terminal wxt size 350,262 enhanced font 'Verdana,10' persist
#set terminal latex
#set output "outputxy.tex"

# line styles
set border linewidth 1.5
set style line 1 linecolor rgb '#0060ad' linetype 1 linewidth 2  # blue
set style line 2 linecolor rgb '#dd181f' linetype 1 linewidth 2  # red

# axes label 
set xlabel 'x'
set ylabel 'y'

# axis ranges
set xrange[0.01:*]
#set yrange[0.01:2]

#set logscale y
set logscale x 
#set logscale xy

# functions to plot
f(x) = 2**( 2 - x + 4 * sin(x) )

# plot
plot f(x) title '2**( 2 - x + 4 * sin(x) )' with lines ls 1

