import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout  {
    id: root
    property SQLBackend backend
    property ListElement model
    
    Layout.fillWidth: true
    Layout.fillHeight: true

    TextEntry {
       Layout.fillWidth: true

      id: nameEntry
      label : "Name"
      placeholderText: "String Field (128 Characters )"
    }

    TextAreaEntry {
      Layout.fillWidth: true

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"
    }
    TextAreaEntry {
      Layout.fillWidth: true

      id: purposeEntry
      label : "Purpose"
      required: true
      placeholderText: "Text Area (5-15 Lines)"
    }

    ListEntry {
      Layout.fillWidth: true
      Layout.fillHeight : true
      label : "Citations"
      model : ListModel {}
      delegate : Rectangle {
        Layout.fillWidth : true
        Layout.preferredHeight : 100
        color : "red"
      }
    }
}
