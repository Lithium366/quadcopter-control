quadcopter.controller('PidList', function ($scope) {

  $(window).on("pidUpdated", function (e, values) {
        values = values.values;
        $scope.pids = {
            x : {
              name : "Pitch",
              p : values[0],
              i : values[1],
              d : values[2]
            },
            y: {
              name : "Roll",
              p : values[3],
              i : values[4],
              d : values[5]
            },
            z : {
              name : "Yaw",
              p : values[6],
              i : values[7],
              d : values[8]
            }
          };
        $scope.$apply();
  });

});