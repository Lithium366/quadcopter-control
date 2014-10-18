quadcopter.controller('pidController', function ($scope) {
    $scope.axes = [
        {name:'x'},
        {name:'y'},
        {name:'z'}
    ];

    $scope.currentAxe = $scope.axes[0];
    $scope.play = true;

    $scope.pids = {
        x : {
            name : "Pitch",
            p : 0,
            i : 0,
            d : 0
        },
        y: {
            name : "Roll",
            p : 0,
            i : 0,
            d : 0
        },
        z : {
            name : "Yaw",
            p : 0,
            i : 0,
            d : 0
        }
    };

    $scope.$watch('currentAxe', function() {
        prop = $scope.currentAxe.name;
    });

    $scope.togglePlay = function () {
        $scope.play = !$scope.play;
    };

    $scope.getPid = function () {
        socket.emit("getPid");
    };

    $scope.setPid = function () {

    };

    var val = 0;
    var error = [];
    var prop = $scope.currentAxe.name;

    $(window).on("dataUpdated", function (e, val) {
        if (val.pid) {
            var values = val.pid;
            if (values.length === 9) {
                $scope.pids.x.p = values[0];
                $scope.pids.x.i = values[1];
                $scope.pids.x.d = values[2];
                $scope.pids.y.p = values[3];
                $scope.pids.y.i = values[4];
                $scope.pids.y.d = values[5];
                $scope.pids.z.p = values[6];
                $scope.pids.z.i = values[7];
                $scope.pids.z.d = values[8];
                $scope.$apply();
            }
        } else if (accel && $scope.play) {
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