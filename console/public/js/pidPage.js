quadcopter.controller('pidController', function ($scope) {
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
    var error = [];
    var prop = $scope.currentAxe.name;

    $(window).on("dataUpdated", function (e, val) {
        if (accel && $scope.play) {
            conv(error, val["error" + prop]);
            accel.render();
        }
    });

    var accel = new CanvasJS.Chart("accelContainer", {
        title: {
            text: "PID error"
        },
        zoomEnabled: false,
        animationEnabled: false,
        axisY: {
            minimum: -20,
            maximum: 20
        },
        data: [
            {
                type: "line",
                showInLegend: true,
                name: "PID error",
                dataPoints: error
            }
        ]
    });


    function conv (arr, data) {
        if (!data) return;

        for (var i = 0; i < data.length; i++) {
            arr.push({
                x: val,
                y: parseInt(data[i]) / 100
            });
            if (arr.length > 1000) {
                arr.shift();
            }
            val++;
        }

    }
});