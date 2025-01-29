import QtQuick
import QtCharts
import QtQuick.Controls

Item {
    width: 800
    height: 600

    property int minDistance: 0
    property int maxDistance: 5000

    function updatePoints(points) {
        lidarSeries.clear();
        for (var i = 0; i < points.length; i++) {
            let d = points[i].distance;
            if (d >= minDistance && d <= maxDistance) {
                lidarSeries.append(points[i].angle, d);
            }
        }
    }

    function updateAxisRange() {
        axisRadial.min = minDistance;
        axisRadial.max = maxDistance;
    }

    Column {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10
        spacing: 10

        Text {
            text: "Distance Filter"
            font.bold: true
        }

        ComboBox {
            id: distanceFilter
            model: ["0-5m", "0-2m)", "5m+"]
            onCurrentIndexChanged: {
                if (currentIndex === 0) {
                    minDistance = 0;
                    maxDistance = 5000;
                } else if (currentIndex === 1) {
                    minDistance = 0;
                    maxDistance = 2000;
                } else if (currentIndex === 2) {
                    minDistance = 0;
                    maxDistance = 15000;
                }
                updateAxisRange();
                updatePoints(scanData.points);
            }
        }
    }

    PolarChartView {
        id: chart
        title: "Lidar Scan Data"
        anchors.fill: parent
        anchors.topMargin: 50
        legend.visible: false
        antialiasing: true

        ValueAxis {
            id: axisAngular
            min: 0
            max: 360
            tickCount: 9
        }

        ValueAxis {
            id: axisRadial
            min: 0
            max: 5000
            labelFormat: "%.1f"
        }

        ScatterSeries {
            id: lidarSeries
            axisAngular: axisAngular
            axisRadial: axisRadial
            markerSize: 5
            color: "blue"
        }

        Component.onCompleted: {
            updatePoints(scanData.points);
        }

        Connections {
            target: scanData

            function onPointsChanged() {
                updatePoints(scanData.points);
            }
        }
    }
}
