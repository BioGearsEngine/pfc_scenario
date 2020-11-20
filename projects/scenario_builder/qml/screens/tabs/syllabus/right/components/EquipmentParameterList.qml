import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../../common/"
Item {
  id : parameterPane

  Label {
    id : parameterLabel
    text : 'Parameters:'
    font.pointSize : 10
    color : "steelblue"
    width : (text.width > 90) ? text.width + 10 : 100
  }
  Rectangle {
    id : parameterRect
    border.color : "grey"
    radius : 2
    height : parameterList.height + parameterControls.height
    anchors {
      left : parameterLabel.right;
      right : parent.right
      leftMargin : 5
    }

    TwoButtonRow {
      id : parameterControls
      anchors.top : parameterRect.top
      anchors.left : parameterRect.left
      anchors.right : parameterRect.right
      anchors.topMargin : 2
      anchors.rightMargin : 5
      anchors.leftMargin : 5

      firstButtonText : "Add"
      secondButtonText : "Remove"

      onFirstButtonClicked : {}
      onSecondButtonClicked : {}
    }
    ListView {
      id : parameterList
      property var fields: []
      anchors {
        top : parameterControls.bottom
        left : parent.left
        right : parent.right
        topMargin : 5
      }
      height : childrenRect.height
      spacing : 5
      clip : true
      highlightFollowsCurrentItem : true
      highlightMoveDuration : 1
      highlight : Rectangle {
        color : '#1111110F'
        Layout.alignment : Qt.AlignTop
        Layout.fillWidth : true
        Layout.margins : 5
      }
      model : ListModel {}


      delegate : Rectangle {
        id : prop
        color : 'transparent'
        border.color : 'steelblue'
        height : 30
        clip : true
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }
        MouseArea {
          anchors.fill : parent
          onClicked : {
            parameterList.currentIndex = index
          }
        }
        Label {
          id : paramNameLabel
          text : "Name:"
        }
        Text {
          id : paramNameText
          anchors.left : paramNameLabel.right
          anchors.leftMargin : 10
        }
        Label {
          id : paramTypeLabel
          text : "Type:"
          anchors.left : paramNameText.right
          anchors.leftMargin : 10
        }
        Text {
          id : paramTypeText
          anchors.left : paramTypeLabel.right
          anchors.leftMargin : 10
        }
        Label {
          id : paramValuesLabel
          anchors.left : paramTypeText.right
          anchors.leftMargin : 10
          text : "Values:"
        }
        Text {
          id : paramValuesText
          anchors.left : paramValuesLabel.right
          anchors.leftMargin : 10
        }
        Component.onCompleted : {
          console.log(index)
        }
      }
      ScrollBar.vertical : ScrollBar {}
    }
  }
}
