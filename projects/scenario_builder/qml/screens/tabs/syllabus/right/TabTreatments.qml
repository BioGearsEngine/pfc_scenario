import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.1
import Qt.labs.platform 1.1 as Labs
import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0 as PFC

ScrollView {
  id : root
  property PFC.SQLBackend backend
  property PFC.Treatment currentTreatment
  property int topIndex // topIndex is the index of the top set of 4 tabs
  Layout.fillWidth : true
  Layout.fillHeight : true

  contentWidth : column.width
  contentHeight : column.height
  clip : true

  PFC.Equipment {
    id : equipment_g
  }
  PFC.Citation {
    id : citation_g
  }

  ColumnLayout {
    id : column
    width : root.width
    spacing : 5
    GridLayout {
      id : grid
      Layout.fillWidth : true
      columns : 2
      TextEntry {
        Layout.minimumWidth : 2 * root.width / 3
        Layout.fillWidth : true
        Layout.leftMargin : 5
        Layout.column : 0
        Layout.row : 0

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
        Layout.column : 0
        Layout.row : 1
        Layout.alignment : Qt.AlignTop
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
      Image {
        id : imageView
        Layout.alignment : Qt.AlignHCenter | Qt.AlignTop
        Layout.fillWidth : true
        Layout.fillHeight : false
        Layout.preferredHeight : 150
        Layout.leftMargin : 5
        Layout.column : 1
        Layout.row : 0
        Layout.rowSpan : 2
        fillMode : Image.PreserveAspectFit
        source : "qrc:/img/treatment_placeholder.png"
      }
      Button {
        Layout.alignment : Qt.AlignHCenter | Qt.AlignTop
        text : "Browse"
        Layout.column : 1
        Layout.row : 2
        FileDialog {
          id : browseDialog
          title : "Please Choose a File:"
          visible : false
          selectMultiple : false
          selectExisting : true
          nameFilters : ["Images (*.jpg *.png *.bmp)", "All files (*)"]
          folder : Labs.StandardPaths.writableLocation(Labs.StandardPaths.PicturesLocation)
          onAccepted : {
            console.log("Selected %1".arg(browseDialog.fileUrls.toString()));
            currentTreatment.image.clear();
            currentTreatment.image.uri = browseDialog.fileUrls.toString();
            currentTreatment.image.cache(currentTreatment.uuid);
            update_treatment(currentTreatment);
          }
          onRejected : {
            console.log("Canceled")
          }
        }

        onClicked : {
          browseDialog.open()
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
      text : currentTreatment ? currentTreatment.description : ""
      maximumRows : 15
      minimumRows : 10
      onEditingFinished : {
        if (text != currentTreatment.description) {
          currentTreatment.description = text;
          update_treatment(currentTreatment);
        }
      }
    }
    StackLayout {
      id : equipmentStack
      Layout.fillWidth : true
      Layout.fillHeight : false
      Layout.leftMargin : 5
      Layout.rightMargin : 20
      currentIndex : 0
      property var treatmentEquipment: []
      property var allEquipment: []

      CrossReferenceEquipment {
        id : equipmentList

        Layout.fillWidth : true
        Layout.leftMargin : 5
        backend : root.backend
        onList : {
          equipmentStack.currentIndex = 1;
          refresh_all_equipment()
        }
        onEquipmentAdded : {
          currentTreatment.equipment.push(equipment_g.make());
          currentTreatment.equipment[currentTreatment.equipment.length - 1].assign(equipment);
          update_treatment(currentTreatment);
          refresh_equipment()
        }
        onEquipmentRemoved : {
          currentTreatment.removeEquipment(index);
          refresh_equipment()
        }
      }
      ListOfAllEquipment {
        id : fullEquipmentList
        Layout.fillWidth : true
        Layout.fillHeight : true
        Layout.leftMargin : 5
        backend : root.backend

        onEquipmentCreated : {
          refresh_all_equipment()
        }
        onEquipmentAdded : {
          currentTreatment.equipment.push(equipment_g.make());
          currentTreatment.equipment[currentTreatment.equipment.length - 1].assign(equipment);
          update_treatment(currentTreatment);
          refresh_equipment();
          equipmentStack.currentIndex = 0;
        }
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
      CrossReferenceCitations {
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
    refresh_citations();
    refresh_equipment();
  }

  Connections {
    target : backend
  }

  function update_treatment(treatment) {
    if (root.backend && treatment) {
      root.backend.update_treatment(treatment)
    }
  }
  function refresh_equipment() {
    equipmentStack.treatmentEquipment = []
    let equipment = currentTreatment.equipment;
    for (var ii = 0; ii < equipment.length; ++ ii) {
      equipmentStack.treatmentEquipment.push(equipment_g.make());
      equipmentStack.treatmentEquipment[equipmentStack.treatmentEquipment.length - 1].assign(equipment[ii]);
    }
    equipmentList.model = equipmentStack.treatmentEquipment;
  }
  function refresh_all_equipment() {
    equipmentStack.allEquipment = [];
    let equipment = root.backend.equipment;
    for (var ii = 0; ii < equipment.length; ++ ii) {
      equipmentStack.allEquipment.push(equipment_g.make());
      equipmentStack.allEquipment[equipmentStack.allEquipment.length - 1].assign(equipment[ii]);
    }
    fullEquipmentList.model = equipmentStack.allEquipment;
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
}
