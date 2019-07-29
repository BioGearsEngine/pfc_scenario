import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

Button {
  id: root
   focus : true
  signal clicked()

  text : "Button"
  font.pointSize : 10

  contentItem: Text {
    id : content
    text: root.text
    font: root.font
    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
  }
  background : Rectangle {
      id : rootBackground
      border.color: root.down ? "#FF9933" : "steelblue"
  }

  onHoveredChanged :{
     root.background.color = root.hovered  ? "#1111110F" : "transparent";
  }

  onClicked : {
    root.clicked()
  }
  onPressed : {
    root.contentItem.color = "#CC6633";
  }

  onReleased : {
    root.contentItem.color = "black";
  }

}
