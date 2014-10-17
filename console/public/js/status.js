quadcopter.controller('Status', function ($scope) {

   $scope.online = false;

    $(window).on("onlineUpdated", function (e, status) {
        $scope.online = status;
        $scope.$apply();
    });

    $(window).on("gpsUpdated", function (e, status) {
        $scope.gps = status;
        $scope.$apply();
    });

    $(window).on("systemUpdated", function (e, values) {
        values = values.values;
        $scope.arm = (values[3] === "1") || false;
        $scope.$apply();
    });

    $scope.armDisarm = function () {
        socket.emit("armDisarm");
    }
});