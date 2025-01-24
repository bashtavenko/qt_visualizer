import QtQuick
import QtCharts

Item {
    width: 800
    height: 600

    function updatePoints(points) {
        lidarSeries.clear();
        for (var i = 0; i < points.length; i++) {
            lidarSeries.append(points[i].angle, points[i].distance);
        }
    }

    PolarChartView {
        id: chart
        title: "Lidar Scan Data"
        anchors.fill: parent
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
            max: 1
            labelFormat: "%.1f"

        }

        ScatterSeries {
            id: lidarSeries
            axisAngular: axisAngular
            axisRadial: axisRadial
            markerSize: 5
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
