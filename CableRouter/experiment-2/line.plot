#!/usr/bin/gnuplot -p

reset
set termoption dash
set term pdf color enhanced font "Times,9"
set style fill transparent solid 0.2 noborder
unset border
unset xtics
unset ytics
set key off
unset colorbox

set output outfile

plot matrix binary format='%float32%float32%float32%int16%int8%int8' using 1:2:3 with dots palette, line using 1:2:(4) with lines palette


# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; gnuplot -e "matrix='matrix-clear.bin'; line='ex-problem-0-$alg-n-$n-run-1.line'; outfile='line-0-$alg-n-$n.pdf'" line.plot; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; gnuplot -e "matrix='matrix-one.bin'; line='ex-problem-1-$alg-n-$n-run-1.line'; outfile='line-1-$alg-n-$n.pdf'" line.plot; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; gnuplot -e "matrix='matrix-two.bin'; line='ex-problem-2-$alg-n-$n-run-1.line'; outfile='line-2-$alg-n-$n.pdf'" line.plot; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; gnuplot -e "matrix='matrix-three.bin'; line='ex-problem-3-$alg-n-$n-run-1.line'; outfile='line-3-$alg-n-$n.pdf'" line.plot; done; done
# for n in {400,800,1600,3200}; do; for alg in {s,t,a,d}; do; gnuplot -e "matrix='matrix-complex.bin'; line='ex-problem-4-$alg-n-$n-run-1.line'; outfile='line-4-$alg-n-$n.pdf'" line.plot; done; done

