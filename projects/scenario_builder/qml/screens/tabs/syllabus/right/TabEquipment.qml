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

  ColumnLayout {
    id : column
    property alias backend : root.backend
    width : root.width

    Citation {
      id : citation
    }

    function update_equipment(equipment) {
      if (equipment) {
        root.backend.update_equipment(equipment)
      }
    }

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
        onList : {}

        onCitationAdded : {}

        onCitationRemoved : {}
      }
      FullCitationListEntry {
        id : fullReferenceList
        backend : root.backend

        onFullAdded : {}

        onFullExit : {}
      }
    }
  }
  onTopIndexChanged : {
    if (topIndex == 1) {}
  }
  onCurrentEquipmentChanged : {}
}
