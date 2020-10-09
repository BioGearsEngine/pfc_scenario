import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
id: root
Layout.preferredHeight : 30
height : section_1.height

property alias firstButtonText : firstButton.text
property alias secondButtonText : secondButton.text
property alias thirdButtonText : thirdButton.text
property alias fourthButtonText : fourthButton.text

signal firstButtonClicked()
signal secondButtonClicked()
signal thirdButtonClicked()
signal fourthButtonClicked();

Rectangle{
  id: section_1
  width: parent.width /3
  height : secondButton.height
  color : "transparent"
  PFCButton {
      id: firstButton
      anchors.left : section_1.left
      text : "LL Button"
      onClicked : {
        root.firstButtonClicked()
      }
  }
}

Rectangle{
  id: section_2
  width: parent.width /3
  height : secondButton.height
  anchors.left : section_1.right
  color : "transparent"
  PFCButton {
      id: secondButton
      anchors.left : section_2.left
      text : "ML Button"

      onClicked : {
        root.secondButtonClicked()
      }
  }

  PFCButton {
      id: thirdButton
      anchors.right : section_2.right
      text : "MR Button"
      onClicked : {
        root.thirdButtonClicked()
      }
  }  
}

Rectangle{
  id: section_3
  width: parent.width /3
  height : secondButton.height
  anchors.left : section_2.right
  color : "transparent"
  PFCButton {
      id : fourthButton
      anchors.right : section_3.right
      text : "RR Button"
      onClicked : {
        root.fourthButtonClicked()
      }
  }
}
}

/*##^## Designer {
    D{i:0;height:200;width:750}
}
 ##^##*/
