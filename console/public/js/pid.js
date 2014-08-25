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

});