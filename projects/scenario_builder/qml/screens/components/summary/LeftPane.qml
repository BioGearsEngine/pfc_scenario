import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"
import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property SQLBackend backend

  ColumnLayout {
    id: summary_leftWindow
    anchors.fill : parent

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_title"
      label : "Title"
      default_value : 'New Scenario'
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_version"
      label : "Version"
      default_value : '0.0.1'
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_security"
      label : "Security"
      default_value : 'UnClassified'
    }

    PropertyAreaEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_purpose"
      label : "Purpose"
      default_value : 'Enter the scenarios purpose.'
      required : false 
    }

    ListEntry {
      id: restrictions
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop

      model : ListModel {
        id: dataModel
        ListElement {
            name: "Export Control"
            value: "US - 2010"
        }
        ListElement {
            name: "Blue "
            value: "US - 9982"
        }
      }
      delegate : Rectangle {
         id : delegateArea
         anchors { left : parent.left; right: parent.right ; margins : 5 }

         color : 'transparent'
         border.color: "steelblue"
         height : 30

         Text {
           id : restriction_title_text
           anchors.left : delegateArea.left
           anchors.leftMargin : 5
           text : name + " " + value
         }

         MouseArea {
           anchors.fill: parent
           onClicked: {
            restrictions.current = index
           }
         }
      }

      onAdded : {
        console.log("Added Export Control " + index)
        dataModel.insert(index, {name:"New Item", value : "%1".arg(index)})
      }
      onRemoved : {
        console.log("Removed Export Control " + index)
        dataModel.remove(index)
        current = math.Max(0,index-1)
      }

    }
    
    Rectangle {
        Layout.fillHeight: true
        color : "Red"
        border.color:"Red"
    }
  }
}
