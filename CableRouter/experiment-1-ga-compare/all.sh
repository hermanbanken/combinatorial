d=`date`
echo "Start @ $d";
for e in {0..4}
do
	echo "Experiment $e";
	for r in {0..9}
	do
		echo "Run $r";
		cat "column-$e.txt" | ../CableRouter > "ex-$e-run-$r.out";
	done
done
d=`date`
echo "Done @ $d";