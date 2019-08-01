import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id: root
  property SQLBackend backend
  property ListElement model
  
  //TAB:SYLLABUS_TAB:OBJECTIVES_LEFTWINDOW
  Rectangle {
      id : listRectangle
      Layout.fillWidth : true
      Layout.fillHeight: true
      Layout.margins : 5

      border.color : "black"

      FourButtonRow {
        id: controls
        anchors.top : listRectangle.top
        anchors.left : listRectangle.left
        anchors.right : listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin  : 5
        anchors.leftMargin  : 5

        height: 120

        firstButtonText : "Add"
        fourthButtonText : "Remove"

        secondButtonText : "Move Up"
        thirdButtonText : "Move Down"
      }

      ListView {
        id : listArea
        anchors { top : controls.bottom ; bottom : parent.bottom; 
             left : parent.left ; right : parent.right }  
        spacing : 5
        clip: true
        highlightFollowsCurrentItem : true

        highlight: Rectangle {
            color: '#1111110F'
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.margins: 5
        }

        ScrollBar.vertical: ScrollBar { }
      }
  }
}
/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
