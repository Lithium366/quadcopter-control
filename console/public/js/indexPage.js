$(function () {

    // If no new data in 200ms - set online indicator to false
    var online = false,
        map = false,
        copter = false,
        me = false;

    function setOffline() {
        if (!online) {
            $(window).trigger("onlineUpdated", false);
        }
        online = false;
        window.setTimeout(setOffline, 200);
    }
    setOffline();

    // Once serial data available, notify observers
    var prevLat = 0;
    var prevLon = 0;
    var tmot = null;
    $(window).on("dataUpdated", function (e, val) {
        if (val.reciever) {
            updateRCcontrols(val.reciever);
        }
        if (val.pid) {
            $(window).trigger("pidUpdated", {values: val.pid});
        }
        if (val.system) {
            if (parseInt(val.system[1]) !== 1000 && parseInt(val.system[2]) !== 1000) {
                if (prevLat !== val.system[1] && prevLon !== val.system[2]) {
                    window.clearTimeout(tmot);
                    tmot = window.setTimeout(function () {
                        if (prevLat === val.system[1] && prevLon === val.system[2]) {
                            $(window).trigger("gpsUpdated", false);
                        }
                    }, 5000);
                    $(window).trigger("gpsUpdated", true);
                    prevLat = val.system[1];
                    prevLon = val.system[2];

                    if (copter) {
                        copter.setPosition(new google.maps.LatLng(val.system[1], val.system[2]));
                    }
                }
            }
            $(window).trigger("systemUpdated", {values: val.system});
        }
        if (val.angles) {
            $(window).trigger("anglesUpdated", {values: val.angles});
        }
        // Set online indicator to true every time new data came
        online = true;
        $(window).trigger("onlineUpdated", true);
    });

    var counter = 0;
    var feets = 0;
    $(window).on("anglesUpdated", function (e, values) {
        $("#img1")[0].setAttribute("patternTransform", "rotate(" + values.values[0] + ", 100, 100) translate(0,"+ values.values[1] + ")");
        $("#compass_scale").css("transform", "rotate(" + (-1 * values.values[2]) + "deg)");
        feets += parseInt(parseFloat(values.values[3]) * 3.28084);
        counter++;
        if (counter == 5) {
            var feetsSm = Math.round(feets / 5 / 2.7778);
            var feetsLg = Math.round(feets / 5 / 27.778);
            $("#altSm").css("transform", "rotate(" + feetsSm + "deg)");
            $("#altLg").css("transform", "rotate(" + feetsLg + "deg)");
            counter = 0;
            feets = 0;
        }
        var fps = parseInt(parseFloat(values.values[4]) * 3.28084);
        $("#vspeed").css("transform", "rotate(" + (fps * 9 - 90) + "deg)");
    });

    var prevSpeed = 0;
    $(window).on("systemUpdated", function (e, values) {
       values = values.values;
       if(prevSpeed !== parseFloat(values[3]).toFixed(1)) {
         prevSpeed = parseFloat(values[3]).toFixed(1);
         $("#airspeed").css("transform", "rotate(" + (prevSpeed * 7.2) + "deg)");
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
    };

    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(showPosition);
    }

    function showPosition(position) {

        $(window).trigger("positionUpdated", position);

        map = new google.maps.Map($('#container-gmap')[0], {
            center: new google.maps.LatLng(position.coords.latitude, position.coords.longitude),
            zoom: 17,
            mapTypeId: google.maps.MapTypeId.SATELLITE
        });

        me = new google.maps.Marker({
            position: new google.maps.LatLng(position.coords.latitude, position.coords.longitude),
            map: map,
            title: "You",
            icon: 'images/pilot.png'
        });

        copter = new google.maps.Marker({
            map: map,
            title: "Quadcopter",
            icon: 'images/drone.png'
        });

        map.setTilt(0);
    }

});