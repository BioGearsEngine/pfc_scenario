import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property int topIndex
  property Equipment currentEquipment : (equipmentList.equipmentDefinitions[equipmentList.currentIndex]) ? equipmentList.equipmentDefinitions[equipmentList.currentIndex] : currentEquipment

  signal reloadEquipmentList();

  Equipment {
    id : currentEquipment
  }
  function refresh_equipment() {

    var list = root.backend.getEquipment()
    for (var ii = 0; ii < list.count; ++i) {
      equipmentList.equipmentDefinitions.push(list[i].make())
      equipmentList.equipmentDefinitions[equipmentList.equipmentDefinitions.length - 1].assign(list[i])
    }
    equipmentList.model = list
  }
  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight : true

    border.color : "black"

    TwoButtonRow {
      id : controls
      anchors.top : listRectangle.top
      anchors.left : listRectangle.left
      anchors.right : listRectangle.right
      anchors.topMargin : 2
      anchors.rightMargin : 5
      anchors.leftMargin : 5

      property int next : 1

      firstButtonText : "Add"
      secondButtonText : "Remove"

      onFirstButtonClicked : {
        if (next < root.model.count) {
          next = root.model.count + 1
        }
        currentEquipment.equipment_id = -1;
        currentEquipment.type = "Type %1".arg(next);
        currentEquipment.name = "New Equipment %1".arg(next);
        currentEquipment.summary = "Summary of Equipment %1".arg(next);
        currentEquipment.description = "Description of Equipment %1".arg(next);
        currentEquipment.image = "";
        currentEquipment.citations = "";
        while (root.backend.select_equipment(currentEquipment)) {
          next = next + 1
          currentEquipment.equipment_id = -1;
          currentEquipment.name = "New Equipment %1".arg(next);
          currentEquipment.description = "Description of Equipment %1".arg(next)
        }
        currentEquipment.uuid = "";
        root.backend.update_equipment(currentEquipment);
        root.model.insert(root.model.count, {
          "id": currentEquipment.equipment_id,
          "type": currentEquipment.type, // Change this to not be default later
          "name": "%1".arg(currentEquipment.name),
          "summary": "%1".arg(currentEquipment.summary),
          "description": "%1".arg(currentEquipment.description),
          "citations": currentEquipment.citations,
          "image": currentEquipment.image

        });
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        currentEquipment.equipment_id = -1;
        currentEquipment.name = root.model.get(root.index).name;
        root.backend.remove_equipment(currentEquipment);
        root.model.remove(root.index);
        equipmentList.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : equipmentList
      property var equipmentDefinitions: []
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
        Layout.alignment : Qt.AlignTop
        Layout.fillWidth : true
        Layout.margins : 5
      }

      model : []

      delegate : Rectangle {

        property var currentDef: equipmentList.equipmentDefinitions[index]
        id : equipment
        color : 'transparent'
        border.color : "steelblue"
        height : equipment_title_text.height + equipment_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            equipmentList.currentIndex = index

          }
        }

        Text {
          id : equipment_title_text
          anchors.left : equipment.left
          anchors.leftMargin : 5
          text : (currentDef) ? currentDef.name : ""


          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : equipment_value_text
          anchors.top : equipment_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
            if (currentDef) {
              if (!enabled) {
                return currentDef.summary
              } else {
                if (currentDef.description === "") {
                  return currentDef.summary
                } else {
                  return currentDef.description
                }

              }
            } else {
              return ""
            }
          }
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges {
            target : equipment_title_text;
            enabled : true
          }
          PropertyChanges {
            target : equipment_value_text;
            enabled : true
          }
        }

        onFocusChanged : {
          if (equipmentList.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}

      Component.onCompleted : {
        var r_count = backend.equipment_count();
        root.backend.equipments();
        equipmentList.equipmentDefinitions = [];
        while (root.backend.next_equipment(currentEquipment)) {
          equipmentList.equipmentDefinitions.push(currentEquipment.make())
          equipmentList.equipmentDefinitions[equipmentList.equipmentDefinitions.length - 1].assign(currentEquipment)
        }
        equipmentList.model = equipmentList.equipmentDefinitions
      }
    }
  }
  onTopIndexChanged : {
    refresh_equipment()
  }
  onReloadEquipmentList : {
    refresh_equipment()
  }
  function rebuildEquipments() {
    equipmentList.equipments = []
    let equipments = root.backend.equipments;
    for (var ii = 0; ii < equipments.length; ++ ii) {
      equipmentList.equipments.push(equipment.make());
      equipmentList.equipments[equipmentList.equipments.length - 1].assign(equipments[ii]);
    }
    equipmentList.model = equipmentList.equipments;
  }

  Component.onCompleted : {
    rebuildEquipments()
  }

  onBackendChanged : {
    if (backend) {
      backend.equipmentsChanged.connect(rebuildEquipments)
    }
  }
}
