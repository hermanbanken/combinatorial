# How to plot

Install gnuplot
Run the following:

    ./tobinary ../data/matrix\ 2.txt matrix2.bin
    gnuplot> set term png     
    gnuplot> set xlabel "x"
    gnuplot> set ylabel "y"
    gnuplot> set zlabel "z"
    gnuplot> set output "printme.png"
    gnuplot> splot 'matrix2.bin' binary format='%float32%float32%float32%int16%int8%int8' using 1:2:3:3 with dots palette