var pid = angular.module('pidApp', []);

pid.controller('PidList', function ($scope) {

  $scope.pids = {
    x : {
      name : "Pitch",
      p : "0.000",
      i : "0.000",
      d : "0.000"
    },
    y: {
      name : "Roll",
      p : "0.000",
      i : "0.000",
      d : "0.000"
    },
    z : {
      name : "Yaw",
      p : "0.000",
      i : "0.000",
      d : "0.000"
    }
  }

  $(window).on("pidUpdated", function (e, values) {
    values = values.values;
    	$scope.pids.x.p = values[0];
    	$scope.pids.x.i = values[1];
    	$scope.pids.x.d = values[2];
    	$scope.pids.y.p = values[3];
    	$scope.pids.y.i = values[4];
    	$scope.pids.y.d = values[5];
    	$scope.pids.z.p = values[6];
    	$scope.pids.z.i = values[7];
    	$scope.pids.z.d = values[8];
  });

});