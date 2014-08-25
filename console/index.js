var express = require('express')
var http = require('http');
var app = express();
var server = http.createServer(app);
var io = require('socket.io').listen(server);

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

var SerialPort = require("serialport").SerialPort;
var serialport = new SerialPort("COM6", {
	baudrate: 57600
});

serialport.on('open', function(){
    serialport.on('data', function(data){
        io.emit('dataUpdated', data.toString());
	console.log(data.toString());
    });
});
