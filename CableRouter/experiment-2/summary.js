#!/bin/sh
':' //; exec "$(command -v nodejs || command -v node)" "$0" "$@"

// Call with:
// ./convert-out-files.js | sh
// for n in {400,800,1600,3200}; do; cat ex-problem-0-{s,t,a,d}-n-$n-run-*.out | grep "^=" | sed "s/$/ $n/"; done | ./summary.js
// for n in {400,800,1600,3200}; do; cat ex-problem-1-{s,t,a,d}-n-$n-run-*.out | grep "^=" | sed "s/$/ $n/"; done | ./summary.js
// for n in {400,800,1600,3200}; do; cat ex-problem-2-{s,t,a,d}-n-$n-run-*.out | grep "^=" | sed "s/$/ $n/"; done | ./summary.js
// for n in {400,800,1600,3200}; do; cat ex-problem-3-{s,t,a,d}-n-$n-run-*.out | grep "^=" | sed "s/$/ $n/"; done | ./summary.js
// for n in {400,800,1600,3200}; do; cat ex-problem-4-{s,t,a,d}-n-$n-run-*.out | grep "^=" | sed "s/$/ $n/"; done | ./summary.js
// For all 5 problems

process.stdin.resume();
process.stdin.setEncoding('utf8');
 
var data = {};
var buffer = [];

process.stdin.on('data', function (line) {
	Array.prototype.push.apply(buffer, line.toString().trim().split(/\s|\n/g));
	while(buffer.length > 5){
		var point = buffer.splice(0, 5);
		add(point[0].charAt(1), parseInt(point[4]), parseFloat(point[1]), parseFloat(point[2]), parseFloat(point[3]));
	}
});

function add(algo, nodes, fitness, pretime, runtime){
	if(!data[algo])
		data[algo] = {};
	if(!data[algo][nodes])
		data[algo][nodes] = { fitness: fitness, pretime: [], runtime: [] };

	data[algo][nodes].pretime.push(pretime);
	data[algo][nodes].runtime.push(runtime);
}

function avg(ar){
	return sum(ar) / ar.length;
}
function dev(ar){
	var a = avg(ar);
	var squares = ar.map(function(c){
		return (c - a)*(c - a);
	});
	return Math.sqrt(sum(squares) / (squares.length - 1));
}
function sum(ar){
	return ar.reduce(function(m, c){
		return m+c;
	}, 0);
}
function min(ar){
	return ar.reduce(function(m, c){
		return !m ? c : Math.min(m,c);
	}, false);
}

process.stdin.on('end', function() {
  console.log("algo\ta-runt.\ts-runt.\tmin-fitn.\ta-fitn.  \ts-fitn.");

  var counts = [400,800,1600,3200];
  console.log("\t"+counts.join("\t\t\t\t")+"\t\t\t\t");
  var sub = "";
  for(var i = 0; i < 4; i++)
  	sub+="\t& fitn.\t& avg pre\t& dev pre\t& avg run\t& dev run";
  console.log(sub);

  for(algo in data){
  	var line = algo;
  	for(nodes in data[algo]){
  		var ap = avg(data[algo][nodes].pretime),
  			dp = dev(data[algo][nodes].pretime),
  			ar = avg(data[algo][nodes].runtime),
  			dr = dev(data[algo][nodes].runtime);
  		line += 
  			"\t& "+data[algo][nodes].fitness.toPrecision(4) +
  			"\t& "+(ap < 1 ? ap.toExponential(3) : ap.toPrecision(4)) +
  			"\t& "+(dp < 1 ? dp.toExponential(3) : dp.toPrecision(4)) +
  			"\t& "+(ar < 1 ? ar.toExponential(3) : ar.toPrecision(4)) +
  			"\t& "+(dr < 1 ? dr.toExponential(3) : dr.toPrecision(4));
  	}
  	console.log(line + "\\\\");
  }

  // 	console.log([n, avg(data[n].runtime), dev(data[n].runtime), min(data[n].fitness), avg(data[n].fitness), dev(data[n].fitness)].map(function(n, i){
  // 		return i == 0 ? n : n.toFixed(3);
  // 	}).join("\t"));
  // }
  process.exit();
});