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
  property Equipment currentEquipment : equipment_g

  signal reloadEquipmentList();

  Equipment {
    id : equipment_g
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
        equipment_g.clear(likely_id);
        root.backend.update_equipment(equipment_g);
        set_current_index(equipment_g.uuid)
      }
      onSecondButtonClicked : {
        if (!equipmentList.equipmentDefinitions || equipmentList.equipmentDefinitions.length < 2) {
          return
        }
        equipment_g.clear();
        equipment_g.assign(equipmentList.equipmentDefinitions[equipmentList.currentIndex]);
        root.backend.remove_equipment(equipment_g);
        update_equipment();
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

      model : ListModel {}

      delegate : Rectangle {
        id : equipment
        property var currentDef: equipmentList.equipmentDefinitions[index]
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
      
      onCurrentIndexChanged : {
        if ( equipmentList.equipmentDefinitions[ currentIndex ] ) {
          root.currentEquipment = equipmentList.equipmentDefinitions[equipmentList.currentIndex];        
        } else {
          root.currentEquipment =  equipment_g;
        }
      }
    }
  }

  Component.onCompleted : {
    update_equipments()
  }

  Connections {
    target : backend
    onEquipmentChanged : {
      update_equipments()
    }
    onEquipmentUpdated : {
      update_equipment(index)
    }
    onCitationsChanged : {
      update_equipment()
    }
  }

  function set_current_index(uuid) {
    for (var ii = 0; ii < equipmentList.equipmentDefinitions.length; ++ ii) {
      if (equipmentList.equipmentDefinitions[ii].uuid == uuid) {
        equipmentList.currentIndex = ii
      }
    }
  }
  function update_equipments() {
    var index = (equipmentList.currentIndex >= 0) ? equipmentList.currentIndex : 0
    equipmentList.equipmentDefinitions = []
    let equipments = root.backend.equipment;
    for (var ii = 0; ii < equipments.length; ++ ii) {
      equipmentList.equipmentDefinitions.push(currentEquipment.make());
      equipmentList.equipmentDefinitions[equipmentList.equipmentDefinitions.length - 1].assign(equipments[ii]);
    }
    equipmentList.model = equipmentList.equipmentDefinitions;
    equipmentList.currentIndex = index
  }
    function update_equipment( id ) {
    for (var ii = 0; ii < equipmentList.equipmentDefinitions.length; ++ ii) {
      if( equipmentList.equipmentDefinitions[ii].equipmnent_id == id ) {
         backend.select_equipment( equipmentList.equipmentDefinitions[ii] );
      }
    }
  }
}
