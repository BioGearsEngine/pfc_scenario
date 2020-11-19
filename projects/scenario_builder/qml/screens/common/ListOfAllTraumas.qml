import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListOfAllForm {
  id : root
  property SQLBackend backend

  signal traumaAdded(int index, Trauma trauma)
  signal traumaRemoved(int index, Trauma trauma)
  signal traumaCreated()

  label : "Trauma"
  labelPlural : "Traumas"

  Trauma {
    id : self
    trauma_id : -1
  }

  model : ListModel {}

  delegate : Rectangle {
    id : trauma
    color : Material.color(Material.Pink, Material.Shade100)
    border.color : "steelblue"
    height : trauma_medical_name_text.height + trauma_description_entry.height
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

    function update_trauma(trauma) {
      if (trauma) {
        root.backend.update_trauma(trauma)
      }
    }


    TextField {
      id : trauma_medical_name_text
      anchors.left : parent.left
	  anchors.right : parent.right
      anchors.leftMargin : 10
      // anchors.verticalCenter  : parent.verticalCenter
      width : 100
      font.weight : Font.Bold
      font.pointSize : 10
	  readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      text : (root.model) ? root.model[index].medicalName : ""
      color : enabled ? Material.color(Material.BlueGrey, Material.Shade900) : Material.color(Material.BlueGrey, Material.Shade500)
	  
	  onEditingFinished : {
        update_trauma(root.model[index])
      }
    }

    TextField {
      id : trauma_description_entry
      anchors.left : parent.left
	  anchors.right : parent.right
      anchors.top : trauma_medical_name_text.bottom
      anchors.topMargin : 5
      anchors.leftMargin : 10
      font.pointSize : 10

      text : (root.model) ? root.model[index].description : ""
      placeholderText : "Unknown"

      readOnly : false
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.color(Material.BlueGrey, Material.Shade900) : Material.color(Material.BlueGrey, Material.Shade500)
      onEditingFinished : {
        update_trauma(root.model[index])
      }
    }

    states : [State {
        name : "Selected"
		
		PropertyChanges {
        target : trauma_medical_name_text;
        readOnly : false
      }
      PropertyChanges {
        target : trauma_medical_name_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : trauma_medical_name_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : trauma_medical_name_text;
        enabled : true
      }
      PropertyChanges {
        target : trauma_medical_name_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }
		
        PropertyChanges {
          target : trauma_description_entry;
          readOnly : false
        }
        PropertyChanges {
          target : trauma_description_entry;
          activeFocusOnPress : true
        }
        PropertyChanges {
          target : trauma_description_entry;
          hoverEnabled : true
        }
        PropertyChanges {
          target : trauma_description_entry;
          enabled : true
        }
        PropertyChanges {
          target : trauma_description_entry;
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

  onFullAdded : {
    self.assign(root.model[root.current]);
    root.traumaAdded(root.current, self);
  }
  onFullNew : {
    var likely_id = root.backend.nextID(SQLBackend.TRAUMAS);
    self.clear(likely_id);
    root.backend.update_trauma(self);
    root.traumaCreated();
  }

  onFullDeleted : {
    self.assign(root.model[index]);
    root.model.splice(index);
    current = Math.max(0, index - 1);
    root.backend.remove_trauma(self);
    root.traumaRemoved(index, self);
  }

  onFullExit : {}
}
