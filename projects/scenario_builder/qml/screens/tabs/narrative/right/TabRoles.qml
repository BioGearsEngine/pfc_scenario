import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation
  Role {
    id : role_g
  }
  Scene {
    id : scene_g
  }
  RoleMap {
    id : roleMap_g
  }

  function refresh_role_list() {
    rolesInSceneList.rolesMap = []
    let l_maps = root.backend.roleMaps;

    for (var ii = 0; ii < l_maps.length; ii++ ) {
      if (l_maps[ii].scene.scene_id == currentScene.scene_id) {      
        rolesInSceneList.rolesMap.push(roleMap_g.make());

        var index = rolesInSceneList.rolesMap.length - 1;
        rolesInSceneList.rolesMap[index].assign(l_maps[ii]);
      }
    }
    rolesInSceneList.model = rolesInSceneList.rolesMap
  }

  function refresh_full_role_list() {
    allRolesList.roles = []
    var roles = root.backend.roles;
    for (var ii = 0; ii < roles.length; ++ ii) {
      var index = allRolesList.roles.length;
      allRolesList.roles.push(role_g.make());
      allRolesList.roles[index].assign(roles[ii]);
    }
    allRolesList.model = allRolesList.roles
  }

  function update_role(role) {
    root.backend.update_role(role)
  }
  StackLayout {
    id : role_stack
    currentIndex : 1
    Rectangle {
      id : full_listRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      color : "transparent"
      border.color : "black"

      FourButtonRow {
        id : full_controls
        anchors.top : full_listRectangle.top
        anchors.left : full_listRectangle.left
        anchors.right : full_listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5

        property int next : 1

        firstButtonText : "Add"
        secondButtonText : "New"
        thirdButtonText : "Remove"
        fourthButtonText : "Exit"

        onFirstButtonClicked : {
          root.backend.update_role_in_scene(currentScene, 
                                           allRolesList.roles[allRolesList.currentIndex]);
          role_stack.currentIndex = 1
        }
        onSecondButtonClicked : {
          var likely_id = root.backend.nextID(SQLBackend.ROLES);
          role_g.clear(likely_id);
          var index = allRolesList.roles.length
          root.backend.update_role(role_g);
          allRolesList.currentIndex = index
        }
        onThirdButtonClicked : {
          if (!allRolesList.roles || allRolesList.roles.length < 2) {
            return
          }
          root.backend.remove_role(allRolesList.roles[allRolesList.currentIndex])
          refresh_full_role_list();
        }
        onFourthButtonClicked : {
          role_stack.currentIndex = 1
        }
      }

      ListView {
        id : allRolesList
        property var roles: []
        anchors {
          top : full_controls.bottom;
          bottom : parent.bottom;
          left : parent.left;
          right : parent.right
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }

        model : ListModel {}

        delegate : Rectangle {
          id : full_role
          color : Material.color(Material.BlueGrey)
          border.color : "steelblue"
          height : full_role_title_text.height + full_role_value_text.height + full_role_trauma_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              allRolesList.currentIndex = index
            }
            onDoubleClicked : {
              root.backend.update_role_in_scene(currentScene, 
                                                allRolesList.roles[allRolesList.currentIndex]);
              role_stack.currentIndex = 1;
            }
          }

          Label {
            id : full_role_title_label
            anchors.left : parent.left
            anchors.leftMargin : 5
            text : "Name: "
            width : 50
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.color(Material.Grey) : Material.secondaryTextColor
          }
          Text {
            id : full_role_title_text
            anchors.left : full_role_title_label.right
            anchors.leftMargin : 5
            text : (allRolesList.roles[index]) ? allRolesList.roles[index].name : "Name of the Role"

            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : full_role_value_label
            anchors.top : full_role_title_text.bottom
            anchors.left : parent.left
            anchors.leftMargin : 5
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Description: "
            enabled : false
            color : enabled ? Material.color(Material.Grey) : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Text {
            id : full_role_value_text
            anchors.left : full_role_value_label.right
            anchors.right : parent.right
            anchors.top : full_role_value_label.top
            anchors.leftMargin : 5
            text : (allRolesList.roles[index]) ? allRolesList.roles[index].description : "Description of the role"
            wrapMode : Text.WordWrap
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : full_role_trauma_label
            anchors.top : full_role_value_text.bottom
            anchors.left : parent.left
            anchors.leftMargin : 5
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Trauma Profile: "
            enabled : false
            color : enabled ? Material.color(Material.Grey) : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Text {
            id : full_role_trauma_text
            anchors.left : full_role_trauma_label.right
            anchors.top : full_role_trauma_label.top
            anchors.leftMargin : 5
            text : (allRolesList.roles[index]) ? allRolesList.roles[index].trauma_profile.name : "Description of the role"
            wrapMode : Text.WordWrap
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : full_role_title_text;
              enabled : true
            }
            PropertyChanges {
              target : full_role_value_text;
              enabled : true
            }
            PropertyChanges {
              target : full_role_trauma_text;
              enabled : true
            }
            PropertyChanges {
              target : full_role_title_label;
              enabled : true
            }
            PropertyChanges {
              target : full_role_value_label;
              enabled : true
            }
            PropertyChanges {
              target : full_role_trauma_label;
              enabled : true
            }
          }

          onFocusChanged : {
            if (allRolesList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }
    }
    Rectangle {
      id : listRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5

      border.color : "black"

      ThreeButtonRow {
        id : controls
        anchors.top : listRectangle.top
        anchors.left : listRectangle.left
        anchors.right : listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5

        property int next : 1

        firstButtonText : "Add"
        secondButtonText : "New"
        thirdButtonText : "Remove"

        onFirstButtonClicked : {
          refresh_full_role_list();
          role_stack.currentIndex = 0
        }
        onSecondButtonClicked : {
          var index = rolesInSceneList.rolesMap.length 
          var likely_id = root.backend.nextID(SQLBackend.ROLES);
          role_g.clear(likely_id);
          root.backend.update_role(role_g);
          root.backend.update_role_in_scene(currentScene, role_g)
          
          roleMap_g.clear();
          roleMap_g.role.assign(role_g); 
          roleMap_g.scene.assign(currentScene);

          root.backend.select_role_map(roleMap_g)
          rolesInSceneList.currentIndex = index

        }
        onThirdButtonClicked : {
          if (!rolesInSceneList.rolesMap || rolesInSceneList.rolesMap.length < 2) {
            return
          }
          role_g.clear();
          role_g.assign(rolesInSceneList.rolesMap[rolesInSceneList.currentIndex].role);
          root.backend.remove_role_from_scene(role_g, currentScene);
          var prevIndex = rolesInSceneList.currentIndex;
          refresh_role_list();
          rolesInSceneList.currentIndex = Math.max(0, prevIndex);
        }
      }

      ListView {
        id : rolesInSceneList
        property var rolesMap: []
        anchors {
          top : controls.bottom;
          bottom : parent.bottom;
          left : parent.left;
          right : parent.right
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          // anchors.left : parent.left
          // anchors.right : parent.right
          // anchors.margins: 5
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }

        model : ListModel {}

        delegate : Rectangle {
          id : role
          color : 'transparent'
          border.color : "steelblue"
          height : role_title_text.height + role_value_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              rolesInSceneList.currentIndex = index

            }
            onDoubleClicked : {
              roleEdit.returnTo = 1;
              roleEdit.currentRoleMap = rolesInSceneList.rolesMap[rolesInSceneList.currentIndex];
              role_stack.currentIndex = 2;
            }
          }
          Label {
            id : role_title_label
            anchors.left : parent.left
            anchors.leftMargin : 5
            text : "Name: "
            width : 50
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.accentColor : Material.secondaryTextColor
          }
          Text {
            id : role_title_text
            anchors.left : role_title_label.right
            anchors.leftMargin : 5
            text : (rolesInSceneList.rolesMap[index]) ? rolesInSceneList.rolesMap[index].role.name : "Undefined"

            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : role_value_label
            anchors.top : role_title_text.bottom
            anchors.left : parent.left
            anchors.leftMargin : 5
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Description: "
            enabled : false
            color : enabled ? Material.accentColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Text {
            id : role_value_text
            anchors.top : role_title_text.bottom
            anchors.left : role_value_label.right
            anchors.right : parent.right

            anchors.leftMargin : 2
            font.pointSize : 10
            text : (rolesInSceneList.rolesMap[index]) ? rolesInSceneList.rolesMap[index].role.description : "Undefined"
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : role_title_text;
              enabled : true
            }
            PropertyChanges {
              target : role_value_text;
              enabled : true
            }
            PropertyChanges {
              target : role_title_label;
              enabled : true
            }
            PropertyChanges {
              target : role_value_label;
              enabled : true
            }
          }

          onFocusChanged : {
            if (rolesInSceneList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }

        ScrollBar.vertical : ScrollBar {}

        Component.onCompleted : {}
      }
    }
    Rectangle {
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      border.color : "black"
      border.width : 1
      RoleEditPane {
        id : roleEdit
        anchors.fill : parent
        anchors.margins : 2
        backend : root.backend
        onExit : {
          role_stack.currentIndex = roleEdit.returnTo
        }
        // border.color : "black"
      }
    }

  }
  onCurrentSceneChanged : {
    refresh_role_list()
  }

  Connections {
    target : backend
    onRolesChanged : {
       refresh_role_list()
       refresh_full_role_list()
    }

    onRoleMapsChanged : {
       refresh_role_list()
    }
  }
}
