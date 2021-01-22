import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0


ColumnLayout {
  id : root

  Role {
    id : role_g
  }

  TraumaProfile {
    id : profile_g
  }

  property SQLBackend backend
  property RoleMap currentRoleMap
  property int returnTo

  property var knownTraumaProfiles

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
    currentIndex : {
      var index = 0;
      if (currentRoleMap) {
        if (currentRoleMap.category && category.model) {
          for (var ii = 0; ii < category.model.length; ii++) {
            if (category.model[ii] == currentRoleMap.category) {
              return ii;
            }
          }
        }
      }
      return 0
    }
    model : [
      "UNKNOWN",
      "PATIENT",
      "LEARNER",
      "WHITNESS",
      "ASSISTANT"
    ]
  }

  ComboBoxEntry {
    id : traumaProfile
    Layout.fillWidth : true
    label : "Trauma Profile"
    textRole : "name"
    currentIndex : {
      var index = 0;
      if (currentRoleMap) {
        if (currentRoleMap.category && category.model) {
          for (var ii = 0; ii < category.model.length; ii++) {
            if (category.model[ii] == currentRoleMap.category) {
              return ii;
            }
          }
        }
      }
      return 0
    }
    model : ListModel {
      ListElement {
            name : "No Profiles Defined"
            role_id : 0
      }
    }
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
          if ( currentRoleMap.role.name != name.text 
          || currentRoleMap.role.description != description.text
          || currentRoleMap.role.traumaProfile.trauma_profile_id != traumaProfile.model[traumaProfile.currentIndex].trauma_profile_id
          || currentRoleMap.role.category != category.model[category.currentIndex]
          ){
            currentRoleMap.role.name = name.text;
            currentRoleMap.role.description = description.text;
            currentRoleMap.role.category = category.model[category.currentIndex];
            currentRoleMap.role.traumaProfile.assign(traumaProfile.model[traumaProfile.currentIndex])

            root.backend.update_role(currentRoleMap.role);
          }
          if ( currentRoleMap.category != category.model[category.currentIndex] ){
            currentRoleMap.category = category.model[category.currentIndex];
            root.backend.update_role_map(currentRoleMap);
          } 
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

  Component.onCompleted : {
      if (backend) {
        refresh_trauma_profiles()
      }
  }

  Connections {
    target : backend
    
    onTraumaProfilesChanged : {
      if (backend) {
        refresh_trauma_profiles()
      }
    }
  }

  //You will see this pattern several times in the codebase
  //I could not come up with a thread safe way
  //TO request a subset list so I have to get all the roles and iterate over them 
  //By Relevance.  I tired a few Q_INVOKABLES but returning a Q_LIST was beyond my skill
  function refresh_trauma_profiles() {
    root.knownTraumaProfiles = []
    var traumaProfiles = root.backend.traumaProfiles;

    for (var ii = 0; ii < traumaProfiles.length; ++ ii) {
        root.knownTraumaProfiles.push(profile_g.make());
        var index = root.knownTraumaProfiles.length - 1;
        root.knownTraumaProfiles[index].assign(traumaProfiles[ii]);
    }
    traumaProfile.model = root.knownTraumaProfiles;
    if ( currentRolMap) {
      for (var ii = 0; ii < root.knownTraumaProfiles.length; ++ ii && currentRoleMap ) {
        if (root.knownTraumaProfiles[ii].role_id == currentRoleMap.role.traumaProfile.trauma_profile_id) {
          trauma_profile_select.currentIndex = ii;
          return;
        }
      }
    }
  }
}
