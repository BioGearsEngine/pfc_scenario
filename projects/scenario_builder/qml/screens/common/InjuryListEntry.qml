import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  signal injuryAdded(int index, int injury_id, double severity, string location, string description)
  signal injuryRemoved(int index, int injury_id, double severity, string location, string description)
  signal locationChanged(int index, string location)
  signal severityChanged(int index, double severity)

  label: "Trauma"
  labelPlaural: "Traumas"
  
  Trauma {
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
       anchors.leftMargin : 5
       anchors.verticalCenter  : parent.verticalCenter 
       width : 100 
       font.weight: Font.Bold
       font.pointSize: 8
       text :  model.medical_name
       color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
    }

    PFCLabel {
      id: injury_location_label
      anchors.left : injury_medical_name_text.right
      anchors.leftMargin : 5
      anchors.verticalCenter  : parent.verticalCenter 
      text: 'Location:'
    }

    TextField {
      id : injury_location_entry
      anchors.left : injury_location_label.right
      anchors.top : parent.top
      anchors.leftMargin : -5
      font.pointSize : 8

      text: model.location
      placeholderText : "Unknown"

      readOnly : false
      activeFocusOnPress: false
      hoverEnabled : false
      enabled : false
      color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        root.locationChanged(index , text)
        if(root.current != index){
          injury.state = "";
        }
      }
    }

    PFCLabel {
     id: injury_severity_label
     anchors.left : injury_location_entry.right
     anchors.leftMargin : 5
     anchors.verticalCenter  : parent.verticalCenter 
     text: 'Severity:'
    }

    TextField {
      id: injury_severity_entry
      anchors.left : injury_severity_label.right
      anchors.top : parent.top
      anchors.leftMargin : -5
      font.pointSize : 8

      text: model.severity
      placeholderText : "Severity Value"
       validator: DoubleValidator {
            bottom:model.min
            top:   model.max
        }

       readOnly : false
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
       onEditingFinished : {
         root.severityChanged(index , text)
        if(root.current != index){
          injury.state = "";
        }
      }
    }

    states: [
      State {
        name : "Selected"
        PropertyChanges{ target : injury_severity_entry; readOnly : false}
        PropertyChanges{ target : injury_severity_entry; activeFocusOnPress : true}
        PropertyChanges{ target : injury_severity_entry; hoverEnabled : true}
        PropertyChanges{ target : injury_severity_entry; enabled : true}
        PropertyChanges{ target : injury_severity_entry; mouseSelectionMode  : TextInput.SelectCharacters }

        PropertyChanges{ target : injury_location_entry; readOnly : false}
        PropertyChanges{ target : injury_location_entry; activeFocusOnPress : true}
        PropertyChanges{ target : injury_location_entry; hoverEnabled : true}
        PropertyChanges{ target : injury_location_entry; enabled  : true}
        PropertyChanges{ target : injury_location_entry; mouseSelectionMode  : TextInput.SelectCharacters }
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

  onList : {
    
  }

  onAdded : {
    //TODO; Model Box Popup with  a selection of known Injuries
    var likely_id = root.backend.nextID(SQLBackend.INJURIES) + 1
    self.injury_id     = -1
    self.medical_name  = "New Trauma %1".arg(likely_id)
    self.common_name   = "New Trauma %1".arg(likely_id)
    self.description   = "New Trauma %1".arg(likely_id)
    self.citations     = ""
    self.min         =  0.0
    self.max         =  0.0
    root.backend.update_injury(self)
    root.model.insert(root.model.count,
      {
        injury_id: self.injury_id
      , medical_name: self.medical_name
      , common_name : self.common_name
      , description: self.description
      , citations: self.citations
      , min: "%1".arg(self.min)
      , max: "%1".arg(self.max)
      , severity: "%1".arg(self.min)
      , location: "Unknown"
      });
    root.injuryAdded(index, self.injury_id, self.min, "Unknown", self.description)
    index++
  }

  onRemoved : {
    self.injury_id =  root.model.get(index).injury_id
    var severity =  root.model.get(index).severity
    var location =  root.model.get(index).location
    root.model.remove(index)
    current = Math.max(0,index-1)
    root.injuryRemoved(index, self.injury_id, severity, location, self.description)
  }
}
