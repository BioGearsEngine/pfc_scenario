import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

CrossReferenceForm {
  id : root
  property SQLBackend backend
  property TraumaProfile profile

  signal traumaModified(int index, TraumaOccurence occurence)
  signal traumaAdded(int index, Trauma newTrauma)
  signal traumaRemoved(int index, TraumaOccurence occurence)

  label : "Trauma"
  labelPlaural : "Traumas"
  buttonTwoVisible : false

  Trauma {
    id : self
    trauma_id : -1
  }

  property var notification :     Component {
    id : notifierComponent
    Rectangle {
      id: notifierRect
      property string message 
      property string header : "Error" 
      height : 100
      width :  body.width + 50
      color : "lightslategrey"
      opacity: 1.0
      visible: false;
      radius : 5
      Rectangle {
        color : "white"
        anchors.left : parent.left
        anchors.right : parent.right
        height : title.height
        Text {
          id: title
          text : header
          font.bold: true
          font.pixelSize : 15
        }
      }
      Text {
        id : body
        text : message
        anchors.centerIn : parent
        font.pixelSize : 25
      }
      Timer {
        id: opacityTimer
        interval: 250; running: true; repeat: true
        onTriggered: {
          parent.opacity = parent.opacity - .1
          if ( parent.opacity < 0.1 ) {
            parent.destroy()
          }
        }
      }
    }
  }

  model : ListModel {}

  delegate : Rectangle {
    id : occurence
    color : 'transparent'
    border.color : "steelblue"
    height : 30
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

    Text {
      id : trauma_medical_name_text
      anchors.left : parent.left
      anchors.leftMargin : 5
      anchors.verticalCenter : parent.verticalCenter
      width : 100
      font.weight : Font.Bold
      font.pointSize : 8
      text : (root.model[index]) ? root.model[index].trauma.medicalName : ""
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
    }

    PFCLabel {
      id : trauma_location_label
      anchors.left : trauma_medical_name_text.right
      anchors.leftMargin : 5
      anchors.verticalCenter : parent.verticalCenter
      text : 'Location:'
    }

    TextField {
      id : trauma_location_entry
      anchors.left : trauma_location_label.right
      anchors.top : parent.top
      anchors.leftMargin : -5
      font.pointSize : 8

      text : (root.model[index]) ? root.model[index].location : "Unknown"
      placeholderText : "Unknown"

      readOnly : false
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        root.model[index].location = text ;
        traumaModified(index, root.model[index])
      }
    }

    PFCLabel {
      id : trauma_severity_label
      anchors.left : trauma_location_entry.right
      anchors.leftMargin : 5
      anchors.verticalCenter : parent.verticalCenter
      text : 'Severity:'
    }

    TextField {
      id : trauma_severity_entry
      anchors.left : trauma_severity_label.right
      anchors.top : parent.top
      anchors.leftMargin : -5
      font.pointSize : 8

      text : (root.model[index]) ? "%1".arg(root.model[index].severity / 100.0) : "0.0"
      placeholderText : "Severity Value"
      validator : DoubleValidator {
        bottom : (root.model[index]) ? root.model[index].trauma.min / 100.0: 0.0
        top : (root.model[index]) ? root.model[index].trauma.max / 100.0 : 1.0
      }

      onTextChanged : {
       if (root.model[index] && !acceptableInput) {
          showReasonForFailure(parent, "Severity must be between %1 and %2".arg(root.model[index].trauma.min / 100.0).arg(root.model[index].trauma.max / 100.0))
        } 
      }
      readOnly : false
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
          root.model[index].severity = parseFloat(text) * 100.0;
          traumaModified(index, root.model[index])        
      }
    }


    states : [State {
        name : "Selected"
        PropertyChanges { target : trauma_severity_entry; readOnly : false }
        PropertyChanges { target : trauma_severity_entry; activeFocusOnPress : true }
        PropertyChanges { target : trauma_severity_entry; hoverEnabled : true }
        PropertyChanges { target : trauma_severity_entry; enabled : true }
        PropertyChanges { target : trauma_severity_entry; mouseSelectionMode : TextInput.SelectCharacters }

        PropertyChanges { target : trauma_location_entry; readOnly : false }
        PropertyChanges { target : trauma_location_entry; activeFocusOnPress : true }
        PropertyChanges { target : trauma_location_entry; hoverEnabled : true }
        PropertyChanges { target : trauma_location_entry; enabled : true }
        PropertyChanges { target : trauma_location_entry; mouseSelectionMode : TextInput.SelectCharacters }
      }
    ]

    onFocusChanged : {
      if (root.current == index) {
        state = 'Selected';
      } else {
        state = "";
      }
    }

  }

  onList : {}
  onAdded : {
    var likely_id = root.backend.nextID(SQLBackend.TRAUMAS) + 1;
    self.clear(likely_id);
    root.backend.update_trauma(self);
    root.traumaAdded(root.model.length, self);
  }
  onRemoved : {
    if (root.model[index]) { // self.assign(root.model[index]);
      root.model.splice(index);
      current = Math.max(0, index - 1);
      root.traumaRemoved(index, root.model[index]);
    }
  }
  function showReasonForFailure(anchor, message){
     notifierComponent.createObject(anchor, { "visible" : true , "anchors.top" : anchor.bottom  , "message" : message, z : 200, dim: false})
  }
}
