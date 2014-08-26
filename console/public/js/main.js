var socket = io();

socket.on('dataUpdated', function(msg){
    $(window).trigger("dataUpdated", msg);
});

var buffer =  "";

$(function () {

    $(window).on("dataUpdated", function (e, val) {
        buffer += val;
        if (buffer.length > 500) {
          buffer = buffer.substr(buffer.length - 500, 500);
        }
        var sections = buffer.match(/\|1001100\|(.*)\|1001100\|/);
        if (!sections || !sections[sections.length - 1]) return false;
        var separatedSections = sections[sections.length - 1].split("|");
        for (var i = 0; i < separatedSections.length; i++) {
          var fields = separatedSections[i].split(":");
          if (fields[0] === 'reciever') {
            updateRCcontrols(fields.splice(1, fields.length));
          } else if (fields[0] === "pid") {
            $(window).trigger("pidUpdated", {values: fields.splice(1, fields.length)});
          }
        }
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


