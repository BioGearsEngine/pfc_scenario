import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  signal injuryAdded(int injury_id)
  signal injuryRemoved(int injury_id)

  label: "Injury"
  labelPlaural: "Injuries"
  
  Injury {
    id: self
    injury_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : injury
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

     Text {
       id : injury_medical_name_text
       anchors.left : parent.left
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.medical_name
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
    }

    Text {
       id : injury_description_text
       anchors.left : injury_medical_name_text.right
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
    //TODO; Model Box Popup with  a selection of known Injuries
    root.injuryAdded(index)
  }

  onRemoved : {
    self.injury_id =  root.model.get(index).injury_id
    root.model.remove(index)
    current = Math.max(0,index-1)
    root.injuryRemoved(index)
  }
}
