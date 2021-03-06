quadcopter.controller('vybroController', function ($scope) {
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
            conv(vybro, val["vybro" + prop]);
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
                name: "Accelerometer data in G",
                dataPoints: vybro
            }
        ]
    });


    function conv (arr, data) {
        if (!data) return;

        for (var i = 0; i < data.length; i++) {
            arr.push({
                x: val,
                y: parseInt(data[i]) / 1000
            });
            if (arr.length > 2000) {
                arr.shift();
            }
            val++;
        }

    }
});