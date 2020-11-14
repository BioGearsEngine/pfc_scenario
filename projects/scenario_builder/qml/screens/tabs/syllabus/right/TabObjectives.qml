import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Objective currentObjective
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true

  Objective {
    id : obj
  }
  Citation {
    id : citation
  }
  Connections {
    target : backend
    onCitationRemoved : {}
  }

  function update_objective(values) {
    obj.objective_id = values.id
    obj.name = values.name
    obj.description = values.description
    obj.citations = values.citations

    root.backend.update_objective(obj)
  }

  function refresh_citations() {
    var values = root.model.get(root.index)
    if (values) {
      nameEntry.text = values.name
      descriptionEntry.text = values.description
      referenceList.model.clear()
      var citations = values.citations.split(";").filter(x => x);
      for (var i = 0; i < citations.length; ++ i) {
        citation.citation_id = parseInt(citations[i])
        citation.key = ""
        citation.title = ""
        if (root.backend.select_citation(citation)) {
          referenceList.model.insert(referenceList.model.count, {
            citation_id: "%1".arg(citation.citation_id),
            key: "%1".arg(citation.key),
            title: "%1".arg(citation.title),
            authors: "%1".arg(citation.authors),
            year: "%1".arg(citation.year),
            publisher: "%1".arg(citation.publisher)
          });
        }
      };
    }
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : nameEntry
    label : "Name"
    placeholderText : "String Field (128 Characters)"

    onEditingFinished : {
      var entry = root.model.get(root.index);
      if (text != entry.name) {
        entry.name = text
        update_objective(entry)
      }
    }
  }

  TextAreaEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : descriptionEntry
    label : "Description"
    required : true
    placeholderText : "Text Area (5-15 Lines)"

    onEditingFinished : {
      var entry = root.model.get(root.index);
      if (text != entry.description) {
        entry.description = text
        update_objective(entry)
      }
    }
  }
  StackLayout {
    id : listStack
    Layout.fillWidth : true
    Layout.fillHeight : false
    Layout.leftMargin : 5
    Layout.rightMargin : 20

    currentIndex : 0
    CitationListEntry {
      id : referenceList
      backend : root.backend
      onList : {
        listStack.currentIndex = 1
      }
      onCitationAdded : {}
      onCitationRemoved : {}
    }
    FullCitationListEntry {
      id : fullReferenceList
      backend : root.backend

      onFullAdded : {}
      onFullExit : { listStack.currentIndex = 0 }
    }
  }
  onTopIndexChanged : {
    if (topIndex == 1) {
      refresh_citations()
    }
  }
}
