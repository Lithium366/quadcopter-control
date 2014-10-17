$(function () {
    var valx = 0;
    var vybro = makeArr();

    function makeArr() {
        var arrk = [];
        for (var i = 0; i < valx - 1; i++) {
            arrk.push({
                x: i,
                y: 0
            });
        }
        return [];
    }

    $(window).on("dataUpdated", function (e, val) {
        if (accel) {
            conv(vybro, val.vybrox);
            accel.render();
        }
    });

    var accel = new CanvasJS.Chart("accelContainer", {
        title: {
            text: "Accelerometer data"
        },
        zoomEnabled: false,
        animationEnabled: false,
        axisY: {
            minimum: -1.5,
            maximum: 1.5,
            interval: 0.5
        },
        data: [
            {
                type: "line",
                showInLegend: true,
                name: "series1",
                legendText: "X",
                dataPoints: vybro
            }
        ]
    });


    function conv (arr, data) {
        if (!data) return;

        for (var i = 0; i < data.length; i++) {
           arr.push({
               x: valx,
               y: parseInt(data[i]) / 1000
           });
            if (arr.length > 1001) {
                arr.shift();
            }
            valx++;
        }

    }
});