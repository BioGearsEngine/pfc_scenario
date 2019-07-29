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

    ComboBox {
        id : typeSelect
        width: 200
        model: [ "Pass/Fail", "Partial"]

        contentItem: Text {

            text: typeSelect.displayText
            font.pointSize: 8
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onActivated: {
            optionalArea.currentIndex = index
            console.log("Index accepted")
        }

    }

    StackLayout {
        id : optionalArea
        Layout.fillHeight : true
        Layout.fillWidth: true
        Layout.leftMargin : typeSelect.width
        Layout.rightMargin : 20
        Layout.bottomMargin : 5

        currentIndex : 0
        Rectangle {
          id: passFailInput
          Layout.fillWidth: true
          Layout.fillHeight : true
          Layout.margins : 5
          border.color  : "steelblue"
        }

        Rectangle {
          id: partialInput
          Layout.fillWidth: true
          Layout.fillHeight : true
          Layout.margins : 5
          border.color  : "red"
        }
    }

}
