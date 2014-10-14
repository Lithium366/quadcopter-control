var quadcopter = angular.module('quadcopter', []);

var socket = io();

$(function () {

    socket.on('sendFile', function(data){
        dataAdapter(data);
    });

    function dataAdapter(data) {
        var contentBlocks = data.split("devider\r\n");
        var dataConv = {
            vybrox: [],
            vybroy: [],
            vybroz: [],
            reciever: [],
            anglex: 0,
            angley: 0,
            anglez: 0,
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
                        arrayOfData[i].vybrox = str.splice(1, 25);
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
                    default:
                        break;
                }
            }
        }

        console.log(arrayOfData);
    }

    var chart = new CanvasJS.Chart("accelContainer", {

        title:{
            text: "Accelerometer data"
        },
        data: [
            {
                type: "line",
                dataPoints: [
                    { y: 1 },
                    { y: 3 },
                    { y: 2 },
                    { y: 1.22 },
                    { y: 7 }
                ]
            }
        ]
    });

    chart.render();

    socket.emit('getFile');
});