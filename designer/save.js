window.requestFileSystem = window.requestFileSystem || window.webkitRequestFileSystem;

window.requestFileSystem(window.PERSISTENT, 1024*1024, function(fs) {
    fs.root.getFile('test.bin', {create: true}, function(fileEntry) { // test.bin is filename
        fileEntry.createWriter(function(fileWriter) {
            var arr = new Uint8Array(3); // data length

            arr[0] = 97; // byte data; these are codes for 'abc'
            arr[1] = 98;
            arr[2] = 99;

            var blob = new Blob([arr]);

            fileWriter.addEventListener("writeend", function() {
                // navigate to file, will download
                location.href = fileEntry.toURL();
            }, false);

            fileWriter.write(blob);
        }, function() {});
    }, function() {});
}, function() {});

window.webkitRequestFileSystem(window.PERSISTENT , 1024*1024, SaveDatFileBro);