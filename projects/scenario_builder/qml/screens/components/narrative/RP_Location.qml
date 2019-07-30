import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout  {
    id: root
    property SQLBackend backend
    Layout.fillWidth: true
    Layout.fillHeight: true

    TextEntry {
      Layout.fillWidth: true
      id: nameEntry
      label : "Scene Name"
      placeholderText: "String Field (128 Characters)"
    }

    TextEntry {
      Layout.fillWidth: true
      id: locationNameEntry
      label : "Location Name"
      placeholderText: "String Field (128 Characters)"
    }
    TimeEntry {
      Layout.fillWidth: true
      id: timeOfDayEntry
      label : "Time of Day"
    }
    TextEntry {
      Layout.fillWidth: true
      id: timeScenarioEntry
      label : "Time in Scenario"
      placeholderText: "Time Input Field (3H20M)"
    }
    TextAreaEntry {
      Layout.fillWidth: true
      id: environmentEntry
      label : "Environments"
      required: true
      placeholderText: "Weather Input Area"
    }
}
