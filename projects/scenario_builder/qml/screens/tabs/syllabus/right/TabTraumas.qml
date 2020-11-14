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
    id : citation
  }

  Connections {
    target : backend
    onCitationRemoved : {}
    onEquipmentRemoved : {}
  }


  function update_injury(citation) {
    if (citation) {
      root.backend.update_injury(citation)
    }
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
        update_injury(currentTrauma)
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
        update_injury(currentTrauma)
      }
    }
    onLabelWidthChanged : {
      medicalNameEntry.nameWidth = commonNameEntry.nameWidth
    }
  }

  StackLayout {
    id : listStack
    Layout.leftMargin : 5
    Layout.rightMargin : 20
    Layout.fillWidth : true
    Layout.fillHeight : false
    currentIndex : 0
    CitationListEntry {
      id : referenceList
      backend : root.backend
      onList : {}
      onCitationAdded : {}
      onCitationRemoved : {}
    }
    FullCitationListEntry {
      id : fullReferenceList
      Layout.fillWidth : true
      backend : root.backend
      onFullAdded : {}
      onFullExit : {}
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
        update_injury(currentTrauma);
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
      update_injury(currentTrauma)
    }
  }
}
