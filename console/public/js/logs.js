var quadcopter = angular.module('quadcopter', []);

var socket = io();

$(function () {

    socket.on('sendFile', function(data){
        var arrayOfData = dataAdapter(data);
        drawVybros(arrayOfData);
    });

    function dataAdapter(data) {
        var contentBlocks = data.split("devider\r\n");
        var dataConv = {
            vybrox: [],
            vybroy: [],
            vybroz: [],
            reciever: {
                throttle: 0,
                pitch: 0,
                roll: 0,
                yaw: 0
            },
            anglex:0,
            angley:0,
            anglez:0,
            pidx: 0,
            pidy: 0,
            pidz: 0,
            time: 0
        };
        var DataConv = function () {};
        DataConv.prototype = dataConv;
        var arrayOfData = [];

        for (var i = 0; i < contentBlocks.length; i++) {
            arrayOfData[i] = new DataConv();
            var dataArr = contentBlocks[i].split("\r\n");
            for (var j = 0; j < dataArr.length; j++) {
                var str = dataArr[j].split(":");
                switch (str[0]) {
                    case 'vybrox':
                        arrayOfData[i].vybrox = str.splice(1, 25)
                        break;
                    case 'vybroy':
                        arrayOfData[i].vybroy = str.splice(1, 25);
                        break;
                    case 'vybroz':
                        arrayOfData[i].vybroz = str.splice(1, 25);
                        break;
                    case 'angles':
                        arrayOfData[i].anglex = parseFloat(str[1]);
                        arrayOfData[i].angley = parseFloat(str[2]);
                        arrayOfData[i].anglez = parseFloat(str[3]);
                        break;
                    case 'reciever':
                        arrayOfData[i].reciever.throttle = parseFloat(str[1]);
                        arrayOfData[i].reciever.pitch = parseFloat(str[2]);
                        arrayOfData[i].reciever.roll = parseFloat(str[3]);
                        arrayOfData[i].reciever.yaw = parseFloat(str[4]);
                        break;
                    default:
                        break;
                }
            }
        }
        return arrayOfData;
    }

    function drawVybros(data) {
        var accel = new CanvasJS.Chart("accelContainer", {
            title: {
                text: "Accelerometer data"
            },
            zoomEnabled: true,
            data: [
                {
                    type: "line",
                    showInLegend: true,
                    name: "series1",
                    legendText: "X",
                    dataPoints: getAccelPoints('vybrox')
                },
                {
                    type: "line",
                    showInLegend: true,
                    name: "series3",
                    legendText: "Y",
                    dataPoints: getAccelPoints('vybroy')
                },
                {
                    type: "line",
                    showInLegend: true,
                    name: "series3",
                    legendText: "Z",
                    dataPoints: getAccelPoints('vybroz')
                }
            ]
        });
        accel.render();

        function getAccelPoints(prop) {
            var points = [];
            for (var i = 0; i < data.length; i++) {
                var obj = data[i][prop];
                for (var j = 0; j < 25; j++) {
                    points.push({
                        y: parseInt(obj[j] || 0) / 1000
                    });
                }
            }
            return points;
        }

        var angles = new CanvasJS.Chart("anglesContainer", {
            title: {
                text: "Angles"
            },
            zoomEnabled: true,
            data: [
                {
                    type: "line",
                    showInLegend: true,
                    name: "series1",
                    legendText: "X",
                    dataPoints: getAnglePoints('anglex')
                },
                {
                    type: "line",
                    showInLegend: true,
                    name: "series3",
                    legendText: "Y",
                    dataPoints: getAnglePoints('angley')
                },
                {
                    type: "line",
                    showInLegend: true,
                    name: "series3",
                    legendText: "Z",
                    dataPoints: getAnglePoints('anglez')
                }
            ]
        });
        angles.render();

        function getAnglePoints(prop) {
            var points = [];
            for (var i = 0; i < data.length; i++) {
                var obj = data[i][prop];
                points.push({
                    y: parseInt(obj || 0)
                });
            }
            return points;
        }

    }

    socket.emit('getFile');
});