import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Item {
  id: root
  property alias nameWidth : name.width
  property alias label: name.text
  property alias text: textEntry.text
  property alias placeholderText: textEntry.placeholderText
  property alias inputMethodHints : textEntry.inputMethodHints
  property alias entryField : textEntry
  property int pointSize : 10
  height : childrenRect.height
  width  : childrenRect.width

  signal editingFinished()
  signal labelWidthChanged()
  
  Layout.preferredHeight : textEntry.height 
  Layout.rightMargin : 20
  
  Label {
  id: name
   text: 'Title:'
   font.pointSize : root.pointSize
   color: "steelblue"
   width: (text.width > 90 ) ? text.width + 10 : 100
  }
  TextField {
    id : textEntry
    maximumLength: 128
    placeholderText: qsTr( 'String Field (128 Characters)')
    font.pointSize : root.pointSize
    anchors { left : name.right ; right : parent.right}
    leftPadding : 5
    rightPadding: 5
    selectByMouse : true
    onEditingFinished : {
      root.editingFinished();
    }
  }
  Component.onCompleted: {
    if (name.width + 10 < name.contentWidth) {
      name.width = name.contentWidth + 10
      root.labelWidthChanged()
    }
  }
}