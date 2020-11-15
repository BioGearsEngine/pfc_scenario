import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListOfAllForm {
  id : root
  property SQLBackend backend

  signal equipmentAdded(int index, Equipment equipment)
  signal equipmentRemoved(int index, Equipment equipment)
  signal equipmentCreated()

  label : "Equipment"
  labelPlural : "Equipment"

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

    TextField {
      id : equipment_name_text
      anchors.left : parent.left
      anchors.right : parent.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].name : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_equipment(root.model[index])
      }
    }
    TextField {
      id : equipment_description_text
      anchors.top : equipment_name_text.bottom
      anchors.left : parent.left
      anchors.right : parent.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].description : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_equipment(root.model[index])
      }
    }

    states : State {
      name : "Selected"
      PropertyChanges {
        target : equipment_name_text;
        readOnly : false
      }
      PropertyChanges {
        target : equipment_name_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : equipment_name_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : equipment_name_text;
        enabled : true
      }
      PropertyChanges {
        target : equipment_name_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }

      PropertyChanges {
        target : equipment_description_text;
        readOnly : false
      }
      PropertyChanges {
        target : equipment_description_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : equipment_description_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : equipment_description_text;
        enabled : true
      }
      PropertyChanges {
        target : equipment_description_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }
    }

    onFocusChanged : {
      if (root.current == index) {
        state = 'Selected';
      } else {
        state = '';
      }
    }
  }

  onFullAdded : {
    self.assign(root.model[root.current]);
    root.equipmentAdded(root.current, self);
  }
  onFullNew : {
    var likely_id = root.backend.nextID(SQLBackend.EQUIPMENTS);
    self.clear(likely_id);
    root.backend.update_equipment(self);
    root.equipmentCreated();
  }
  onFullDeleted : {
    self.assign(root.model[index]);
    root.model.splice(index);
    current = Math.max(0, index - 1);
    root.backend.remove_equipment(self);
    root.equipmentRemoved(index, self);
  }
  onFullExit : {}
}
