import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
id: root
Layout.preferredHeight : 30
height : section_1.height

property string label : "Label"
property alias firstButtonText : firstButton.text
property alias secondButtonText : secondButton.text

signal firstButtonClicked();
signal secondButtonClicked();

Rectangle{
  id: section_1
  width: parent.width /2
  height : firstButton.height
  color : "transparent"
  PFCButton {
      id: firstButton
      anchors.left : section_1.left
      text : "New " + root.label
      onClicked : {
        root.firstButtonClicked()
      }
  }
}

Rectangle{
  id: section_2
  width: parent.width /2
  height : secondButton.height
  anchors.left : section_1.right
  color : "transparent"
  PFCButton {
      id : secondButton
      anchors.right : section_2.right
      text : "Delete " + root.label
      onClicked : {
        root.secondButtonClicked()
      }
  }
}
}

/*##^## Designer {
    D{i:0;height:200;width:750}
}
 ##^##*/
