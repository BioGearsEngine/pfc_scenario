import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0


ScrollView {
  id : root

  property SQLBackend backend
  property Event currentEvent
  property Scene currentScene

  property int returnTo
  property var knownRoles

  Role {
    id : role_g
  }
  signal exit()

  onWidthChanged : {
    eventColumn.width = width
  }
  ColumnLayout {
    id : eventColumn
    // anchors.fill : parent
    Layout.fillWidth : true

    TextEntry {
      id : name
      Layout.fillWidth : true
      label : "Name"
      text : (currentEvent) ? currentEvent.name : "unknown"
      placeholderText : "String Field"
    }

    TextAreaEntry {
      id : description
      Layout.fillWidth : true
      label : "Description"
      text : (currentEvent) ? currentEvent.description : "unknown"
      placeholderText : "String Field"
    }


    RowLayout {
      id : actor_one
      Layout.leftMargin : 5
      Label {
        text : "Who Acts"
        font.pointSize : 10
        color : "steelblue"
        Layout.minimumWidth : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : actor_one_select
        width : 200
        textRole : "name"
        model : ListModel {
          ListElement {
            name : "No Actors Defined"
            role_id : 0
          }
        }
        contentItem : Text {
          text : parent.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
    }

    RowLayout {
      id : actor_two
      Layout.leftMargin : 5
      Label {
        text : "Target"
        font.pointSize : 10
        color : "steelblue"
        Layout.minimumWidth : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : actor_two_select
        width : 200
        textRole : "name"
        model : ListModel {
          ListElement {
            name : "No Actors Defined"
            role_id : 0
          }
        }
        contentItem : Text {
          text : parent.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
    }

    TextAreaEntry {
      id : details
      Layout.fillWidth : true
      label : "Details"
      text : (currentEvent) ? currentEvent.details : "unknown"
      placeholderText : "String Field"
    }
    RowLayout {
      id : fidelity
      Layout.leftMargin : 5
      Label {
        text : "Fidelity"
        font.pointSize : 10
        color : "steelblue"
        Layout.minimumWidth : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : fidelitySelect
        width : 200
        model : ["Low", "Medium", "High"]
        currentIndex : {
          let index = (currentEvent) ? fidelitySelect.find(currentEvent.fidelity) : 0;
          fidelitySelect.currentIndex = (index === -1) ? 0 : index;
        }
        contentItem : Text {
          text : fidelitySelect.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
    }
    RowLayout {
      id : type
      Layout.leftMargin : 5
      Label {
        text : "Type"
        font.pointSize : 10
        color : "steelblue"
        width : (text.width > 90) ? text.width + 10 : 100
        Layout.minimumWidth : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : typeSelect
        width : 200
        model : [
          "Action",
          "Dialogue",
          "Movement",
          "Sound",
          "Environment"
        ]
        currentIndex : {
          let index = (currentEvent) ? typeSelect.find(currentEvent.category) : 0;
          typeSelect.currentIndex = (index === -1) ? 0 : index;
        }
        contentItem : Text {
          text : typeSelect.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
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
            currentEvent.name = name.text;
            currentEvent.description = description.text;
            currentEvent.actorOne.assign(root.knownRoles[actor_one_select.currentIndex]);
            currentEvent.actorTwo.assign(root.knownRoles[actor_two_select.currentIndex]);
            currentEvent.details = details.text;
            currentEvent.fidelity = fidelitySelect.model[fidelitySelect.currentIndex].toUpperCase();
            currentEvent.category = typeSelect.model[typeSelect.currentIndex].toUpperCase();
            root.backend.update_event(currentEvent);
            exit();
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
          text : 'Exit'
          onClicked : {
            root.exit()
          }
        }
      }
    }
  }

  Connections {
    target : backend

    onRolesChanged : {
      if (backend && currentScene && currentEvent) {
        refresh_roles()

      }
    }
    onScenesChanged : {
      if (backend && currentScene && currentEvent) {
        refresh_roles()
      }
    }

  }

  onCurrentEventChanged : {
    if (backend && currentScene && currentEvent) {
      refresh_roles()
    }
  }

  function refresh_roles() {
    root.knownRoles = []
    var roleMaps = root.backend.roleMaps;

    for (var ii = 0; ii < roleMaps.length; ++ ii) {
      if (roleMaps[ii].scene.scene_id == currentScene.scene_id) {
        root.knownRoles.push(role_g.make());
        var index = root.knownRoles.length - 1;
        root.knownRoles[index].assign(roleMaps[ii].role);
      }
    }
    actor_one_select.model = root.knownRoles;
    actor_two_select.model = root.knownRoles;

    for (var ii = 0; ii < root.knownRoles.length; ++ ii) {
      console.log(root.knownRoles[ii].role_id)
      if (root.knownRoles[ii].role_id == currentEvent.actorOne.role_id) {
        console.log("Actor 1 set to %1".arg(ii))
        actor_one_select.currentIndex = ii;
      }
      if (root.knownRoles[ii].role_id == currentEvent.actorTwo.role_id) {
        console.log("Actor 1 set to %2".arg(ii))
        actor_two_select.currentIndex = ii;
      }
    }
  }
}
