quadcopter.controller('pidController', function ($scope) {
    $scope.axes = [
        {name:'x'},
        {name:'y'},
        {name:'z'}
    ];

    $scope.currentAxe = $scope.axes[0];
    $scope.play = true;
    $scope.savedPid = true;

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
        var arr = [
            parseInt($scope.pids.x.p * 1000) || 0,
            parseInt($scope.pids.x.i * 1000) || 0,
            parseInt($scope.pids.x.d * 1000) || 0,
            parseInt($scope.pids.y.p * 1000) || 0,
            parseInt($scope.pids.y.i * 1000) || 0,
            parseInt($scope.pids.y.d * 1000) || 0,
            parseInt($scope.pids.z.p * 1000) || 0,
            parseInt($scope.pids.z.i * 1000) || 0,
            parseInt($scope.pids.z.d * 1000) || 0
        ];
        var chksumm = 0;
        for (var i = 0; i < arr.length; i++) {
            chksumm += arr[i]
        }
        arr.push(chksumm);
        $scope.savedPid = false;
        socket.emit("setPid", arr.join(":"));
    };

    socket.on("pidSaved", function () {
        $scope.savedPid = true;
        $scope.$apply();
    });

    var val = 0;
    var error = [];
    var prop = $scope.currentAxe.name;

    $(window).on("dataUpdated", function (e, val) {
        if (val.pid) {
            console.log(val);
            var values = val.pid;
            if (values.length === 9) {
                $scope.pids.x.p = parseFloat(values[0]).toFixed(3);
                $scope.pids.x.i = parseFloat(values[1]).toFixed(3);
                $scope.pids.x.d = parseFloat(values[2]).toFixed(3);
                $scope.pids.y.p = parseFloat(values[3]).toFixed(3);
                $scope.pids.y.i = parseFloat(values[4]).toFixed(3);
                $scope.pids.y.d = parseFloat(values[5]).toFixed(3);
                $scope.pids.z.p = parseFloat(values[6]).toFixed(3);
                $scope.pids.z.i = parseFloat(values[7]).toFixed(3);
                $scope.pids.z.d = parseFloat(values[8]).toFixed(3);
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