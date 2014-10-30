#!/bin/sh
':' //; exec "$(command -v nodejs || command -v node)" "$0" "$@"

// echo "../data/matrix-1824.txt\nd 5 r g 6 5 2\n2 2 613 570" | CableRouter

var fs = require('fs');

for(var a = 0; a < 6; a++){
	var file = fs.readFileSync("result-"+a+".txt"),
		data = JSON.parse(file);
	
	var cols = data.map(function(it,i){
		return [
			i*64,
			it[3].median.toPrecision(5),
			it[3].first[4].toPrecision(5),
			it[3].last[0].toPrecision(5),
			it[0].toPrecision(5),
		].join("\t");
	}).join("\n");

	fs.writeFile('result-m-l-h-'+a+'.txt', cols);
}

/* 
gnuplot:

reset
set termoption dash
set xrange [*:23e3]
set logscale y
unset logscale x
set yrange [3000:12e6]
set xlabel "Iterations [n]"
set ylabel "Best fitness found [f]"
set style fill transparent solid 0.2 noborder

set style line 1 lt 1 lw 1 linecolor rgb "orange"
set style line 2 lt 1 lw 1 linecolor rgb "red" 
set style line 3 lt 1 lw 1 linecolor rgb "yellow"
set style line 4 lt 1 lw 1 linecolor rgb "green"
set style line 5 lt 1 lw 1 linecolor rgb "purple"
set style line 6 lt 1 lw 1 linecolor rgb "blue"
set key Left

set term pdf color enhanced font "Times,9"
set output "combined-2.pdf"

plot 'result-m-l-h-0.txt' using 1:3:4 with filledcurves x1 lc rgb "orange" notitle,\
     'result-m-l-h-3.txt' using 1:3:4 with filledcurves x1 lc rgb "red" notitle,\
     'result-m-l-h-1.txt' using 1:3:4 with filledcurves x1 lc rgb "yellow" notitle,\
     'result-m-l-h-4.txt' using 1:3:4 with filledcurves x1 lc rgb "green" notitle,\
     'result-m-l-h-2.txt' using 1:3:4 with filledcurves x1 lc rgb "purple" notitle,\
     'result-m-l-h-5.txt' using 1:3:4 with filledcurves x1 lc rgb "blue" notitle,\
     'result-m-l-h-0.txt' using 1:3 with lines ls 1 notitle,\
     'result-m-l-h-3.txt' using 1:3 with lines ls 2 notitle,\
     'result-m-l-h-1.txt' using 1:3 with lines ls 3 notitle,\
     'result-m-l-h-4.txt' using 1:3 with lines ls 4 notitle,\
     'result-m-l-h-2.txt' using 1:3 with lines ls 5 notitle,\
     'result-m-l-h-5.txt' using 1:3 with lines ls 6 notitle,\
     'result-m-l-h-0.txt' using 1:4 with lines ls 1 notitle,\
     'result-m-l-h-3.txt' using 1:4 with lines ls 2 notitle,\
     'result-m-l-h-1.txt' using 1:4 with lines ls 3 notitle,\
     'result-m-l-h-4.txt' using 1:4 with lines ls 4 notitle,\
     'result-m-l-h-2.txt' using 1:4 with lines ls 5 notitle,\
     'result-m-l-h-5.txt' using 1:4 with lines ls 6 notitle,\
     'result-m-l-h-0.txt' using 1:2 with lines ls 1 lw 3 title 'CMA-ES',\
     'result-m-l-h-3.txt' using 1:2 with lines ls 2 lw 3 title 'active CMA-ES',\
     'result-m-l-h-1.txt' using 1:2 with lines ls 3 lw 3 title 'IPOP-CMA-ES',\
     'result-m-l-h-4.txt' using 1:2 with lines ls 4 lw 3 title 'active IPOP-CMA-ES',\
     'result-m-l-h-2.txt' using 1:2 with lines ls 5 lw 3 title 'BIPOP-CMA-ES',\
     'result-m-l-h-5.txt' using 1:2 with lines ls 6 lw 3 title 'active BIPOP-CMA-ES'
set output

     'result-m-l-h-0.txt' using 1:5 with lines lc 1 lt 1 lw 1 dt 1 notitle,\
     'result-m-l-h-3.txt' using 1:5 with lines lc 4 lt 1 lw 1 dt 1 notitle,\
     'result-m-l-h-1.txt' using 1:5 with lines lc 2 lt 1 lw 1 dt 1 notitle,\
     'result-m-l-h-4.txt' using 1:5 with lines lc 5 lt 1 lw 1 dt 1 notitle,\
     'result-m-l-h-2.txt' using 1:5 with lines lc 3 lt 1 lw 1 dt 1 notitle,\
     'result-m-l-h-5.txt' using 1:5 with lines lc 6 lt 1 lw 1 dt 1 notitle,\
replot
*/