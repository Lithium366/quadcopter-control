quadcopter.controller('systemController', function ($scope) {

    $(window).on("systemUpdated", function (e, values) {
        values = values.values;
        $scope.dtime = values[0];
        $scope.$apply();
    });

});