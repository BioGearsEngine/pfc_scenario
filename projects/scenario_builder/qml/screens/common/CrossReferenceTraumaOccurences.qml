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
      text : (root.model) ? root.model[index].trauma.medicalName : ""
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

      text : (root.model) ? root.model[index].location : "Unknown"
      placeholderText : "Unknown"

      readOnly : false
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        root.model[index].location = text
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

      text : (root.model) ? "%1".arg(root.model[index].severity) : "0.0"
      placeholderText : "Severity Value"
      validator : DoubleValidator {
        bottom : (root.model) ? root.model[index].trauma.min : 0.0
        top : (root.model) ? root.model[index].trauma.max : 1.0
      }

      readOnly : false
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        root.model[index].severity = "%1".arg(text)
        traumaModified(index, root.model[index])
      }
    }

    states : [State {
        name : "Selected"
        PropertyChanges {
          target : trauma_severity_entry;
          readOnly : false
        }
        PropertyChanges {
          target : trauma_severity_entry;
          activeFocusOnPress : true
        }
        PropertyChanges {
          target : trauma_severity_entry;
          hoverEnabled : true
        }
        PropertyChanges {
          target : trauma_severity_entry;
          enabled : true
        }
        PropertyChanges {
          target : trauma_severity_entry;
          mouseSelectionMode : TextInput.SelectCharacters
        }

        PropertyChanges {
          target : trauma_location_entry;
          readOnly : false
        }
        PropertyChanges {
          target : trauma_location_entry;
          activeFocusOnPress : true
        }
        PropertyChanges {
          target : trauma_location_entry;
          hoverEnabled : true
        }
        PropertyChanges {
          target : trauma_location_entry;
          enabled : true
        }
        PropertyChanges {
          target : trauma_location_entry;
          mouseSelectionMode : TextInput.SelectCharacters
        }
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
    if (root.model[index]) {
      //self.assign(root.model[index]);
      root.model.splice(index);
      current = Math.max(0, index - 1);
      root.traumaRemoved(index, root.model[index]);
    }
  }
}
