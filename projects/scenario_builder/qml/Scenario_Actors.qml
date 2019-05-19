import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import com.ara.pfc.ScenarioModel 1.0
import com.ara.pfc.ScenarioModel.Actors 1.0
Item {
id:root
  property StackView stack
  property Actors model

  Component {
    id: delegateComp

    RowLayout
    {
      width: parent.width
      Text {
        id: scenario_model_label
        font.pointSize: 10
        text: "Name:"
        font.weight: Font.Bold
        verticalAlignment : Text.AlignTop
        anchors {
          left : parent.left
        }
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: 75
      }

      Text {
        id: scenario_model_content
        font.pointSize: 10
        text: (0 == description.length) ? name : name + " - <i>" + description + "</i>"
        wrapMode: Text.Wrap
        horizontalAlignment : Text.AlignJustify

         anchors {
          left : scenario_model_label.right
          rightMargin: 10
        }
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
      }

      RoundButton {
        id: control
        text: qsTr("Edit")
        font.pointSize : 8

        Layout.alignment: Qt.AlignTop

        Component {
          id: dialog
          Screen_Scenario_Actors_Dialog { 
          }
        }
        onClicked: {
          stack.push(
            dialog, { data_model : model }
          )
        }
      }
    }
  }

  Label {
    id: header
    text: "Actors"
    font.pointSize  : 14
    font.italic: true
    color: "steelblue"

    anchors {
      top: parent.top
      horizontalCenter : parent.horizontalCenter
    }
    Layout.alignment: Qt.AlignVCenter
    height: 50
  }

  ListView {
    id:actorList
    anchors {
      top: header.bottom
      left: parent.left;  leftMargin: 10
      right: parent.right; rightMargin: 10
    }
    spacing: 2
    clip:true
    orientation:ListView.Vertical
    model: root.model
    delegate: delegateComp
    onContentHeightChanged: {
      if ( height < contentHeight) {
        height = contentHeight
      }
      root.height =root.height = childrenRect.height+ header.height;
    }
    onHeightChanged: {
      if ( height < contentHeight) {
           height = contentHeight
      }
    }
  }

  Button {
    id: button
    anchors {
      top : parent.top
      right : parent.right
      verticalCenter: header.verticalCenter
    }
   
    text: "New Actor"
    font.pointSize  : 10
    onClicked: {
        root.model.new_actor()
    }
  }
  height : actorList.height + header.height
}