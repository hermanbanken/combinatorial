d=`date`
echo "Start @ $d";
for e in {0..79}
do
	echo "Experiment $e";
    echo "Run";
	cat "ex-$e.txt" | ../CableRouter > "ex-$e.out";
done
d=`date`
echo "Done @ $d";
