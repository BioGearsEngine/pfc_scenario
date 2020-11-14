import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id : root
  property SQLBackend backend

  signal citationAdded(int index, int citation_id)
  signal citationRemoved(int index, int citation_id)

  label : "Reference"

  Citation {
    id : self
    citation_id : -1
  }

  model : ListModel {}

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
      root.backend.update_citation(citation)
    }

    TextField {
      id : citation_key_text
      anchors.left : parent.left
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].key : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_citation(root.model[index])
      }
    }
    TextField {
      id : citation_authors_text
      anchors.left : citation_key_text.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].authors : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_citation(root.model[index])
      }
    }
    TextField {
      id : citation_year_text
      anchors.left : citation_authors_text.right
      anchors.leftMargin : 10
      font.pointSize : 10
      text : (root.model) ? root.model[index].year : ""
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_citation(root.model[index])
      }
    }
    TextField {
      id : citation_title_text
      anchors.left : citation_year_text.right
      anchors.right : parent.right
      anchors.leftMargin : 5
      text : (root.model) ? root.model[index].title : ""
      width : 150
      font.weight : Font.Bold
      font.pointSize : 10
      readOnly : true
      activeFocusOnPress : false
      hoverEnabled : false
      enabled : false
      color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
      onEditingFinished : {
        update_citation(root.model[index])
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
  onAdded : {}
  onRemoved : {}
}
