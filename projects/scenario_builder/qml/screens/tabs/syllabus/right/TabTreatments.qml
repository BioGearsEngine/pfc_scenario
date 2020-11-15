import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ScrollView {
  id : root
  property SQLBackend backend
  property Treatment currentTreatment
  property int topIndex // topIndex is the index of the top set of 4 tabs
  Layout.fillWidth : true
  Layout.fillHeight : true

  contentWidth : column.width
  contentHeight : column.height
  clip : true

  function update_treatment(treatment) {
    if (root.backend && treatment) {
      root.backend.update_treatment(treatment)
    }
  }
  function refresh_equipment() {
    equipmentStack.treatmentCitations = []
    let equipment = currentTreatment.equipment;
    for (var ii = 0; ii < equipment.length; ++ ii) {
      equipmentStack.treatmentCitations.push(equipment_g.make());
      equipmentStack.treatmentCitations[equipmentStack.treatmentCitations.length - 1].assign(equipments[ii]);
    }
    referenceList.model = equipmentStack.treatmentCitations;
  }
  function refresh_all_equipments() {
    equipmentStack.allCitations = [];
    let equipment = root.backend.equipment;
    for (var ii = 0; ii < equipment.length; ++ ii) {
      equipmentStack.allCitations.push(equipment_g.make());
      equipmentStack.allCitations[equipmentStack.allCitations.length - 1].assign(equipment[ii]);
    }
    fullReferenceList.model = equipmentStack.allCitations;
  }
  function refresh_citations() {
    citationStack.treatmentCitations = []
    let citations = currentTreatment.citations;
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

  Equipment {
    id : equipment_g
  }
  Citation {
    id : citation_g
  }

  ColumnLayout {
    id : column
    property alias backend : root.backend
    width : root.width


    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : medicalNameEntry
      label : "Medical Name"
      placeholderText : "String Field (128 Characters)"
      text : (currentTreatment) ? currentTreatment.medicalName : ""
      onEditingFinished : {
        if (text != currentTreatment.medicalName) {
          currentTreatment.medicalName = text;
          update_treatment(currentTreatment);
        }
      }
    }

    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : commonNameEntry
      label : "Common Name"
      placeholderText : "String Field (128 Characters)"
      text : (currentTreatment) ? currentTreatment.commonName : ""
      onEditingFinished : {
        if (text != currentTreatment.commonName) {
          currentTreatment.commonName = text;
          update_treatment(currentTreatment);
        }
      }
      onLabelWidthChanged : {
        medicalNameEntry.nameWidth = commonNameEntry.nameWidth;
      }
    }

    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : descriptionEntry
      label : "Description"
      required : true
      placeholderText : "Text Area (5-15 Lines)"
      text : currentTreatment ? currentTreatment.description : ""
      onEditingFinished : {
        if (text != currentTreatment.description) {
          currentTreatment.description = text;
          update_treatment(currentTreatment);
        }
      }
    }
    StackLayout {
      id : equipmentListStack
      Layout.fillWidth : true
      Layout.fillHeight : false
      Layout.leftMargin : 5
      Layout.rightMargin : 20
      currentIndex : 0
      property var treatmentEquipment: []
      property var allEquipment: []
      ListOfEquipment {
        id : equipmentList

        Layout.fillWidth : true
        Layout.leftMargin : 5
        backend : column.backend
        onList : {
          equipmentStack.currentIndex = 1;
          refresh_all_equipment()
        }
        // onEquipmentAdded : {
        // currentTreatment.equipment.push(equipment_g.make());
        // currentTreatment.equipment[currentTreatment.equipment.length - 1].assign(equipment);
        // update_treatment(currentTreatment);
        // refresh_equipment()
        // }
        // onEquipmentRemoved : {
        // currentTreatment.removeEquipment(index);
        // refresh_equipment()
        // }
      }
      ListOfAllEquipment {
        id : fullEquipmentList
        Layout.fillWidth : true
        Layout.fillHeight : true
        Layout.leftMargin : 5
        backend : root.backend

        // onEquipmentCreated : {
        // refresh_all_equipment()
        // }
        // onEquipmentAdded : {
        // currentTreatment.equipment.push(equipment_g.make());
        // currentTreatment.equipment[currentTreatment.equipment.length - 1].assign(equipment);
        // update_treatment(currentTreatment);
        // refresh_equipment();
        // equipmentStack.currentIndex = 0;
        // }
        onFullExit : {
          refresh_equipment();
          equipmentStack.currentIndex = 0;
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
          currentTreatment.citations.push(citation_g.make());
          currentTreatment.citations[currentTreatment.citations.length - 1].assign(citation);
          update_treatment(currentTreatment);
          refresh_citations()
        }
        onCitationRemoved : {
          currentTreatment.removeCitation(index);
          refresh_citations()
        }
      }
      ListOfAllCitations {
        id : fullReferenceList
        Layout.leftMargin : 5
        backend : root.backend

        onCitationCreated : {
          refresh_all_citations()
        }
        onCitationAdded : {
          currentTreatment.citations.push(citation_g.make());
          currentTreatment.citations[currentTreatment.citations.length - 1].assign(citation);
          update_treatment(currentTreatment);
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
  onCurrentTreatmentChanged : {
    refresh_citations()
  }
}
