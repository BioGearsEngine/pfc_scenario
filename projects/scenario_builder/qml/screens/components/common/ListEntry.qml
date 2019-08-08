import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property alias label: name.text
  property alias model : listArea.model
  property alias delegate : listArea.delegate
  property alias current : listArea.currentIndex
  property alias highlight : listArea.highlight

  signal added(int index)
  signal removed(int index)

  height : content.height
  Layout.rightMargin : 20
  Layout.alignment : Qt.AlignTop

  Label {
  id: name
  Layout.alignment : Qt.AlignTop
   text: 'Label'
   font.pointSize : 10
   color: "steelblue"
   width: 100
  }

  Rectangle {
    id : content
    color: "transparent"
    border.color: "grey"
    radius: 2
    height : 300
    anchors { left : name.right; right: parent.right ; }

   PFCButton {
      id : addButton
      text : "Add " + name.text
      anchors.left : content.left
      anchors.leftMargin : 5
  
      onClicked : {
        root.added(root.model.count)
      }
      onComplete: console.log("Add Button works")
    }

    PFCButton {
      id: removeButton
      text : "Remove " + name.text
      anchors.right : content.right
      anchors.rightMargin : 5

      
      onClicked : {
        root.removed(root.current )
      }
      onComplete: console.log("Remove Button works") 
    }

    ListView {
      id : listArea
      anchors { left : parent.left; right: parent.right ; 
                top   : addButton.bottom ; topMargin : 5
                bottom : content.bottom
              }
      spacing : 5
      clip: true
      highlightFollowsCurrentItem : true

      highlight: Rectangle {
          color: '#1111110F'
          Layout.fillWidth : true
          Layout.margins: 5
      }

      ScrollBar.vertical: ScrollBar { }
    }
  }
}
