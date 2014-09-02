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

app.get('/', function(req, res){
  res.render('index');
});
app.get('/logs', function(req, res){
  res.render('logs');
});

server.listen(3000, function() {
    console.log('Listening on port %d', server.address().port);
});

var port = "COM4";

var sp = new SerialPort(port, {
  baudrate: 57600,
  parser: serialport.parsers.readline("\r\n")
});

var jsobj = {};
var nowDate = new Date();
var logger = fs.createWriteStream('logs/log_' + ((nowDate.getMonth() < 9) ? '0' : '') + (nowDate.getMonth() + 1) +
    '-' + ((nowDate.getDate() >= 10) ? '' : '0') + nowDate.getDate() +
    '-' + nowDate.getFullYear() +
    '-' + ((nowDate.getHours() >= 10) ? '' : '0') + nowDate.getHours() +
    ':' + ((nowDate.getMinutes() >= 10) ? '' : '0') + nowDate.getMinutes() +
    ':' + ((nowDate.getSeconds() >= 10) ? '' : '0') + nowDate.getSeconds() +
    '.txt', {'flags': 'a'});
var loggerData = "";
var new_data_available = false;
setTimeout(function () {
  if (new_data_available) {
    logger.write(loggerData + "\r\n");
  } else {
    logger.write("no updates\r\n");
  }
  new_data_available = false;
}, 5);

sp.on('open', function () {
  sp.on('data', function (data) {
    new_data_available = true;
    loggerData = data;
    var dataParsed = data.split(":");
    if (dataParsed[0] === "devider") {
      io.emit('dataUpdated', jsobj);
      jsobj = {};
    } else {
      jsobj[dataParsed[0]] = dataParsed.splice(1, dataParsed.length);
    }
  });
});

