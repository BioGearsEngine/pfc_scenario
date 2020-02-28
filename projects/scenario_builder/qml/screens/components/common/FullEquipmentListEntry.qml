import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

FullListEntry {
  id: root
  property SQLBackend backend

  signal equipmentAdded(int index, int equipment_id)
  signal equipmentRemoved(int index, int equipment_id)

  label: "Reference"
  
  Equipment {
    id: self
    equipment_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : equipment
     color : 'transparent'
     border.color: "steelblue"
     height : 30
     anchors { left : parent.left; right: parent.right ; margins : 5 }

     MouseArea {
       anchors.fill: parent
       onClicked: {
        root.current = index
       }
     }

     function update_equipment(id) {
         self.equipment_id = id
         self.name     = equipment_name_text.text
         self.description   = equipment_description_text.text
         root.backend.update_equipment(self)
     }

     TextField {
       id : equipment_name_text
       anchors.left : parent.left
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.name
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_equipment(model.equipment_id)
      }
    }
    TextField {
       id : equipment_description_text
       anchors.left : equipment_name_text.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.description
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_equipment(model.equipment_id)
       }
    }

    states: State {
      name : "Selected"
      PropertyChanges{ target : equipment_name_text; readOnly : false}
      PropertyChanges{ target : equipment_name_text; activeFocusOnPress : true}
      PropertyChanges{ target : equipment_name_text; hoverEnabled : true}
      PropertyChanges{ target : equipment_name_text; enabled : true}
      PropertyChanges{ target : equipment_name_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : equipment_description_text; readOnly : false}
      PropertyChanges{ target : equipment_description_text; activeFocusOnPress : true}
      PropertyChanges{ target : equipment_description_text; hoverEnabled : true}
      PropertyChanges{ target : equipment_description_text; enabled  : true}
      PropertyChanges{ target : equipment_description_text; mouseSelectionMode  : TextInput.SelectCharacters }
    }

    onFocusChanged: {
      if(root.current == index){
        state = 'Selected';
      }
      else{
        state = '';
      }
    }
  }

  onFullAdded : {

  }
  onFullNew : {
    var likely_id = root.backend.nextID(SQLBackend.EQUIPMENTS) + 1
    self.equipment_id = -1
    self.type = "1"
    self.name  = "New Equipment %1".arg(likely_id)
    self.description = "New Equipment %1".arg(likely_id)
    self.citations   = ""
    self.image    = ""
    root.backend.update_equipment(self)
    root.model.insert(root.model.count,
      {
        equipment_id: "%1".arg(self.equipment_id)
      , type: "%1".arg(self.type)
      , name : "%1".arg(self.name)
      , description: "%1".arg(self.description)
      , citations: "%1".arg(self.citations)
      , image: "%1".arg(self.image)
      });
    root.equipmentAdded (root.model.count, self.equipment_id)
  }
  onFullDeleted : {

  }
  onFullExit : {

  }
}