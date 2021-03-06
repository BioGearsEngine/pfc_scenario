import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

CrossReferenceForm {
  id : root
  property SQLBackend backend

  signal citationAdded(int index, Citation citation)
  signal citationRemoved(int index, Citation citation)

  label : "Reference"

  Citation {
    id : self
    citation_id : -1
  }

  delegate : Rectangle {
    id : citation
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

    function update_citation(citation) {
      if (citation) {
        root.backend.update_citation(citation)
      }
    }

    TextField {
      id : citation_key_text
      anchors.left : parent.left
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model && root.model[index]) ? root.model[index].key : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      selectByMouse : true
      onEditingFinished : {
        if (root.model[index].key != text) {
          root.model[index].key = text
          update_citation(root.model[index])
        }
      }
    }
    TextField {
      id : citation_authors_text
      anchors.left : citation_key_text.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model && root.model[index]) ? root.model[index].authors : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      selectByMouse : true
      onEditingFinished : {
        if (root.model[index].authors != text) {
          root.model[index].authors = text
          update_citation(root.model[index])
        }
      }
    }

    TextField {
      id : citation_year_text
      anchors.left : citation_authors_text.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model && root.model[index]) ? root.model[index].year : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      selectByMouse : true
      onEditingFinished : {
        if (root.model[index].year != text) {
          root.model[index].kyearey = text
          update_citation(root.model[index])
        }
      }
    }
    TextField {
      id : citation_title_text
      anchors.left : citation_year_text.right
      anchors.right : parent.right
      anchors.leftMargin : 5
      text : (root.model && root.model[index]) ? root.model[index].title : ""
      width : 150
      font.weight : Font.Bold
      font.pointSize : 10
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      selectByMouse : true
      onEditingFinished : {
        if (root.model[index].title != text) {
          root.model[index].title = text
          update_citation(root.model[index])
        }
      }
    }

    states : State {
      name : "Selected"
      PropertyChanges {
        target : citation_title_text;
        readOnly : false
      }
      PropertyChanges {
        target : citation_title_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : citation_title_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : citation_title_text;
        enabled : true
      }
      PropertyChanges {
        target : citation_title_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }

      PropertyChanges {
        target : citation_key_text;
        readOnly : false
      }
      PropertyChanges {
        target : citation_key_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : citation_key_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : citation_key_text;
        enabled : true
      }
      PropertyChanges {
        target : citation_key_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }

      PropertyChanges {
        target : citation_authors_text;
        readOnly : false
      }
      PropertyChanges {
        target : citation_authors_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : citation_authors_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : citation_authors_text;
        enabled : true
      }
      PropertyChanges {
        target : citation_authors_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }

      PropertyChanges {
        target : citation_year_text;
        readOnly : false
      }
      PropertyChanges {
        target : citation_year_text;
        activeFocusOnPress : true
      }
      PropertyChanges {
        target : citation_year_text;
        hoverEnabled : true
      }
      PropertyChanges {
        target : citation_year_text;
        enabled : true
      }
      PropertyChanges {
        target : citation_year_text;
        mouseSelectionMode : TextInput.SelectCharacters
      }
    }

    onFocusChanged : {
      if (root.current == index) {
        state = 'Selected';
      } else {
        state = '';
      }
    }
  }

  onList : {}
  onAdded : {
    var likely_id = root.backend.nextID(SQLBackend.CITATIONS) + 1;
    self.clear(likely_id);
    root.backend.update_citation(self);
    root.citationAdded(root.model.length, self)
  }
  onRemoved : {
    if (root.model[index]) {
      self.assign(root.model[index]);
      root.model.splice(index);
      current = Math.max(0, index - 1);
      root.citationRemoved(index, self)
    }
  }
}
