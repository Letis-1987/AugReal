import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Extras 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import QtQuick.Scene3D 2.0
import QtMultimedia 5.9 as Mult

Item {
    anchors.fill: parent
    Keys.onPressed: Qt.quit(0)

    Scene3D {
        id: scene3d
        width: parent.width * 0.7
        height: parent.height * 0.7
        anchors.margins: 10
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

        Scene {}
    }

    Rectangle {
        width: 50
        height: 50
        color: "green"
        id: rec

        NumberAnimation {
            target: rec
            property: "x"
            from: 0
            to: 500
            running: true
            loops: Animation.Infinite
            duration: 5000
        }
        NumberAnimation {
            target: rec
            property: "y"
            from: 0
            to: 500
            running: true
            loops: Animation.Infinite
            duration: 5000
        }
    }

}
