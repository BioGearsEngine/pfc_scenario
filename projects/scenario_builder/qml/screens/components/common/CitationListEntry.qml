import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  signal citationAdded(int index, int citation_id)
  signal citationRemoved(int index, int citation_id)

  label: "Reference"
  
  Citation {
    id: self
    citation_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : citation
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

     function update_citation(id) {
         self.citation_id = id
         self.key     = citation_key_text.text
         self.title   = citation_title_text.text
         self.authors = citation_authors_text.text
         self.year    = citation_year_text.text

         console.log("%1,%2,%3,%4,%5".arg(self.citation_id)
          .arg(self.key)
          .arg(self.title)
          .arg(self.authors)
          .arg(self.year))

         root.backend.update_citation(self)
     }

     TextField {
       id : citation_key_text
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
          console.log("%1,%2,%3,%4,%5".arg(model.citation_id)
          .arg(model.key)
          .arg(model.title)
          .arg(model.authors)
          .arg(model.year))
         update_citation(model.citation_id)
      }
    }
    TextField {
       id : citation_authors_text
       anchors.left : citation_key_text.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.authors
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_citation(model.citation_id)
       }
    }

     TextField {
       id : citation_year_text
       anchors.left : citation_authors_text.right
       anchors.leftMargin : 10
       font.pointSize: 10
       text :  model.year
       readOnly : true
       activeFocusOnPress: false
       hoverEnabled : false
       enabled : false
       color: enabled ? Material.primaryTextColor : Material.primaryTextColor
       onEditingFinished : {
         update_citation(model.citation_id)
      }
    }
    TextField {
       id : citation_title_text
       anchors.left : citation_year_text.right
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
         update_citation(model.citation_id)
      }
    }

    states: State {
      name : "Selected"
      PropertyChanges{ target : citation_title_text; readOnly : false}
      PropertyChanges{ target : citation_title_text; activeFocusOnPress : true}
      PropertyChanges{ target : citation_title_text; hoverEnabled : true}
      PropertyChanges{ target : citation_title_text; enabled : true}
      PropertyChanges{ target : citation_title_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : citation_key_text; readOnly : false}
      PropertyChanges{ target : citation_key_text; activeFocusOnPress : true}
      PropertyChanges{ target : citation_key_text; hoverEnabled : true}
      PropertyChanges{ target : citation_key_text; enabled : true}
      PropertyChanges{ target : citation_key_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : citation_authors_text; readOnly : false}
      PropertyChanges{ target : citation_authors_text; activeFocusOnPress : true}
      PropertyChanges{ target : citation_authors_text; hoverEnabled : true}
      PropertyChanges{ target : citation_authors_text; enabled  : true}
      PropertyChanges{ target : citation_authors_text; mouseSelectionMode  : TextInput.SelectCharacters }

      PropertyChanges{ target : citation_year_text; readOnly : false}
      PropertyChanges{ target : citation_year_text; activeFocusOnPress : true}
      PropertyChanges{ target : citation_year_text; hoverEnabled : true}
      PropertyChanges{ target : citation_year_text; enabled  : true}
      PropertyChanges{ target : citation_year_text; mouseSelectionMode  : TextInput.SelectCharacters }
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

  onAdded : {
    var likely_id = root.backend.nextID(SQLBackend.CITATIONS) + 1
    self.citation_id = -1
    self.key = "REF%1".arg(likely_id)
    self.title  = "New Reference %1".arg(likely_id)
    self.authors = "Unknown Author"
    self.year   = 2018
    self.key    = "AuthorYear_%1".arg(likely_id)
    root.backend.update_citation(self)
    root.model.insert(root.model.count,
      {
        citation_id: "%1".arg(self.citation_id)
      , key: "%1".arg(self.key)
      , title : "%1".arg(self.title)
      , authors: "%1".arg(self.authors)
      , year: "%1".arg(self.year)
      });
    root.citationAdded(root.model.count, self.citation_id)
  }
  onRemoved : {
    self.citation_id =  root.model.get(index).citation_id
    root.model.remove(index)
    current = Math.max(0,index-1)
    root.citationRemoved(index, self.citation_id )
  }
}
