import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property string label : "Label"
  property string labelPlaural : label + "s"

  property alias count : listArea.count
  property alias model : listArea.model
  property alias delegate : listArea.delegate
  property alias current : listArea.currentIndex
  property alias highlight : listArea.highlight

  signal list()
  signal added(int index)
  signal removed(int index)

  height : content.height

  Layout.rightMargin : 20
  Layout.alignment : Qt.AlignTop

  Label {
    id : name
    Layout.alignment : Qt.AlignTop
    text : root.labelPlaural
    font.pointSize : 10
    color : "steelblue"
    width : 100
  }

  Rectangle {
    id : content
    color : "transparent"
    border.color : "grey"
    radius : 2
    height : 300
    anchors {
      left : name.right;
      right : parent.right;
    }

    PFCButton {
      id : listButton
      text : "List " + root.label + "s"
      anchors.left : content.left
      anchors.leftMargin : 5

      onClicked : {
        root.list()
      }
      onComplete : {}
    }
    PFCButton {
      id : addButton
      text : "Add " + root.label
      anchors.left : listButton.right
      anchors.right : removeButton.left
      anchors.leftMargin : 5
      anchors.rightMargin : 5

      onClicked : {
        root.added(root.model.count)
      }
      onComplete : {}
    }
    PFCButton {
      id : removeButton
      text : "Remove " + root.label
      anchors.right : content.right
      anchors.rightMargin : 5


      onClicked : {
        root.removed(root.current)
      }
      onComplete : {}
    }

    ListView {
      id : listArea
      anchors {
        left : parent.left;
        right : parent.right;
        top : addButton.bottom;
        topMargin : 5
        bottom : content.bottom
      }
      spacing : 5
      clip : true
      highlightFollowsCurrentItem : true
      highlightMoveDuration : 1
      highlight : Rectangle {
        color : '#1111110F'
        anchors.left : (parent) ? parent.left : undefined
        anchors.right : (parent) ? parent.right : undefined
        anchors.margins : 5

      }

      ScrollBar.vertical : ScrollBar {}
    }
  }
}
