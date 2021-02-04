import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQml 2.0
Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button{
        id:button
        x:47
        y:229
        text:qsTr("Button 1")
        antialiasing: true
        scale: 1.3
        onClicked: display.changeValue(1);
}
    Label{
        id:label
        x:23
        y:88
        width: 355
        height: 62
        text: qsTr("Label")
        horizontalAlignment: Text.AlignHCenter
        elide: Text.ElideMiddle
        font.capitalization: Font.MixedCase
        font.family: "Arial"
        font.pointSize: 20

    Connections{
        target: display
        onValueChanged: label.text = s;
    }}

}
