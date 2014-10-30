#!/bin/sh
':' //; exec "$(command -v nodejs || command -v node)" "$0" "$@"

// echo "../data/matrix-1824.txt\nd 5 r g 6 5 2\n2 2 613 570" | CableRouter

var exec = require('child_process').spawn,
	fs = require('fs');

var n_runs = 100,
	file = "../experiment-1-ga-compare/matrix-complex.txt",
	windmills = "10 400 1052 400",
	z_value_975percentile = 1.96;

var algos = [
	'cma-es',
	'ipop-cma-es',
	'bipop-cma-es',
	'active cma-es',
	'active ipop-cma-es',
	'active bipop-cma-es'
];

var f_values = algos.map(function(){ return []; });
var results = algos.map(function(){ return []; });

var start = algos.reduceRight(function(memo, name, a){
	return (function(whenDone){
		process.stdout.write("Starting computation for "+name+"\n");
		var runs = [], 
			child = exec('../CableRouter');
		
		// On finish
		child.on('close', function(){
			process.stdout.write("\n");

			var maxIterations = runs.reduce(function(m,c){ return Math.max(m, c.length); }, 0);

			for(var i = 0; i < maxIterations; i++){
				f_values[a][i] = runs.map(function(r){ return r[i] || r[r.length-1]; });
			}

			writeOut(a);
			whenDone();	
		});

		// Read output
		var lastIteration = 1,
			overflow = "";

		child.stderr.on('data', (function(a, data){
			var lastPos = 0,
				text = overflow+data.toString(),
			    pattern = new RegExp(/^([\d\.e+\-]+)\s([\d\.e+\-]+)$/mg),
			    match;
			while (match = pattern.exec(text)) {
				lastPos = match.index+match[0].length;
				var it = Number(match[1])/64;
				if(it < lastIteration){
					runs.push([]);
				}
				runs[runs.length-1].push(Number(match[2]));
				lastIteration = it;
			}
			overflow = text.substring(lastPos);
		}).bind(this, a));

		// Write progress
		var received_run = 0;
		child.stdout.on('data', function(data){
			received_run += data.toString().split(/^\?/mg).length - 1;
			process.stdout.write("\r> "+name+" run "+received_run+" of "+n_runs);
		});

		// Write input
		child.stdin.write(file+"\nd 5\nr g 6 5 "+a+"\n");
		for(var run = 0; run < n_runs; run++){
			child.stdin.write(windmills+"\n");
		}
		child.stdin.end();

	}).bind(this,memo);
}, function(){
	console.log("Completely done!!!");
});

function writeOut(a){
	for(var it in f_values[a]){
		results[a][it] = mean_low_high_n(f_values[a][it]);
	}
	fs.writeFile('result-'+a+'.txt', JSON.stringify(results[a]), function (err) {
	  if (err) return console.log(err);
	  console.log('Written results to file result-'+a+'.txt');
	});
}

start();

return;

function add(algo, nodes, fitness, pretime, runtime){
	if(!data[algo])
		data[algo] = {};
	if(!data[algo][nodes])
		data[algo][nodes] = { fitness: fitness, pretime: [], runtime: [] };

	data[algo][nodes].pretime.push(pretime);
	data[algo][nodes].runtime.push(runtime);
}

function mean_low_high_n(ar){
	var m = avg(ar),
		s = dev(ar),
		error = z_value_975percentile * s / Math.sqrt(ar.length);
	ar.sort(function(a, b){return a-b});
	return [m, s, error, {
		first: ar.slice(0, 5),
		last: ar.slice(-5),
		median: ar[Math.floor(ar.length/2)]
	}, ar.length];
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