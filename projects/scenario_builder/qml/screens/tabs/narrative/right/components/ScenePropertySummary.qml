import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0


GridLayout {
  // property ListModel properties  //So, Loader properties are overriden by properties in a Component file
  // property string name           //This class requires 3 properties, but how to make that apparent if you can't define them in the file
  // property string notes
  id : root

  columns : 2
  rows : 2

  // Label {
  // Layout.column : 0
  // Layout.row : 0
  // id : nameText
  // text : "%1: %2".arg(type).arg(name)
  // font.pointSize : 10
  // color : "steelblue"
  // width : 100
  // }
  // Text {
  // height : 100
  // width : 100
  // Layout.preferredHeight : 100
  // Layout.preferredWidth : 100
  // id : notesText

  // wrapMode : Text.WrapAnywhere
  // text : notes
  // color : "black"

  // Rectangle {
  //     anchors.fill : parent
  //     anchors.centerIn : parent
  //     color : "purple"
  // }
  // }

  // ColumnLayout {
  // Layout.fillWidth : true
  // Layout.fillHeight : true
  // Repeater {
  //     Layout.fillWidth : true
  //     Layout.minimumWidth : 200
  //     Layout.minimumHeight : 40
  //     Layout.preferredHeight : 40
  //     model : properties
  //     delegate : RowLayout {
  //       spacing : 5
  //       Text {
  //         font.pointSize : 10
  //         text : model.name + ":"
  //         Layout.minimumWidth : 50
  //       }

  //       Text {
  //         font.pointSize : 10
  //         text : model.value
  //         Layout.minimumWidth : 50
  //       }
  //     }
  // }
  // }

  Rectangle {
    id : labelArea
    Layout.preferredWidth : root.width / 4
    Layout.preferredHeight : root.height / 4
    Layout.minimumHeight : 50
    Layout.minimumWidth : 250
    Layout.fillWidth : true
    Layout.leftMargin : 10
    border.color : "blue"
    border.width : 5
    anchors.margins : 5

    Label {
      id : nameLabel
      anchors.left : labelArea.left
      anchors.top : labelArea.top
      text : "%1: ".arg(type)
    }
    Text {
      id : nameText
      anchors.left : nameLabel.right;
      anchors.leftMargin : 5;
      text : name
    }
  }
  Rectangle {
    Layout.preferredWidth : root.width / 4
    Layout.preferredHeight : root.height / 4
    Layout.minimumHeight : 50
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.rowSpan : 2
    border.color : "green"
    border.width : 5

    TextArea {
      anchors.fill : parent
      wrapMode : Text.WrapAnywhere
      text : notes
    }
  }
  Rectangle {
    Layout.preferredWidth : root.width / 4
    Layout.preferredHeight : root.height / 4
    Layout.minimumHeight : 50
    Layout.fillWidth : true
    border.color : "yellow"
    border.width : 5

    ColumnLayout {
      anchors.fill : parent
      Layout.fillWidth : true
      Layout.fillHeight : true
      Repeater {
        Layout.fillWidth : true
        Layout.minimumWidth : 200
        Layout.minimumHeight : 40
        Layout.preferredHeight : 40
        model : properties
        delegate : RowLayout {
          spacing : 5
          Text {
            font.pointSize : 10
            text : model.name + ":"
            Layout.minimumWidth : 50
          }

          Text {
            font.pointSize : 10
            text : model.value
            Layout.minimumWidth : 50
          }
        }
      }
    }
  }

  Component.onCompleted : {
    console.log("ScenePropertySummary", properties)
  }
}
