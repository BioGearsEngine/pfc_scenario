import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../common"

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

  StackLayout {
    id : role_stack
    Rectangle {
      id : full_listRectangle
      width : root.width
      height : root.height
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5

      border.color : "black"

      FourButtonRow {
        id : objectsButtonRow
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

        }
        onSecondButtonClicked : {

        }
        onThirdButtonClicked : {

        }
        onFourthButtonClicked : {

        }
      }

      ListView {
        id : objectsInScene
        anchors {
          top : objectsButtonRow.bottom;
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
          id : objectInSceneDelegate
          color : 'transparent'
          border.color : "steelblue"
          height : objectInSceneDelegate_title_text.height + objectInSceneDelegate_value_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              objectsInScene.currentIndex = index

            }
          }

          Text {
            id : objectInSceneDelegate_title_text
            anchors.left : objectInSceneDelegate.left
            anchors.leftMargin : 5
            text : model.name
            width : 150
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.primaryTextColor
          }

          Text {
            id : objectInSceneDelegate_value_text
            anchors.top : objectInSceneDelegate_title_text.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.leftMargin : 10
            font.pointSize : 10
            text : model.description
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.primaryTextColor
            elide : Text.ElideRight
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : objectInSceneDelegate_title_text;
              enabled : true
            }
            PropertyChanges {
              target : objectInSceneDelegate_value_text;
              enabled : true
            }
          }

          onFocusChanged : {
            if (objectsInScene.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }
    }
  }
  onIndexChanged : {
    role_stack.currentIndex = 1;
    var values = model.get(index);
    if (values && model.count != 0) {
      objectsInScene.model.clear()

      currentEquipmentMap.scene.scene_id = root.model.get(root.index).id
      currentEquipmentMap.scene.name = root.model.get(root.index).name
      root.backend.equipment_in_scene(currentEquipmentMap.scene)

      while (root.backend.next_equipment_map(currentEquipmentMap)) {
        console.log("Setting up \"%1\"".arg(currentEquipmentMap.name))
        objectsInScene.model.insert(objectsInScene.model.count, {
          "id": currentEquipmentMap.role_id,
          "name": "%1".arg(currentEquipmentMap.name),
          "description": "%1".arg(currentEquipmentMap.description)
        });
      }
    }
  }
  onCountChanged : {
    if (count == 0) {
      contentStack.currentIndex = 0
      while (objectsInScene.count > 0) {
        objectsInScene.model.remove(0)
      }
    } else {
      indexChanged()
    }
  }
}
