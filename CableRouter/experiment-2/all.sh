d=`date`
echo "Start @ $d";
for e in {0..4}
do
	echo "Experiment $e";
    echo "Run";
	cat "column-$e.txt" | ../CableRouter > "ex-$e.out";
done
d=`date`
echo "Done @ $d";
