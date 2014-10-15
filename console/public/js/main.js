var socket = io();

socket.on('dataUpdated', function(msg){
    $(window).trigger("dataUpdated", msg);
});

var quadcopter = angular.module('quadcopter', []);