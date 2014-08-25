$(function () {

    var gaugeOptions = {

        chart: {
            type: 'gauge'
        },

        title: null,

        pane: {
            center: ['50%', '85%'],
            size: '130%',
            startAngle: -90,
            endAngle: 90,
            background: {
                backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#49A33F',
                innerRadius: '70%',
                outerRadius: '100%',
                shape: 'arc'
            }
        },

        tooltip: {
            enabled: false
        },

        yAxis: {
            lineWidth: 0,
            minorTickInterval: null,
            tickPixelInterval: 100,
            tickWidth: 0,
            title: {
                y: -20
            },
            labels: {
                enabled: false
            }
        },

        plotOptions: {
            gauge: {
                dataLabels: {
                    y: 25,
                    borderWidth: 0,
                    useHTML: true
                }
            }
        }
    };

    $('#container-throttle').highcharts(Highcharts.merge(gaugeOptions, {
        yAxis: {
	   plotBands: [{
	         from: 40,
	         to: 75,
	         color: 'green',
	         thickness: 8
	     }, {
	         from: 75,
	         to: 90,
	         color: 'orange',
	         thickness:8
	     }, {
	         from: 90,
	         to: 100,
	         color: 'red',
	         thickness:8
	     }],
            min: 0,
            max: 100,
            title: {
                text: 'Throttle',
                style: {"font-size": "10px"}
            }
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Throttle',
            data: [0]
        }]

    }));


    $('#container-yaw').highcharts(Highcharts.merge(gaugeOptions, {
        yAxis: {
	    plotBands: [{
	         from: -50,
	         to: -40,
	         color: '#d51921',
	         thickness: 8
	     }, {
	         from: 40,
	         to: 50,
	         color: '#d51921',
	         thickness:8
	     },{
	         from: -40,
	         to: -20,
	         color: 'orange',
	         thickness: 8
	     }, {
	         from: 20,
	         to: 40,
	         color: 'orange',
	         thickness:8
	     }],
            min: -50,
            max: 50,
            title: {
                text: 'Yaw',
                style: {"font-size": "10px"}
            }
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Yaw',
            data: [0]
        }]

    }));

   
    $('#container-pitch').highcharts(Highcharts.merge(gaugeOptions, {
        yAxis: {
	    plotBands: [{
	         from: -30,
	         to: -25,
	         color: '#d51921',
	         thickness: 8
	     }, {
	         from: 25,
	         to: 30,
	         color: '#d51921',
	         thickness:8
	     },{
	         from: -25,
	         to: -15,
	         color: 'orange',
	         thickness: 8
	     }, {
	         from: 15,
	         to: 25,
	         color: 'orange',
	         thickness:8
	     }],
            min: -30,
            max: 30,
            title: {
                text: 'Pitch',
                style: {"font-size": "10px"}
            }
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Pitch',
            data: [0]
        }]

    }));

   
    $('#container-roll').highcharts(Highcharts.merge(gaugeOptions, {
        yAxis: {
	    plotBands: [{
	         from: -30,
	         to: -25,
	         color: '#d51921',
	         thickness: 8
	     }, {
	         from: 25,
	         to: 30,
	         color: '#d51921',
	         thickness:8
	     },{
	         from: -25,
	         to: -15,
	         color: 'orange',
	         thickness: 8
	     }, {
	         from: 15,
	         to: 25,
	         color: 'orange',
	         thickness:8
	     }],
            min: -30,
            max: 30,
            title: {
                text: 'Roll',
                style: {"font-size": "10px"}
            }
        },

        credits: {
            enabled: false
        },

        series: [{
            name: 'Roll',
            data: [0]
        }]

    }));


});
