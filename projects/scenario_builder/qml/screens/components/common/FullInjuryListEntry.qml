import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

FullListEntry {
  id: root
  property SQLBackend backend

  signal injuryAdded(int index, int injury_id, double severity, string location)
  signal injuryRemoved(int index, int injury_id, double severity, string location)
  signal locationChanged(int index, string location)
  signal severityChanged(int index, double severity)

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
     height : injury_medical_name_text.height + injury_description_entry.height
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
       anchors.leftMargin : 5
       //anchors.verticalCenter  : parent.verticalCenter 
       width : 100 
       font.weight: Font.Bold
       font.pointSize: 8
       text :  model.medical_name
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
    }

    TextField {
      id : injury_description_entry
      anchors.left : parent.left
      anchors.top : injury_medical_name_text.bottom
      anchors.topMargin : 5
      anchors.leftMargin : 5
      font.pointSize : 8

      text: model.description
      placeholderText : "Unknown"

      readOnly : false
      activeFocusOnPress: false
      hoverEnabled : false
      enabled : false
      color: enabled ? Material.primaryTextColor : Material.primaryTextColor
      onEditingFinished : {
        root.locationChanged(index , text)
        if(root.current != index){
          injury.state = "";
        }
      }
    }

    states: [
      State {
        name : "Selected"

        PropertyChanges{ target : injury_description_entry; readOnly : false}
        PropertyChanges{ target : injury_description_entry; activeFocusOnPress : true}
        PropertyChanges{ target : injury_description_entry; hoverEnabled : true}
        PropertyChanges{ target : injury_description_entry; enabled  : true}
        PropertyChanges{ target : injury_description_entry; mouseSelectionMode  : TextInput.SelectCharacters }
      }
    ]

    onFocusChanged: {
      if(root.current == index){
        state = 'Selected';
      }
      else{
        state = "";
      }
    }

  }

  //TODO: Modifiy InjurySets to Store a Severity Value and A Location Value for each Entry
  //TODO: Then Displa these values instead of the description.
  //TODO: It would be cool if the delegate on state change added a description field which expanded it

  onFullAdded : {
    console.log("FULLADDED")
  }
  onFullNew : {
    //TODO; Model Box Popup with  a selection of known Injuries
    console.log("!!!!")
    var likely_id = root.backend.nextID(SQLBackend.INJURIES) + 1
    self.injury_id     = -1
    self.medical_name  = "New Injury %1".arg(likely_id)
    self.common_name   = "New Injury %1".arg(likely_id)
    self.description   = "New Injury %1".arg(likely_id)
    self.citations     = ""
    self.min         =  0.0
    self.max         =  0.0
    root.backend.update_injury(self)
    root.model.insert(root.model.count,
      {
        injury_id: "%1".arg(self.injury_id)
      , medical_name: self.medical_name
      , common_name : self.common_name
      , description: self.description
      , citations: self.citations
      , min: "%1".arg(self.min)
      , max: "%1".arg(self.max)
      , severity: "%1".arg(self.min)
      , location: "Unknown"
      });
    root.injuryAdded(index, self.injury_id, self.min, "Unknown")
    console.log("!!!!")
  }

  onFullDeleted : {

  }

  onFullExit : {

  }
}
