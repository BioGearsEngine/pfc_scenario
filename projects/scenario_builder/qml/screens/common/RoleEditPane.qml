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
  property RoleMap currentRoleMap
  property int returnTo

  signal exit()
  Layout.fillWidth : true
  Layout.fillHeight : true

  TextEntry {
    id : name
    Layout.fillWidth : true
    label : "Name"
    text : (currentRoleMap && currentRoleMap.role) ? currentRoleMap.role.name : "unknown"
    placeholderText : "String Field"
  }

  ComboBoxEntry {
    id : category
    Layout.fillWidth : true
    label : "Category"
    currentIndex : (currentRoleMap) ? category.model.find(currentRoleMap.category) : undefined
    model : [
      "UNKNOWN",
      "PATIENT",
      "LEARNER",
      "WHITNESS",
      "ASSISTANT"
    ]
  }

  TextAreaEntry {
    id : description
    Layout.fillWidth : true
    label : "Description"
    text : (currentRoleMap && currentRoleMap.role) ? currentRoleMap.role.description : "unknown"
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
          currentRoleMap.role.name = name.text;
          currentRoleMap.category = category.model[category.currentIndex];
          currentRoleMap.role.description = description.text;
          root.backend.update_role_map(currentRoleMap);
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
