#!/bin/sh
':' //; exec "$(command -v nodejs || command -v node)" "$0" "$@"

var algs = ["s","t","a","d"];

for(var i = 0; i < 80; i+=4){
	var p = Math.floor(i/4) % 5;
	var a = algs[Math.floor(i/20)];
	for(var r = 0; r < 5; r++){
		console.log("mv ex-"+(i+0)+"-run-"+r+".out ex-problem-"+p+"-"+a+"-n-400-run-"+r+".out");
		console.log("mv ex-"+(i+1)+"-run-"+r+".out ex-problem-"+p+"-"+a+"-n-800-run-"+r+".out");
		console.log("mv ex-"+(i+2)+"-run-"+r+".out ex-problem-"+p+"-"+a+"-n-1600-run-"+r+".out");
		console.log("mv ex-"+(i+3)+"-run-"+r+".out ex-problem-"+p+"-"+a+"-n-3200-run-"+r+".out");
	}
}
