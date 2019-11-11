import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

FullListEntry {
  id: root
  property SQLBackend backend

  signal restrictionAdded(int index, int restriction_id)
  signal restrictionRemoved(int index, int restriction_id)

  label: "Reference"
  
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
         self.key     = restriction_key_text.text
         self.title   = restriction_title_text.text
         self.authors = restriction_authors_text.text
         self.year    = restriction_year_text.text

         console.log("%1,%2,%3,%4,%5".arg(self.restriction_id)
          .arg(self.key)
          .arg(self.title)
          .arg(self.authors)
          .arg(self.year))

         root.backend.update_restriction(self)
     }

     TextField {
       id : restriction_key_text
       anchors.left : parent.left
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.key
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
          console.log("%1,%2,%3,%4,%5".arg(model.restriction_id)
          .arg(model.key)
          .arg(model.title)
          .arg(model.authors)
          .arg(model.year))
         update_restriction(model.restriction_id)
      }
    }
    TextField {
       id : restriction_authors_text
       anchors.left : restriction_key_text.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.authors
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_restriction(model.restriction_id)
       }
    }

     TextField {
       id : restriction_year_text
       anchors.left : restriction_authors_text.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.year
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_restriction(model.restriction_id)
      }
    }
    TextField {
       id : restriction_title_text
       anchors.left : restriction_year_text.right
       anchors.right : parent.right
       anchors.leftMargin : 5
       text :  model.title 
       width : 150
       font.weight: Font.Bold
       font.pointSize: 10
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_restriction(model.restriction_id)
      }
    }

    states: State {
      name : "Selected"
      PropertyChanges{ target : restriction_title_text; readOnly : false}
      PropertyChanges{ target : restriction_title_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_title_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_title_text; enabled : true}
      PropertyChanges{ target : restriction_title_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : restriction_key_text; readOnly : false}
      PropertyChanges{ target : restriction_key_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_key_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_key_text; enabled : true}
      PropertyChanges{ target : restriction_key_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : restriction_authors_text; readOnly : false}
      PropertyChanges{ target : restriction_authors_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_authors_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_authors_text; enabled  : true}
      PropertyChanges{ target : restriction_authors_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : restriction_year_text; readOnly : false}
      PropertyChanges{ target : restriction_year_text; activeFocusOnPress : true}
      PropertyChanges{ target : restriction_year_text; hoverEnabled : true}
      PropertyChanges{ target : restriction_year_text; enabled  : true}
      PropertyChanges{ target : restriction_year_text; mouseSelectionMode  : TextInput.SelectCharacters }
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
    var likely_id = root.backend.nextID(SQLBackend.RESTRICTIONS) + 1
    self.restriction_id = -1
    self.key = "REF%1".arg(likely_id)
    self.title  = "New Reference %1".arg(likely_id)
    self.authors = "Unknown Author"
    self.year   = 2018
    self.key    = "AuthorYear_%1".arg(likely_id)
    root.backend.update_restriction(self)
    root.model.insert(root.model.count,
      {
        restriction_id: "%1".arg(self.restriction_id)
      , key: "%1".arg(self.key)
      , title : "%1".arg(self.title)
      , authors: "%1".arg(self.authors)
      , year: "%1".arg(self.year)
      });
    root.restrictionAdded(root.model.count, self.restriction_id)
  }
  onFullDeleted : {

  }
  onFullExit : {

  }
}