var express = require('express');
var app = express();

app.use(express.static(__dirname + "/public"));
app.set('view engine', 'jade');
app.set('views', __dirname + '/views');

app.get('/serial', function(req, res){
    res.send(serialString);
});

app.get('/', function(req, res){
    res.render('index', {
        title: "Hello"
    });
});

var server = app.listen(3000, function() {
    console.log('Listening on port %d', server.address().port);
});

//var SerialPort = require("serialport").SerialPort;
//var serialport = new SerialPort("COM6");
var serialString = "";
/*serialport.on('open', function(){
    serialport.on('data', function(data){
        serialString = data[0];
    });
});*/
