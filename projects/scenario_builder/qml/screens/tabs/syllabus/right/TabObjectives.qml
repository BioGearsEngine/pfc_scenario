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

  Citation {
    id : citation_g
  }
  Connections {
    target : backend
    onCitationsChanged : {
      refresh_citations()
    }
  }

  function update_objective(objective) {
    if (objective) {
      root.backend.update_objective(objective)
    }
  }

  function refresh_citations() {
    citationStack.objectiveCitations = []
    let citations = currentObjective.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.objectiveCitations.push(citation_g.make());
      citationStack.objectiveCitations[citationStack.objectiveCitations.length - 1].assign(citations[ii]);
    }
    referenceList.model = citationStack.objectiveCitations;
  }
  function refresh_all_citations() {
    citationStack.allCitations = [];
    let citations = root.backend.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.allCitations.push(citation_g.make());
      citationStack.allCitations[citationStack.allCitations.length - 1].assign(citations[ii]);
    }
    fullReferenceList.model = citationStack.allCitations;
  }
  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : nameEntry
    label : "Name"
    placeholderText : "String Field (128 Characters)"
    text : (currentObjective) ? currentObjective.name : ""
    onEditingFinished : {
      if (text != currentObjective.name) {
        currentObjective.name = text
        update_objective(currentObjective)
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
    text : (currentObjective) ? currentObjective.description : ""
    onEditingFinished : {
      if (text != currentObjective.description) {
        currentObjective.description = text
        update_objective(currentObjective)
      }
    }
  }

  StackLayout {
    id : citationStack
    Layout.fillWidth : true
    Layout.fillHeight : false
    Layout.leftMargin : 5
    Layout.rightMargin : 20

    property var objectiveCitations: []
    property var allCitations: []

    currentIndex : 0
    CrossReferenceCitations {
      id : referenceList
      backend : root.backend
      onList : {
        citationStack.currentIndex = 1;
        refresh_all_citations()
      }
      onCitationAdded : {
        currentObjective.citations.push(citation_g.make());
        currentObjective.citations[currentObjective.citations.length - 1].assign(citation);
        update_objective(currentObjective);
        refresh_citations()
      }
      onCitationRemoved : {
        currentObjective.removeCitation(index);
        refresh_citations()
      }
    }
    ListOfAllCitations {
      id : fullReferenceList
      backend : root.backend

      onCitationCreated : {
        refresh_all_citations()
      }
      onCitationAdded : {
        currentObjective.citations.push(citation_g.make());
        currentObjective.citations[currentObjective.citations.length - 1].assign(citation);
        update_objective(currentObjective);
        refresh_citations();
        citationStack.currentIndex = 0;
      }
      onFullExit : {
        refresh_citations();
        citationStack.currentIndex = 0;
      }
    }
  }
  onCurrentObjectiveChanged : {
    refresh_citations()
  }
  onBackendChanged : {
    if (backend) {
      backend.citationsChanged.connect(refresh_citations);
    }
  }
}