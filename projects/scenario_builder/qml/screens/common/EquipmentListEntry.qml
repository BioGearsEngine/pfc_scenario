import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  signal equipmentAdded(int index, int equipment_id)
  signal equipmentRemoved(int index, int equipment_id)

  label : "Equipment"
  labelPlaural : "Equipment"
  
  Equipment {
    id: self
    equipment_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : equipment
     color : 'transparent'
     border.color: "steelblue"
     height : equipment_name_text.height + equipment_description_text.height
     anchors { left : parent.left; right: parent.right ; margins : 5 }

     MouseArea {
       anchors.fill: parent
       onClicked: {
        root.current = index
       }
     }

     Text {
       id : equipment_name_text
       anchors.left : parent.left
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.name
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
    }

    Text {
       id : equipment_description_text
       anchors.top : equipment_name_text.bottom
       anchors.left : parent.left 
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.description
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       elide : Text.ElideRight
      }

    }

    //TODO: Modifiy InjurySets to Store a Severity Value and A Location Value for each Entry
    //TODO: Then Displa these values instead of the description.
    //TODO: It would be cool if the delegate on state change added a description field which expanded it

  onAdded : {
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

  onRemoved : {
    self.equipment_id =  root.model.get(index).equipment_id
    root.model.remove(index)
    current = Math.max(0,index-1)
    root.equipmentRemoved(index,self.equipment_id)
  }
}
