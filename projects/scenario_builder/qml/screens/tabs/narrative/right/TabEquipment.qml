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
  property ListModel model
  property int index // = roles.index
  property int count


  EquipmentMap {
    id : currentEquipmentMap
  }

  function update_role(values) {
    obj.role_id = values.id
    obj.name = values.name
    obj.description = values.description
    root.backend.update_role(obj)
  }

  ListView {
    id : objectsInSceneView
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.margins : 5

    model : ListModel {}

    delegate : SceneProperty {
      Layout.fillWidth : true
      Layout.minimumHeight : 25
      Layout.preferredHeight : 25
      anchors.left : parent.left
      anchors.right : parent.right
      
      object : model
    }
  }

  onIndexChanged : { // role_stack.currentIndex = 1;
    var values = model.get(index);
    if (values && model.count != 0) {
      objectsInSceneView.model.clear()

      currentEquipmentMap.scene.scene_id = root.model.get(root.index).id
      currentEquipmentMap.scene.name = root.model.get(root.index).name
      root.backend.equipment_in_scene(currentEquipmentMap.scene)

      while (root.backend.next_equipment_map(currentEquipmentMap)) {
        console.log("Setting up \"%1\"".arg(currentEquipmentMap.name))
        objectsInSceneView.model.insert(objectsInSceneView.model.count, {
          "id": currentEquipmentMap.role_id,
          "scene_fk": currentEquipmentMap.scene.scene_id,
          "equipment_fk": currentEquipmentMap.equipment.equipment_id,
          "name": "%1".arg(currentEquipmentMap.name),
          "description": "%1".arg(currentEquipmentMap.notes),
          "properties": currentEquipmentMap.equipment.properties,
          "values": currentEquipmentMap.values
        });
      }
    }
  }

  onCountChanged : {
    if (count == 0) {
      contentStack.currentIndex = 0
      while (objectsInSceneView.count > 0) {
        objectsInSceneView.model.remove(0)
      }
    } else {
      indexChanged()
    }
  }
}


  // StackLayout {
  //   id : role_stack
  //   currentIndex : 1
  //   Rectangle {
  //     id : full_listRectangle
  //     width : root.width
  //     height : root.height
  //     Layout.fillWidth : true
  //     Layout.fillHeight : true
  //     Layout.margins : 5

  //     border.color : "black"


  //     FourButtonRow {
  //       id : objectsButtonRow
  //       anchors.top : full_listRectangle.top
  //       anchors.left : full_listRectangle.left
  //       anchors.right : full_listRectangle.right
  //       anchors.topMargin : 2
  //       anchors.rightMargin : 5
  //       anchors.leftMargin : 5

  //       property int next : 1

  //       firstButtonText : "Add"
  //       secondButtonText : "New"
  //       thirdButtonText : "Remove"
  //       fourthButtonText : "Exit"

  //       onFirstButtonClicked : {

  //       }
  //       onSecondButtonClicked : {

  //       }
  //       onThirdButtonClicked : {

  //       }
  //       onFourthButtonClicked : {

  //       }
  //     }

  //     ListView {
  //       id : objectsInScene
  //       anchors {
  //         top : objectsButtonRow.bottom;
  //         bottom : parent.bottom;
  //         left : parent.left;
  //         right : parent.right
  //       }
  //       spacing : 5
  //       clip : true
  //       highlightFollowsCurrentItem : true
  //       highlightMoveDuration : 1
  //       highlight : Rectangle {
  //         color : '#1111110F'
  //         Layout.alignment : Qt.AlignTop
  //         Layout.fillWidth : true
  //         Layout.margins : 5
  //       }

  //       model : ListModel {}

  //       delegate : Rectangle {
  //         id : objectInSceneDelegate
  //         color : 'transparent'
  //         border.color : "steelblue"
  //         height : objectInSceneDelegate_title_text.height + objectInSceneDelegate_value_text.height
  //         anchors {
  //           left : parent.left;
  //           right : parent.right;
  //           margins : 5
  //         }

  //         MouseArea {
  //           anchors.fill : parent
  //           onClicked : {
  //             objectsInScene.currentIndex = index

  //           }
  //         }

  //         Text {
  //           id : objectInSceneDelegate_title_text
  //           anchors.left : objectInSceneDelegate.left
  //           anchors.leftMargin : 5
  //           text : model.name
  //           width : 150
  //           font.weight : Font.Bold
  //           font.pointSize : 10
  //           enabled : false
  //           color : enabled ? Material.primaryTextColor : Material.primaryTextColor
  //         }

  //         Text {
  //           id : objectInSceneDelegate_value_text
  //           anchors.top : objectInSceneDelegate_title_text.bottom
  //           anchors.left : parent.left
  //           anchors.right : parent.right
  //           anchors.leftMargin : 10
  //           font.pointSize : 10
  //           text : model.description
  //           enabled : false
  //           color : enabled ? Material.primaryTextColor : Material.primaryTextColor
  //           elide : Text.ElideRight
  //         }

  //         states : State {
  //           name : "Selected"
  //           PropertyChanges {
  //             target : objectInSceneDelegate_title_text;
  //             enabled : true
  //           }
  //           PropertyChanges {
  //             target : objectInSceneDelegate_value_text;
  //             enabled : true
  //           }
  //         }

  //         onFocusChanged : {
  //           if (objectsInScene.currentIndex == index) {
  //             state = 'Selected';
  //           } else {
  //             state = '';
  //           }
  //         }
  //       }
  //       ScrollBar.vertical : ScrollBar {}
  //     }
    // }




    // StackLayout {
    // id : contentStack
    // Layout.fillWidth : true
    // Layout.fillHeight : true
    // currentIndex : 0
    // Rectangle {
    //     id : listRectangle
    //     Layout.fillWidth : true
    //     Layout.fillHeight : true
    //     Layout.margins : 5

    //     border.color : "black"

    //     ThreeButtonRow {
    //       id : controls
    //       anchors.top : listRectangle.top
    //       anchors.left : listRectangle.left
    //       anchors.right : listRectangle.right
    //       anchors.topMargin : 2
    //       anchors.rightMargin : 5
    //       anchors.leftMargin : 5

    //       property int next : 1

    //       firstButtonText : "Add"
    //       secondButtonText : "New"
    //       thirdButtonText : "Remove"

    //       onFirstButtonClicked : {
    //         if (count == 0) {
    //           return
    //         }
    //         role_stack.currentIndex = 0 full_objectsInScene.model.clear()root.backend.roles()while (root.backend.next_role(currentEquipmentMap)) {
    //           full_objectsInScene.model.insert(full_objectsInScene.model.count, {
    //             "id": currentEquipmentMap.role_id,
    //             "name": "%1".arg(currentEquipmentMap.name),
    //             "description": "%1".arg(currentEquipmentMap.description)
    //           });
    //         }
    //       }
    //       onSecondButtonClicked : {
    //         if (count == 0) {
    //           return
    //         }
    //         if (next < objectsInScene.model.count) {
    //           next = objectsInScene.model.count + 1
    //         }
    //         currentEquipmentMap.role_id = -1 currentEquipmentMap.name = "New Role %1".arg(next);
    //         currentEquipmentMap.description = "Description of Role %1".arg(next);
    //         self_scene.scene_id = root.model.get(root.index).id;
    //         self_scene.name = root.model.get(root.index).name;
    //          while (root.backend.select_role(currentEquipmentMap)) {
    //           next = next + 1
    //           currentEquipmentMap.role_id = -1;
    //           currentEquipmentMap.name = "New Role %1".arg(next);
    //           currentEquipmentMap.description = "Description of Role %1".arg(next)
    //         }

    //         root.backend.update_role_in_scene(self_scene, currentEquipmentMap)objectsInScene.model.insert(objectsInScene.model.count, {
    //           "id": currentEquipmentMap.role_id,
    //           "name": "%1".arg(currentEquipmentMap.name),
    //           "description": "%1".arg(currentEquipmentMap.description)
    //         });
    //         ++next;
    //       }
    //       onThirdButtonClicked : {
    //         if (count == 0) {
    //           return
    //         }
    //         currentEquipmentMap.role_id = objectsInScene.model.get(objectsInScene.currentIndex).role_id;
    //         self.name = objectsInScene.model.get(objectsInScene.currentIndex).name;
    //         self_scene.scene_id = root.model.get(root.index).id;
    //         self_scene.name = root.model.get(root.index).name;
    //         root.backend.remove_role_from_scene(currentEquipmentMap, self_scene);
    //         objectsInScene.model.remove(objectsInScene.currentIndex);
    //         objectsInScene.currentIndex = Math.max(0, objectsInScene.currentIndex - 1);
    //       }
    //     }

    //     ListView {
    //       id : objectsInScene
    //       anchors {
    //         top : controls.bottom;
    //         bottom : parent.bottom;
    //         left : parent.left;
    //         right : parent.right
    //       }
    //       spacing : 5
    //       clip : true
    //       highlightFollowsCurrentItem : true
    //       highlightMoveDuration : 1
    //       highlight : Rectangle {
    //         color : '#1111110F'
    //         // anchors.left : parent.left
    //         // anchors.right : parent.right
    //         // anchors.margins: 5
    //         Layout.alignment : Qt.AlignTop
    //         Layout.fillWidth : true
    //         Layout.margins : 5
    //       }

    //       model : ListModel {}

    //       delegate : Rectangle {
    //         id : role
    //         color : 'transparent'
    //         border.color : "steelblue"
    //         height : role_title_text.height + role_value_text.height
    //         anchors {
    //           left : parent.left;
    //           right : parent.right;
    //           margins : 5
    //         }

    //         MouseArea {
    //           anchors.fill : parent
    //           onClicked : {
    //             objectsInScene.currentIndex = index

    //           }
    //           onDoubleClicked : {
    //             roleEdit.name = objectsInScene.model.get(index).name roleEdit.description = objectsInScene.model.get(index).description roleEdit.roleID = objectsInScene.model.get(index).id contentStack.currentIndex = 1
    //           }
    //         }

    //         Text {
    //           id : role_title_text
    //           anchors.left : role.left
    //           anchors.leftMargin : 5
    //           text : model.name
    //           width : 150
    //           font.weight : Font.Bold
    //           font.pointSize : 10
    //           enabled : false
    //           color : enabled ? Material.primaryTextColor : Material.primaryTextColor
    //         }

    //         Text {
    //           id : role_value_text
    //           anchors.top : role_title_text.bottom
    //           anchors.left : parent.left
    //           anchors.right : parent.right
    //           anchors.leftMargin : 10
    //           font.pointSize : 10
    //           text : description
    //           enabled : false
    //           color : enabled ? Material.primaryTextColor : Material.primaryTextColor
    //           elide : Text.ElideRight
    //         }

    //         states : State {
    //           name : "Selected"
    //           PropertyChanges {
    //             target : role_title_text;
    //             enabled : true
    //           }
    //           PropertyChanges {
    //             target : role_value_text;
    //             enabled : true
    //           }
    //         }

    //         onFocusChanged : {
    //           if (objectsInScene.currentIndex == index) {
    //             state = 'Selected';
    //           } else {
    //             state = '';
    //           }
    //         }
    //       }

    //       ScrollBar.vertical : ScrollBar {}

    //       Component.onCompleted : {
    //         if (model.count == 0) {
    //           return
    //         }
    //         var r_count = backend.role_count();
    //         root.backend.roles()while (root.backend.next_role(currentEquipmentMap)) {

    //           objectsInScene.model.insert(objectsInScene.model.count, {
    //             id: currentEquipmentMap.role_id,
    //             name: "%1".arg(currentEquipmentMap.name),
    //             description: "%1".arg(currentEquipmentMap.description)
    //           });
    //         }
    //       }
    //     }
    // }
    // Rectangle {
    //     Layout.fillWidth : true
    //     Layout.fillHeight : true
    //     Layout.margins : 5
    //     border.color : "black"
    //     border.width : 1
    //     RoleEditPane {
    //       id : roleEdit
    //       anchors.fill : parent
    //       anchors.margins : 2
    //       backend : root.backend
    //       onExit : {
    //         contentStack.currentIndex = 0 role_stack.currentIndex = 1 var values = model.get(index)if (values) {
    //           objectsInScene.model.clear()
    //           self_scene.scene_id = root.model.get(root.index).id
    //           self_scene.name = root.model.get(root.index).name
    //           root.backend.roles_in_scene(self_scene)
    //           while (root.backend.next_role(currentEquipmentMap)) {
    //             objectsInScene.model.insert(objectsInScene.model.count, {
    //               "id": currentEquipmentMap.role_id,
    //               "name": "%1".arg(currentEquipmentMap.name),
    //               "description": "%1".arg(currentEquipmentMap.description)
    //             });
    //           }
    //         }
    //       }
    //       // border.color : "black"
    //     }
    // }
    // }
  
  // }

