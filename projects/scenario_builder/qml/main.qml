import QtQuick 2.3
import QtQuick.Window 2.2


// main window
Window {
  id:object1
  width: 320; height: 240
  visible: true
  MouseArea {
    anchors.fill: parent
    onClicked: {
        //Qt.quit();
    }
  }
  
  Rectangle {
    anchors.fill:parent
    width: parent.width
    height: parent.height
    Text {
        anchors.centerIn: parent
        text: "Main Page"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            ld.source= "Cell.qml"
        }
    }
    Loader{
        id:ld;
        anchors.fill: parent;
    }
  }
}
