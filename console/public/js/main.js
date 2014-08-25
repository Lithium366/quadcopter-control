var socket = io();

socket.on('dataUpdated', function(msg){
    $(window).trigger("dataUpdated", msg);
});

$(function () {

    $(window).on("dataUpdated", function (e, val) {
        var separated = val.split(":");
        if (separated[0] === 'receiver') {
            updateRCcontrols(separated.splice(1, separated.length));
        }
    });

    var updateRCcontrols = function (values) {
		var chart = $('#container-throttle').highcharts();
		if (chart) {
			chart.series[0].points[0].update(parseInt(values[0]));
		}

    	chart = $('#container-yaw').highcharts();
		if (chart) {
			chart.series[0].points[0].update(parseInt(values[1]))
		}
    		
    	chart = $('#container-pitch').highcharts();
		if (chart) {
			chart.series[0].points[0].update(parseInt(values[2]));
		}

    	chart = $('#container-roll').highcharts();
		if (chart) {
			chart.series[0].points[0].update(parseInt(values[3]));
		}
    }

});


