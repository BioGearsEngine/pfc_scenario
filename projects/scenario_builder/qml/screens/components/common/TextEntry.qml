import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
  id: root
  property alias label: name.text
  property alias value: value.text
  property alias placeholderText: value.placeholderText

  signal editingFinished()
  
  Layout.preferredHeight : value.height 
  
  Label {
  id: name
   text: 'Title:'
   font.pointSize : 10
   color: "steelblue"
   width: 100
  }
  TextField {
    id : value
    maximumLength: 128
    placeholderText: qsTr( 'String Field (128 Characters)')
    font.pointSize : 10
    anchors { left : name.right ; right : parent.right}
    leftPadding : 5
    rightPadding: 5

    onEditingFinished : {
      root.editingFinished();
    }
  }
}