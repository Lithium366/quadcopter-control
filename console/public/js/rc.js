$(function () {

var gaugeOptions = {

    chart: {
        type: 'solidgauge'
    },

    title: null,

    pane: {
        center: ['50%', '85%'],
        size: '130%',
        startAngle: -90,
        endAngle: 90,
        background: {
            backgroundColor: (Highcharts.theme && Highcharts.theme.background2) || '#EEE',
            innerRadius: '50%',
            outerRadius: '100%',
            shape: 'arc'
        }
    },

    tooltip: {
        enabled: false
    },

    yAxis: {
        stops: [
            [0.1, '#55BF3B'], // green
            [0.5, '#DDDF0D'], // yellow
            [0.9, '#DF5353'] // red
        ],
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
        solidgauge: {
            dataLabels: {
                y: 20,
                borderWidth: 0,
                useHTML: true
            }
        }
    }
};

$('#container-throttle').highcharts(Highcharts.merge(gaugeOptions, {
        yAxis: {
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

});
