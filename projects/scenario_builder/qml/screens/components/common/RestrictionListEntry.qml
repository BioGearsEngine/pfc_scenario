import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  signal restrictionAdded(int index, int restriction_id)
  signal restrictionRemoved(int index, int restriction_id)

  label: "Restriction"

  Restriction {
    id: self
    restriction_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : restriction
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

     function update_restriction(id) {
         self.restriction_id = id
         self.name    = restriction_name_text.text
         self.value   = restriction_value_text.text

         console.log("%1,%2,%3,%4,%5".arg(self.restriction_id)
          .arg(self.name)
          .arg(self.value))

         root.backend.update_restriction(self)
     }
    
     TextField {
       id : restriction_title_text
       anchors.left : restriction.left
       anchors.leftMargin : 5
       text :  model.name 
       width : 150
       font.weight: Font.Bold
       font.pointSize: 10
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         self.restriction_id = -1
         self.name = restriction_title_text.text
         self.value = restriction_value_text.text
         root.backend.update_restriction(self)
      }
    }

     TextField {
       id : restriction_value_text
       anchors.left : restriction_title_text.right
       anchors.right : parent.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.value
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         self.restriction_id = -1
         self.name = restriction_title_text.text
         self.value = restriction_value_text.text
         root.backend.update_restriction(self)
      }
      }
     

    states: State {
      name : "Selected"
      PropertyChanges{ target : restriction_title_text; readOnly : false}
      PropertyChanges{ target : restriction_title_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_title_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_title_text; enabled : true}
      PropertyChanges{ target : restriction_title_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : restriction_value_text; readOnly : false}
      PropertyChanges{ target : restriction_value_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_value_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_value_text; enabled  : true}
      PropertyChanges{ target : restriction_value_text; mouseSelectionMode  : TextInput.SelectCharacters }
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

  onList : {
    
  }
  onAdded : {
    self.restriction_id = -1
    self.name = "New Restriction %1".arg(root.model.count)
    self.value = "New Restriction%1".arg(index)
    root.backend.update_restriction(self)
    root.model.insert(root.model.count,
      {"name": "%1".arg(self.name), "value": "%1".arg(self.value)});
  }
  onRemoved : {
    self.restriction_id = -1
    self.name = root.model.get(index).name

    root.backend.remove_restriction(self)
    root.model.remove(index)
    current = Math.max(0,index-1)
  }
  onBackendChanged : {
    var r_count = backend.restriction_count();
    root.backend.restrictions()
    while ( root.backend.next_restriction(self) ){
      root.model.insert(root.model.count,
        {"name": "%1".arg(self.name), "value": "%1".arg(self.value)});
    }
  }
}
