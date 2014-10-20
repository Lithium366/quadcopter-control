quadcopter.controller('systemController', function ($scope) {

    $(window).on("systemUpdated", function (e, values) {
        values = values.values;
        $scope.dtime = values[0];
        $scope.$apply();
    });

    $scope.getWeather = function () {
        if ($scope.position)
            $(window).trigger("obtainWeather");
    };

    $scope.armDisarm = function () {
        socket.emit("armDisarm");
    };

    $scope.setLevel = function () {
        socket.emit("setLevel");
    };

    $(window).on("obtainWeather", function (e) {
        $.getJSON("//api.openweathermap.org/data/2.5/weather", {
            lat: $scope.position.coords.latitude,
            lon: $scope.position.coords.longitude
        }, function (resp) {
            if (resp.main.pressure) {
                weather(resp.main.pressure);
            } else {
                console.log("No pressure information");
            }
        });
    });

    $(window).on("positionUpdated", function (e, position) {
        $scope.position = position;
        $scope.$apply();
        $(window).trigger("obtainWeather");
    });

    function weather(pressure) {
        $(window).trigger("weatherObtained", pressure);
        window.setTimeout(function () {
            weather(pressure);
        }, 20000);
    }

});