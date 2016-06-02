name=system("echo mprtp-subflow-") 
time=system("date +%Y_%m_%d_%H_%M_%S")

if (!exists("plot_title")) plot_title='Subflow Receiver Rate Report'
if (!exists("throughput_file")) throughput_file='logs/snd_1_ratestat.csv'
if (!exists("bw_file")) bw_file='logs/veth0.csv'
if (!exists("output_file")) output_file='reports/snd-throughputs.pdf'
if (!exists("duration")) duration=100
if (!exists("range")) range=6000
if (!exists("tcpstat")) tcpstat='logs/tcpstat10.csv'

#-------------------------------------------------------------------------

set terminal pdf enhanced rounded size 10,6
set output output_file
set key font ",28"
set xtics font ",28"
set ytics font ",28"
set ylabel font ",28"
set xlabel font ",28"

set origin 0,0
set size ratio 0.2
set datafile separator "," 

set multiplot layout 3,1

set key inside horizontal top right
unset key  
set tmargin 1
set bmargin 2
#set lmargin 23
set lmargin 10
set rmargin 3
set yrange [0:3500]
set ytics 1000
#set ylabel "bitrate (KBits)" offset -8
set grid ytics lt 0 lw 1 lc rgb "#bbbbbb"
set grid xtics lt 0 lw 1 lc rgb "#bbbbbb"

# Line width of the axes
# Line styles
#colors:
# magenta: #ee2e2f
# green:   #008c48
# blue:    #185aa9
# orange:  #f47d23
# purple:  #662c91
# claret:  #a21d21
# lpurple: #b43894

set style line 1 linecolor rgb '#008c48' linetype 1 linewidth 1
set style line 2 linecolor rgb '#b43894' linetype 2 linewidth 1
set style line 3 linecolor rgb '#185aa9' linetype 3 linewidth 1
set style line 4 linecolor rgb '#a21d21' linetype 4 linewidth 1	
set style line 5 linecolor rgb '#662c91' linetype 5 linewidth 1	

unset xtics 
set y2label "Path 1" offset 2
 plot throughput_file using ($0 * 0.1):($3+$5) with lines ls 1 title "Path 1 SR + FEC", \
      throughput_file using ($0 * 0.1):11 with lines ls 4 title "Path 1 Capacity"

set y2label "Path 2" offset 2      
 plot throughput_file using ($0 * 0.1):($8+$10) with lines ls 1 title "Path 2 SR + FEC", \
      throughput_file using ($0 * 0.1):12 with lines ls 4 title "Path 2 Capacity"
      
      
set yrange [0:6000]
set ytics 2000
set xrange [0:duration]
#set xtics 50 offset 0,-1
set xtics 40
set xlabel "time (s)" offset 0,-2
#set bmargin 3

set y2label "Aggregated" offset 2
  plot throughput_file using ($0 * 0.1):($3+$5+$8+$10) with lines ls 1 title "Path 2 SR + FEC", \
       throughput_file using ($0 * 0.1):($11 + $12) with lines ls 4 title "Path 2 Capacity"
      

#plot throughput_file using 0:($4+$6) with lines ls 1 title "Sending Rate + FEC", \
#     throughput_file using 0:2 with lines ls 2 title "Target Rate", \
#     throughput_file using 0:6 with lines ls 3 title "FEC Rate", \
#     throughput_file using 0:7 with lines ls 4 title "Path Capacity"
     
unset multiplot
