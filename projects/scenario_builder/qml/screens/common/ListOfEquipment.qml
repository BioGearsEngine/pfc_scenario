import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListOfForm {
  id : root
  property SQLBackend backend

  signal equipmentAdded(int index, Equipment equipment)
  signal equipmentRemoved(int index, Equipment equipment)

  label : "Equipment"
  labelPlaural : "Equipment"

  Equipment {
    id : self
    equipment_id : -1
  }

  model : ListModel {}

  delegate : Rectangle {
    id : equipment
    color : 'transparent'
    border.color : "steelblue"
    height : equipment_name_text.height + equipment_description_text.height
    anchors {
      left : parent.left;
      right : parent.right;
      margins : 5
    }

    MouseArea {
      anchors.fill : parent
      onClicked : {
        root.current = index
      }
    }

    function update_equipment(equipment) {
      if (equipment) {
        root.backend.update_equipment(equipment)
      }
    }

    Text {
      id : equipment_name_text
      anchors.left : parent.left
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].name : ""
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
    }

    Text {
      id : equipment_description_text
      anchors.top : equipment_name_text.bottom
      anchors.left : parent.left
      anchors.leftMargin : 10
      font.pointSize : 10
      text : {
        if (root.model[index]) {
          if (!enabled) {
            return root.model[index].summary
          } else {
            if (root.model[index].description === "") {
              return root.model[index].summary
            } else {
              return root.model[index].description
            }
          }
        } else {
          return ""
        }
      }
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
    }

    states : State {
      name : "Selected"
      PropertyChanges {
        target : equipment_name_text;
        enabled : true
      }
      PropertyChanges {
        target : equipment_description_text;
        enabled : true
      }
    }

    onFocusChanged : {
      if (current.current == index) {
        state = 'Selected';
      } else {
        state = '';
      }
    }

  }
  onList : {}
  onAdded : {
    var likely_id = root.backend.nextID(SQLBackend.EQUIPMENT) + 1;
    self.clear(likely_id);
    root.backend.update_equipment(self);
    root.equipmentAdded(root.model.length, self)
  }
  onRemoved : {
    if (root.model[index]) {
      self.assign(root.model[index]);
      root.model.splice(index);
      current = Math.max(0, index - 1);
      root.equipmentRemoved(index, self)
    }
  }
}
