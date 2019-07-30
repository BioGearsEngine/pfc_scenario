import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
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

    ListEntry {
      Layout.fillWidth: true
      label : "References"
      model : ListModel {
          ListElement{name: "item1"}
          ListElement {name: "item2"}
      }
      delegate : Rectangle {
        Layout.fillWidth : true
        Layout.preferredHeight : 100
        color : "red"
      }
    }


    TextAreaEntry {
      Layout.fillWidth: true

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"
    }

    TextEntry {
       Layout.fillWidth: true

      id: severityEntry
      label : "Severity Range"
      placeholderText: "TODO: Make a Spinner Box"
    }
  }
