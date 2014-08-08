/* Msin JS */
var socket = io();

socket.on('dataUpdated', function(msg){
    console.log(msg);
});
