import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout  {
    id: root
    property SQLBackend backend
    focus: true
    Layout.fillWidth: true
    Layout.fillHeight: true

    TextEntry {
      Layout.fillWidth: true
      id: medicalNameEntry
      label : "Medical Name"
      placeholderText: "String Field (128 Characters )"
    }

    TextEntry {
      Layout.fillWidth: true
      id: commonNameEntry
      label : "Common Name"
      placeholderText: "String Field (128 Characters )"
    }

    TextAreaEntry {
      Layout.fillWidth: true

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"
    }
    ListEntry {
      Layout.fillWidth: true
      Layout.fillHeight : true
      label : "Equipment"
      model : ListModel {}
      delegate : Rectangle {
        Layout.fillWidth : true
        Layout.preferredHeight : 100
        color : "red"
      }
    }
    
    ListEntry {
      Layout.fillWidth: true
      Layout.fillHeight : true
      label : "References"
      model : ListModel {}
      delegate : Rectangle {
        Layout.fillWidth : true
        Layout.preferredHeight : 100
        color : "red"
      }
    }
}
