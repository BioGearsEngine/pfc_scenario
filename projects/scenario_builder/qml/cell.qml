import QtQuick 2.0

Item {
    id: container
    //property alias cellColor: rectangle.color
    //signal clicked(color cellColor)

    width: 40; height: 25

  Rectangle {
    id: object2
	anchors.centerIn: parent
    width: parent.width
    height: parent.height
    color: "navy";
    Text {
        anchors.centerIn: parent
        text: "Second Window"
		color: "white"
		//fontSize: 10;
    }

    MouseArea{
        anchors.fill: parent;
        onClicked: ld.source= "";
    }

  }
}