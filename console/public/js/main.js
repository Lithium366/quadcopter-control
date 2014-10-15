var socket = io();

socket.on('dataUpdated', function(msg){
    $(window).trigger("dataUpdated", msg);
});

var quadcopter = angular.module('quadcopter', []);

$(function () {

    // If no new data in 200ms - set online indicator to false
    var online = false;
    function setOffline() {
        if (!online) {
            $(window).trigger("onlineUpdated", false);
        }
        online = false;
        window.setTimeout(setOffline, 200);
    }
    setOffline();

    // Once serial data available, notify observers
    $(window).on("dataUpdated", function (e, val) {
        if (val.reciever) {
            updateRCcontrols(val.reciever);
        }
        if (val.pid) {
            $(window).trigger("pidUpdated", {values: val.pid});
        }
        if (val.system) {
            $(window).trigger("systemUpdated", {values: val.system});
        }
        if (val.angles) {
            $(window).trigger("anglesUpdated", {values: val.angles});
        }
        // Set online indicator to true every time new data came
        online = true;
        $(window).trigger("onlineUpdated", true);
    });

    var updateRCcontrols = function (values) {
		var chart = $('#container-throttle').highcharts();
		if (chart && values[0]) {
			chart.series[0].points[0].update(parseInt(values[0]));
		}

    	chart = $('#container-yaw').highcharts();
		if (chart && values[3]) {
			chart.series[0].points[0].update(parseInt(values[3]))
		}
    		
    	chart = $('#container-pitch').highcharts();
		if (chart && values[1]) {
			chart.series[0].points[0].update(parseInt(values[1]));
		}

    	chart = $('#container-roll').highcharts();
		if (chart && values[2]) {
			chart.series[0].points[0].update(parseInt(values[2]));
		}
    }

});


