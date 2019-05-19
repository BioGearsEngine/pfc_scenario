import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import com.ara.pfc.ScenarioModel 1.0
import com.ara.pfc.ScenarioModel.Locations 1.0
Item {
  id:root
  anchors.fill : parent.fill

  property StackView stack
  property Locations model

  Component {
    id: delegateComp
    RowLayout
    {
      width: parent.width
      Text {
        id: scenario_model_label
        font.pointSize: 10
        text: name + ":"
        font.weight: Font.Bold
        verticalAlignment : Text.AlignTop
        anchors {
          left : parent.left
        }
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: 75
      }

      Text {
        id: scenario_model_lat
        font.pointSize: 10
        text: "<i>  lat: </i>" + lat + "<i>  lon: </i>" + lon + "<i>  alt: </i>" + alt
        wrapMode: Text.Wrap
        horizontalAlignment : Text.AlignJustify

         anchors {
          left : scenario_model_label.right
          rightMargin: 10
          leftMargin: 10
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
          Screen_Scenario_Locations_Dialog { 
          }
        }
        onClicked: {
          stack.push(
            dialog, { data_model : model}
          )
        }
      }
    }
  }

  Label {
    id: header
    text: "Locations"
    font.pointSize  : 14
    font.italic: true
    color: "steelblue"

    anchors {
      top: parent.top
      horizontalCenter : parent.horizontalCenter
    }
    Layout.alignment: Qt.AlignVCenter
    height : 50;
  }

  ListView {
    id:locationList
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
      root.height = childrenRect.height + header.height;
      console.log("onContentHeightChanged("+root.height+")")
    }
    onHeightChanged: {
      if ( height < contentHeight) {
            height = contentHeight
      }
      console.log("onHeightChanged("+height+")")
    }
  }

  Button {
    id: button
    anchors {
      right : parent.right
      verticalCenter: header.verticalCenter
    }
    text: "New Location"
    font.pointSize  : 10
    onClicked: {
      root.model.new_location()
    }

  }
  height : header.childrenRect.height
}