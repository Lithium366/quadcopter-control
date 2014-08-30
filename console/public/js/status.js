quadcopter.controller('Status', function ($scope) {

   $scope.online = false;

    $(window).on("onlineUpdated", function (e, status) {
        $scope.online = status;
        $scope.$apply();
    });
});