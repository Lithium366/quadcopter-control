quadcopter.controller('enginesController', function ($scope) {
    $scope.axes = [
        {name:'x'},
        {name:'y'},
        {name:'z'}
    ];

    $scope.currentAxe = $scope.axes[0];
    $scope.play = true;

    $scope.$watch('currentAxe', function() {
        prop = $scope.currentAxe.name;
    });

    $scope.togglePlay = function () {
        $scope.play = !$scope.play;
    };

    var val = 0;
    var vybro = [];
    var prop = $scope.currentAxe.name;

    $(window).on("dataUpdated", function (e, val) {
        if (accel && $scope.play) {
            conv(vybro, val["engine" + prop]);
            accel.render();
        }
    });

    var accel = new CanvasJS.Chart("accelContainer", {
        title: {
            text: "PID output"
        },
        zoomEnabled: false,
        animationEnabled: false,
        axisY: {
            minimum: -400,
            maximum: 400
        },
        data: [
            {
                type: "line",
                showInLegend: true,
                name: "PID output values in microseconds",
                dataPoints: vybro
            }
        ]
    });


    function conv (arr, data) {
        if (!data) return;

        for (var i = 0; i < data.length; i++) {
            arr.push({
                x: val,
                y: parseInt(data[i]) ? parseInt(data[i]) : (parseInt(data[i-1]) || 0)
            });
            if (arr.length > 500) {
                arr.shift();
            }
            val++;
        }

    }
});