import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ScrollView {
  id : root
  property SQLBackend backend
  property Equipment currentEquipment
  property int topIndex // topIndex is the index of the top set of 4 tabs

  contentWidth : column.width
  contentHeight : column.height
  clip : true

  Citation {
    id : citation_g
  }

  Connections {
    target : backend
    onCitationsChanged : {
      refresh_citations()
    }
  }

  function update_equipment(equipment) {
    if (equipment) {
      root.backend.update_equipment(equipment)
    }
  }
  function refresh_citations() {
    citationStack.equipmentCitations = []
    let citations = currentEquipment.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.equipmentCitations.push(citation_g.make());
      citationStack.equipmentCitations[citationStack.equipmentCitations.length - 1].assign(citations[ii]);
    }
    referenceList.model = citationStack.equipmentCitations;
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

  ColumnLayout {
    id : column
    property alias backend : root.backend
    width : root.width


    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : nameEntry
      label : "Name"
      placeholderText : "String Field (128 Characters)"
      text : (currentEquipment) ? currentEquipment.name : ""
      onEditingFinished : {
        if (text != currentEquipment.name) {
          currentEquipment.name = text;
          update_equipment(currentEquipment);
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : summaryEntry
      label : "Summary"
      required : true
      placeholderText : "Text Area (5-15 Lines)"
      text : (currentEquipment) ? currentEquipment.summary : ""
      onEditingFinished : {
        if (text != currentEquipment.summary) {
          currentEquipment.summary = text;
          update_equipment(currentEquipment);
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : descriptionEntry
      label : "Description"
      placeholderText : "Text Area (5-15 Lines)"
      text : (currentEquipment) ? currentEquipment.description : ""
      required : true
      onEditingFinished : {
        if (text != currentEquipment.description) {
          currentEquipment.description = text;
          update_equipment(currentEquipment);
        }
      }
    }

    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : imageEntry
      label : "Image"
      placeholderText : "String Field (128 Characters )"
      text : (currentEquipment) ? currentEquipment.image : ""
      onEditingFinished : {
        if (text != currentEquipment.image) {
          currentEquipment.image = text;
          update_equipment(currentEquipment);
        }
      }
    }

    StackLayout {
      id : citationStack
      Layout.fillWidth : true
      Layout.fillHeight : false
      Layout.leftMargin : 5
      Layout.rightMargin : 20
      Layout.bottomMargin : 5

      property var equipmentCitations: []
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
          currentEquipment.citations.push(citation_g.make());
          currentEquipment.citations[currentEquipment.citations.length - 1].assign(citation);
          update_equipment(currentEquipment);
          refresh_citations()
        }

        onCitationRemoved : {
          currentEquipment.removeCitation(index);
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
          currentEquipment.citations.push(citation_g.make());
          currentEquipment.citations[currentEquipment.citations.length - 1].assign(citation);
          update_equipment(currentEquipment);
          refresh_citations();
          citationStack.currentIndex = 0;
        }

        onFullExit : {
          refresh_citations();
          citationStack.currentIndex = 0;
        }
      }
    }
  }

  onCurrentEquipmentChanged : {
    refresh_citations();
  }
  onBackendChanged : {
    if (backend) {
      backend.citationsChanged.connect(refresh_citations);
    }
  }
}