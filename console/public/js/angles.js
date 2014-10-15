quadcopter.controller('anglesController', function ($scope) {

    $(window).on("anglesUpdated", function (e, values) {
        values = values.values;
        $scope.x = values[0];
        $scope.y = values[1];
        $scope.z = values[2];
        $scope.$apply();
    });

});