import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQml 2.0









Window {
    width: 800
    height: 480
    visible: true
    color: "#080d25"

    Image {
        id: egocover
        x: 269
        y: 232
        width: 563
        height: 251
        source: "Ego-cover.png"
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: text1
        x: 20
        y: 17
        width: 260
        height: 58
        color: "#26e8f3"
        text: qsTr("Car Sharing ")
        font.pixelSize: 50
        wrapMode: Text.Wrap
        font.family: "Times New Roman"
        font.styleName: "Light"
        font.capitalization: Font.MixedCase
        minimumPointSize: 14
        minimumPixelSize: 14
    }

    Text {
        id: text2
        x: 194
        y: 126
        width: 107
        height: 46
        color: "#f3f5f5"
        text: qsTr("PM2.5")
        font.pixelSize: 26
        minimumPixelSize: 12
    }

    Text {
        id: text3
        x: 108
        y: 187
        width: 193
        height: 46
        color: "#f3f5f5"
        text: qsTr("Air Filter State")
        font.pixelSize: 26
        minimumPixelSize: 12
    }

    Text {
        id: text4
        x: 58
        y: 375
        width: 202
        height: 42
        color: "#f3f5f5"
        text: qsTr("Drive carefully")
        font.pixelSize: 29
    }

    Text {
        id: text5
        x: 76
        y: 415
        width: 134
        height: 42
        color: "#f3f5f5"
        text: qsTr("Good  Trip!")
        font.pixelSize: 26
    }
   /* title: qsTr("Hello World")

    Text {
        id: helloText
        text: "Hello world!"
        y: 30
        //anchors.horizontalCenter: page.horizontalCenter
        font.pointSize: 24; font.bold: true
    }*/
}

