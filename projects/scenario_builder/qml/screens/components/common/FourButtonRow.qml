import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
Layout.preferredHeight : 30

property alias leftText : lButton.text
property alias mLeftText : mLButton.text
property alias mRightText : mRButton.text
property alias rightText : rButton.text

Rectangle{
  id: section_1
  width: parent.width /3
  height : mLButton.height
  color : "transparent"
  PFCButton {
      id: lButton
      anchors.left : section_1.left
      text : "LL Button"
  }
}
Rectangle{
  id: section_2
  width: parent.width /3
  height : mLButton.height
  anchors.left : section_1.right
  color : "transparent"
  PFCButton {
      id: mLButton
      anchors.left : section_2.left
      text : "ML Button"
  }

  PFCButton {
      id: mRButton
      anchors.right : section_2.right
      text : "MR Button"
  }
}
Rectangle{
  id: section_3
  width: parent.width /3
  height : mLButton.height
  anchors.left : section_2.right
  color : "transparent"
  PFCButton {
      id : rButton
      anchors.right : section_3.right
      text : "RR Button"
  }
}
}

/*##^## Designer {
    D{i:0;height:200;width:750}
}
 ##^##*/
