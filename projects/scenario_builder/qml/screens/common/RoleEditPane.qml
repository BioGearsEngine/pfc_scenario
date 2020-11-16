import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0


ColumnLayout {
  id : root

  Role {
    id : self
  }
  property SQLBackend backend
  property Role currentRole
  property int returnTo

  signal exit()
  Layout.fillWidth : true
  Layout.fillHeight : true

  TextEntry {
    id : name
    Layout.fillWidth : true
    label : "Name"
    text : (currentRole) ? currentRole.name : "unknown"
    placeholderText : "String Field"
  }
  TextEntry {
    id : codeName
    Layout.fillWidth : true
    label : "Identifier"
    text : (currentRole) ?currentRole.codeName: "unknown"
    placeholderText : "String Field"
  }
  TextAreaEntry {
    id : description
    Layout.fillWidth : true
    label : "Description"
    text : (currentRole) ?currentRole.description: "unknown"
    placeholderText : "String Field"
  }
  Row {
    Layout.fillWidth : true
    Rectangle {
      id : section_1
      width : 300
      height : description.height
      color : 'transparent'
      PFCButton {
        id : saveButton
        anchors.right : section_1.right
        text : 'Save'
        onClicked : {
          currentRole.name = name.text
          currentRole.codeName = codeName.text
          currentRole.description = description.text
          root.backend.update_role(currentRole);
          exit()
        }
      }
    }
    Rectangle {
      id : section_2
      width : 300
      height : description.height
      color : 'transparent'
      PFCButton {
        id : exitButton
        anchors.right : section_2.right
        text : 'Cancel'
        onClicked : {
          exit();
        }
      }
    }
  }
}
