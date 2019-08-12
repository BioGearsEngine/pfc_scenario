import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
  id: root
  property alias label: name.text
  property alias text: textEntry.text
  property alias placeholderText: textEntry.placeholderText

  height : childrenRect.height
  signal editingFinished()
  
  Layout.preferredHeight : textEntry.height 
  Layout.rightMargin : 20
  
  Label {
  id: name
   text: 'Title:'
   font.pointSize : 10
   color: "steelblue"
   width: (text.width > 90 ) ? text.width + 10 : 100
  }
  TextField {
    id : textEntry
    maximumLength: 128
    placeholderText: qsTr( 'String Field (128 Characters)')
    font.pointSize : 10
    anchors { left : name.right ; right : parent.right}
    leftPadding : 5
    rightPadding: 5
    selectByMouse : true
    onEditingFinished : {
      root.editingFinished();
    }
  }
}