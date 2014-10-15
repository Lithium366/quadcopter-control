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

    $(window).on("anglesUpdated", function (e, values) {
        $("#img1")[0].setAttribute("patternTransform", "rotate(" + values.values[0] + ", 100, 100) translate(0,"+ values.values[1] + ")");
        $("#compass_scale").css("transform", "rotate(" + values.values[2] + "deg)");
        var feets = parseInt(parseFloat(values.values[3]) * 3.28084);
        var feetsSm = Math.round(feets / 2.7778);
        var feetsLg = Math.round(feets / 27.778);
        $("#altSm").css("transform", "rotate(" + feetsSm + "deg)");
        $("#altLg").css("transform", "rotate(" + feetsLg + "deg)");
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