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

  ColumnLayout {
    id : column
    property alias backend : root.backend
    width : root.width

    Equipment {
      id : equipment
    }
    Citation {
      id : citation
    }


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
      EquipmentListEntry {
        id : equipmentList

        Layout.fillWidth : true
        Layout.leftMargin : 5
        backend : column.backend
        onList : {
          equipmentListStack.currentIndex = 1;
        }
        onEquipmentAdded : {}
        onEquipmentRemoved : {}
      }
      FullEquipmentListEntry {
        id : fullEquipmentList
        Layout.fillWidth : true
        Layout.fillHeight : true
        Layout.leftMargin : 5
        backend : root.backend

        onFullAdded : {}
        onFullExit : {}
      }
    }
    StackLayout {
      id : listStack
      Layout.fillWidth : true
      Layout.fillHeight : false
      Layout.leftMargin : 5
      Layout.rightMargin : 20
      Layout.bottomMargin : 5
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
        Layout.leftMargin : 5
        backend : root.backend

        onFullAdded : {}
        onFullExit : {}
      }
    }
  }
}
