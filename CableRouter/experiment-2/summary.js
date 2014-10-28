#!/bin/sh
':' //; exec "$(command -v nodejs || command -v node)" "$0" "$@"

// Call with:
// cat ex-0-run-* | grep "=g" | ./summary.js
// cat ex-1-run-* | grep "=g" | ./summary.js
// cat ex-2-run-* | grep "=g" | ./summary.js
// cat ex-3-run-* | grep "=g" | ./summary.js
// cat ex-4-run-* | grep "=g" | ./summary.js
// For all 5 columns

process.stdin.resume();
process.stdin.setEncoding('utf8');
 
var data = {};
var buffer = [];

process.stdin.on('data', function (line) {
	Array.prototype.push.apply(buffer, line.toString().trim().split(/\s|\n/g));
	while(buffer.length > 6){
		var point = buffer.splice(0, 7);
		data[point[4]] = data[point[4]] || { runtime: [], fitness: [] };
		data[point[4]].runtime.push(parseFloat(point[3]));
		data[point[4]].fitness.push(parseFloat(point[1]));
	}
});

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
  for(n in data){
  	console.log([n, avg(data[n].runtime), dev(data[n].runtime), min(data[n].fitness), avg(data[n].fitness), dev(data[n].fitness)].map(function(n, i){
  		return i == 0 ? n : n.toFixed(3);
  	}).join("\t"));
  }
  process.exit();
});