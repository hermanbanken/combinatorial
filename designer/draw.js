var canvas = $("canvas").get(0);
var c = canvas.getContext("2d");
var drawing = false;
var mouse = {x: 0, y: 0 };
var cursor = $("#cursor");

document.addEventListener('mousemove', function(e){
    mouse.x = e.clientX || e.pageX;
    mouse.y = e.clientY || e.pageY;
    window.requestAnimFrame(cycle);
}, false);

$("canvas, #cursor").on("mousedown", function(e){
    drawing = true;
    mouse.x = e.clientX || e.pageX;
    mouse.y = e.clientY || e.pageY;
    window.requestAnimFrame(cycle);
});

$("canvas, #cursor").on("mouseup blur", function(){ drawing = false; });
$(window).on("mouseout", function(e){
    if (e.toElement == null && e.relatedTarget == null) {
        drawing = false
    }
});

function cycle(){
    cursor.css({
        left: mouse.x,
        top: mouse.y
    });
    if(drawing){
        drawCursor(mouse.x,mouse.y);
    }
}

window.requestAnimFrame = (function(){
    return  window.requestAnimationFrame   ||
        window.webkitRequestAnimationFrame ||
        window.mozRequestAnimationFrame    ||
        function( callback ){
            window.setTimeout(callback, 1000 / 60);
        };
})();

$(window).on("resize",function(){
    var imgData=c.getImageData(0, 0, canvas.width, canvas.height);
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;
    c.putImageData(imgData,0,0);
    $(".dim").text(window.innerWidth + "x" + window.innerHeight);
}).trigger("resize");

$(".save").on("click", function(e){
    var w = canvas.width,
        h = canvas.height,
        data = c.getImageData(0, 0, w, h).data,
        lines = [],
        blob;

    var ct = 0;
    for(var x = 0, i = 0, n = data.length; i < n; i += 4, x++, ct++) {
        var soil = "1",
            obs = "0";

        // Alpha, not mapped
        if(data[i+3] === 0) continue;

        // Bomb, red
        if(data[i] === 255 && data[i+1] === 0 && data[i+2] === 0)
            obs = "2";
        // Builder, green
        if(data[i] === 0 && data[i+1] === 255 && data[i+2] === 0)
            obs = "1";
        // Pipe, blue
        if(data[i] === 0 && data[i+1] === 0 && data[i+2] === 255)
            obs = "3";

        var line = (x % w) + " " + ~~(x / h) + " -5.000 -1.000 " + soil + ".000 " + obs + ".000\n";
        lines.push(line);
    }
    console.log(ct, "data points", "of", data.length);

    blob = new Blob(lines, {type: 'text/plain'});
    saveAs(blob, "matrix.txt");

    canvas.toBlob(function(blob) {
        saveAs(blob, "matrix.png");
    }, "image/png");
});

var styles = {
    blue: {
        width: 4,
        height: 4,
        borderRadius: 2,
        background: "blue",
        marginLeft: -2,
        marginTop: -2
    },
    red: {
        width: 50,
        height: 50,
        borderRadius: 25,
        background: "red",
        marginLeft: -25,
        marginTop: -25
    },
    green: {
        width: 50,
        height: 50,
        borderRadius: 25,
        background: "green",
        marginLeft: -25,
        marginTop: -25
    },
    white: {
        width: 100,
        height: 100,
        borderRadius: 50,
        background: "white",
        marginLeft: -50,
        marginTop: -50
    }
}, currentStyle = "white";

$("#bar button").on("click", function(e){
    currentStyle = styles[this.id] || styles["white"];
    cursor.css(currentStyle);
    e.preventDefault();
    return false;
});
$("#white").trigger("click");

function drawCursor(x,y){
    c.beginPath();
    c.arc(x, y, currentStyle.borderRadius, 0, 2 * Math.PI, false);
    c.fillStyle = currentStyle.background;
    c.fill();
}