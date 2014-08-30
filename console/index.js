var express = require('express')
var http = require('http');
var app = express();
var server = http.createServer(app);
var io = require('socket.io').listen(server);
var Firmata = require('firmata');
var serialport = require("serialport");
var SerialPort = serialport.SerialPort

app.use(express.static(__dirname + "/public"));
app.set('view engine', 'jade');
app.set('views', __dirname + '/views');

app.get('/', function(req, res){
    res.render('index', {
        title: "Hello"
    });
});

server.listen(3000, function() {
    console.log('Listening on port %d', server.address().port);
});

var port = "COM4";

/*var board = new Firmata.Board(port, function (err) {
  if (err) {
    console.log(err);
    return;
  }
  console.log("Connected to 3RD Radio on " + port);
  console.log('Firmware: ' + board.firmware.name + '-' + board.firmware.version.major + '.' + board.firmware.version.minor);
});*/

var sp = new SerialPort(port, {
	baudrate: 57600,
    parser: serialport.parsers.readline("\r\n")
});

var jsobj = {};
sp.on('open', function(){
    sp.on('data', function(data){
        var dataParsed = data.split(":");
        if (dataParsed[0] === "devider") {
            io.emit('dataUpdated', jsobj);
            jsobj = {};
        } else {
            jsobj[dataParsed[0]] = dataParsed.splice(1, dataParsed.length);
        }
    });
});
