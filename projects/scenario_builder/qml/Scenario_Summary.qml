import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import com.ara.pfc.ScenarioModel 1.0
//---------------------------------------------------------------------------
Item {
  id:root
  property StackView stack : null
  property ScenarioModel model : null

  Component {
    id: delegateComp

    RowLayout
    {
      width: parent.width
      Text {
        id: scenario_model_label
        font.pointSize: 10
        text: label + ":  "
        font.weight: Font.Bold
        verticalAlignment : Text.AlignTop
        anchors {
          left : parent.left
        }
        Layout.alignment: (scenario_model_content.width >
                            scenario_model_content.contentWidth ) ? Qt.AlignCenter:Qt.AlignTop
        Layout.minimumWidth: 75
      }

      Text {
        id: scenario_model_content
        font.pointSize: 10
        text: content
        wrapMode: Text.Wrap
        horizontalAlignment : Text.AlignJustify

         anchors {
          left : scenario_model_label.right
          rightMargin: 10
        }
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignCenter
      }
    }
  }//Delegate Component

  Label {
    id: header
    text: "Summary"
    font.pixelSize: 22
    font.italic: true
    color: "steelblue"

    anchors {
      top: parent.top
      horizontalCenter : parent.horizontalCenter
    }
    Layout.alignment: Qt.AlignVCenter
  }//end label

  Button {
    id: control
    anchors {
     right : parent.right
       top : parent.top
       topMargin : 2
       rightMargin : 8
      
    }

    text: qsTr("Edit")
    font.pointSize : 8

    Component {
      id: dialog
      Screen_Scenario_Summry_Dialog { 
      }
    }
    onClicked: {
      root.stack.push(
        dialog, { data_model : root.model}
      )
    }
    Layout.alignment: Qt.AlignTop
  }

    
  ListView {
    id:summaryList
    anchors {
      top: header.bottom
      left: parent.left;  leftMargin: 10
      right: parent.right; rightMargin: 10
    }
    spacing: 6
    clip:true
    orientation:ListView.Vertical
    model: root.model
    delegate: delegateComp
    onContentHeightChanged: {
      if ( height < contentHeight) {
        height = contentHeight
      }
      root.height =root.height = childrenRect.height+ header.height + 30;
    }
    onHeightChanged: {
      if ( height < contentHeight) {
           height = contentHeight
      }
    }
  }//End list view

  height : summaryList.height + header.height
} // End Main Item