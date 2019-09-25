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
        id: propertyEntry
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
      name : "scenario_description"
      label : "Description"
      default_value : 'Enter the scenarios description.'
      required : false
    }

    RestrictionListEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      height :100
    }

    Rectangle {
        Layout.fillHeight: true
        color : "Red"
        border.color:"Red"
    }
  }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
