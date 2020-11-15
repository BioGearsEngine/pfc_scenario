import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Trauma currentTrauma
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true
  Layout.fillHeight : true

  Citation {
    id : citation_g
  }

  Connections {
    target : backend
    onCitationRemoved : {}
    onEquipmentRemoved : {}
  }

  function update_trauma(citation) {
    if (citation) {
      root.backend.update_trauma(citation)
    }
  }

  function refresh_citations() {
    citationStack.treatmentCitations = []
    let citations = currentTrauma.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.treatmentCitations.push(citation_g.make());
      citationStack.treatmentCitations[citationStack.treatmentCitations.length - 1].assign(citations[ii]);
    }
    referenceList.model = citationStack.treatmentCitations;
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

    id : medicalNameEntry
    label : "Medical Name"
    placeholderText : "String Field (128 Characters)"
    text : (currentTrauma) ? currentTrauma.medicalName : ""
    onEditingFinished : {
      if (text != currentTrauma.medicalName) {
        currentTrauma.medicalName = text
        update_trauma(currentTrauma)
      }
    }
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5

    id : commonNameEntry
    label : "Common Name"
    placeholderText : "String Field (128 Characters)"
    text : (currentTrauma) ? currentTrauma.commonName : ""
    onEditingFinished : {
      if (text != currentTrauma.commonName) {
        currentTrauma.commonName = text
        update_trauma(currentTrauma)
      }
    }
    onLabelWidthChanged : {
      medicalNameEntry.nameWidth = commonNameEntry.nameWidth
    }
  }

  StackLayout {
    id : citationStack
    Layout.leftMargin : 5
    Layout.rightMargin : 20
    Layout.fillWidth : true
    Layout.fillHeight : false

    property var treatmentCitations: []
    property var allCitations: []

    currentIndex : 0
    ListOfCitations {
      id : referenceList
      backend : root.backend
      onList : {
        citationStack.currentIndex = 1;
        refresh_all_citations()
      }
      onCitationAdded : {
        currentObjective.citations.push(citation_g.make());
        currentObjective.citations[currentObjective.citations.length - 1].assign(citation);
        update_trauma(currentObjective);
        refresh_citations()
      }
      onCitationRemoved : {
        currentObjective.removeCitation(index);
        refresh_citations()
      }
    }
    ListOfAllCitations {
      id : fullReferenceList
      Layout.fillWidth : true
      backend : root.backend
      onCitationCreated : {
        refresh_all_citations()
      }
      onCitationAdded : {
        currentObjective.citations.push(citation_g.make());
        currentObjective.citations[currentObjective.citations.length - 1].assign(citation);
        update_trauma(currentObjective);
        refresh_citations();
        citationStack.currentIndex = 0;
      }
      onFullExit : {
        refresh_citations();
        citationStack.currentIndex = 0;
      }
    }
  }

  TextAreaEntry {
    Layout.leftMargin : 5

    id : descriptionEntry
    label : "Description"
    required : true
    placeholderText : "Text Area (5-15 Lines)"
    text : (currentTrauma) ? currentTrauma.description : ""
    onEditingFinished : {
      if (text != currentTrauma.description) {
        currentTrauma.description = text;
        update_trauma(currentTrauma);
      }
    }
  }

  RangeEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5

    id : severityEntry
    label : "Severity Range"
    min : (currentTrauma) ? currentTrauma.min : 0
    max : (currentTrauma) ? currentTrauma.max : 1
    onRangeModified : {
      currentTrauma.min = min;
      currentTrauma.max = max;
      update_trauma(currentTrauma)
    }
  }
  onCurrentTraumaChanged : {
    refresh_citations()
  }
}
