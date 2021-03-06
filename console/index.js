var express = require('express')
var http = require('http');
var app = express();
var server = http.createServer(app);
var io = require('socket.io').listen(server);
var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var fs = require('fs');

app.use(express.static(__dirname + "/public"));
app.set('view engine', 'jade');
app.set('views', __dirname + '/views');
app.locals.pretty = true;

server.listen(3000, function() {
    console.log('Listening on port %d', server.address().port);
});


/*** SERIAL PORT ***/
var port = "/dev/tty.SLAB_USBtoUART";

var sp = new SerialPort(port, {
    baudrate: 57600,
    parser: serialport.parsers.readline("\r\n")
});

sp.on('open', function () {
    var tmot = null;
    var jsobj = {};
    var nowDate = new Date();
    var changeMode = true;

    /*var logger = fs.createWriteStream('public/logs/log_' + ((nowDate.getMonth() < 9) ? '0' : '') + (nowDate.getMonth() + 1) +
        '-' + ((nowDate.getDate() >= 10) ? '' : '0') + nowDate.getDate() +
        '-' + nowDate.getFullYear() +
        '-' + ((nowDate.getHours() >= 10) ? '' : '0') + nowDate.getHours() +
        '-' + ((nowDate.getMinutes() >= 10) ? '' : '0') + nowDate.getMinutes() +
        '-' + ((nowDate.getSeconds() >= 10) ? '' : '0') + nowDate.getSeconds() +
        '.txt', {'flags': 'a'});*/

    sp.on('data', function (data) {
        //logger.write(data + "\r\n");

        if (data === 'pidsaved') {
          changeMode = true;
        }
        var dataParsed = data.split(":");
        if (dataParsed[0] === "devider") {
            io.emit('dataUpdated', jsobj);
            jsobj = {};
        } else {
            jsobj[dataParsed[0]] = dataParsed.splice(1, dataParsed.length);
        }
    });

    app.get('/', function(req, res){
        choseMode("f");
        res.render('index');
    });

    app.get('/logs', function(req, res){
        res.render('logs');
    });
    app.get('/pid', function(req, res){
        choseMode("b");
        res.render('pid');
    });
    app.get('/engines', function(req, res){
        choseMode("e");
        res.render('engines');
    });
    app.get('/accelerometer', function(req, res){
        choseMode("v");
        res.render('vybro');
    });

    function choseMode(mode) {
        clearTimeout(tmot);
        tmot = setTimeout(function () {
            sp.write(mode);
            choseMode(mode);
        }, 1000);
    }

    /*** Socket.io listeners ***/
    io.on('connection', function (socket) {

        socket.on('getFile', function () {
            var files = fs.readdirSync('public/logs');
            var data = fs.readFileSync('public/logs/' + files[files.length - 1], 'utf8');
            io.emit('sendFile', data);
        });

        socket.on('armDisarm', function () {
            sp.write("a");
        });

        socket.on('setLevel', function () {
            sp.write("l");
        });

        socket.on('setPid', function (val) {
            changeMode = false;
            savePid(val);
        });

        socket.on('weatherObtained', function (val) {
            sp.write("w" + val);
        });

        socket.on('setPidMode', function (val) {
            if (!val) {
                choseMode("b");
            } else {
                choseMode(val);
            }
        });

        socket.on('getPid', function (val) {
            choseMode("t");
            setTimeout(function () {
                if (!val) {
                    choseMode("b");
                } else {
                    choseMode(val);
                }
            }, 1500);
        });

        function savePid (val) {
            if (!changeMode) {
                setTimeout(function () {
                    sp.write("s" + val);
                    savePid(val);
                }, 200);
            } else {
                io.emit('pidSaved');
            }
        }

    });

});



