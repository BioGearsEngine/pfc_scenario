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
        var likely_id = root.backend.nextID(SQLBackend.EQUIPMENTS);
        currentEquipment.clear(likely_id);
        root.backend.update_equipment(currentEquipment);
        equipmentList.equipmentDefinitions.push(currentEquipment.make());
        equipmentList.equipmentDefinitions[equipmentList.equipmentDefinitions.length - 1].assign(currentEquipment);
        equipmentList.model = equipmentList.equipmentDefinitions;
        equipmentList.currentIndex = equipmentList.equipmentDefinitions.length - 1
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        currentEquipment.clear();
        currentEquipment.name = root.model.get(root.index).name;
        root.backend.remove_equipment(currentEquipment);
        rebuildObjectives();
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

    }
  }

  function rebuildEquipments() {
    equipmentList.equipmentDefinitions = []
    let equipments = root.backend.equipment;
    for (var ii = 0; ii < equipments.length; ++ ii) {
      equipmentList.equipmentDefinitions.push(currentEquipment.make());
      equipmentList.equipmentDefinitions[equipmentList.equipmentDefinitions.length - 1].assign(equipments[ii]);
    }
    equipmentList.model = equipmentList.equipmentDefinitions;
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
