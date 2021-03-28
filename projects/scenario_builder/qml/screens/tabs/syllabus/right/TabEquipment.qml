import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.1
import Qt.labs.platform 1.1 as Labs
import "../../../common"
import "components"

import com.ara.pfc.ScenarioModel.SQL 1.0 as PFC

ScrollView {
  id : root
  property PFC.SQLBackend backend
  property PFC.Equipment currentEquipment
  property int topIndex // topIndex is the index of the top set of 4 tabs

  contentWidth : column.width
  contentHeight : column.height
  clip : true

  PFC.Citation {
    id : citation_g
  }

  PFC.EquipmentParameter {
    id : equipment_parameter_g
  }

  Connections {
    target : backend
    onCitationsChanged : {
      refresh_citations()
    }
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
        Layout.fillHeight : true
        Layout.leftMargin : 5
        Layout.column : 0
        Layout.row : 1

        maximumRows : 20
        minimumRows : 12
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

      Image {
        id : imageView
        Layout.alignment : Qt.AlignHCenter | Qt.AlignVCenter
        property int updateCounter : 0
        Layout.fillWidth : true
        Layout.fillHeight : false
        Layout.preferredHeight : 150
        Layout.leftMargin : 5
        Layout.column : 1
        Layout.row : 0
        Layout.rowSpan : 2
        fillMode : Image.PreserveAspectFit
        source : updateCounter, (currentEquipment) ? currentEquipment.image.uri :"qrc:/img/equipment_placeholder.png"
      }

      Button {
        Layout.alignment : Qt.AlignHCenter
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
            currentEquipment.image.clear();
            currentEquipment.image.uri = browseDialog.fileUrls.toString();
            currentEquipment.image.cache(currentEquipment.uuid);
            backend.update_image(currentEquipment.image);
            update_equipment(currentEquipment);
            imageView.updateCounter++
            
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

    // 0: OTHER
    // 1: Equipment (IV Pole, Monitor, Oxygen Tank)
    // 2: Attachment (Tempus Pro, Splint)
    // 3:Substance (Drugs)
    // 4:Consumable Equipment (IV Bags, Syringes)

    ComboBoxEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : categoryEntry
      label : "Category"
      model : [
        "Other",
        "Equipment",
        "Attchment",
        "Substance",
        "Consumable Equipment"
      ]
      currentIndex : (currentEquipment) ? currentEquipment.type : 0
      onEditingFinished : {
        if (currentIndex != currentEquipment.type) {
          currentEquipment.type = currentIndex;
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
    EquipmentParameterList {
      id : parameterLists
      Layout.fillWidth : true
      Layout.alignment : Qt.AlignTop
      Layout.leftMargin : 5
      Layout.bottomMargin : 5
      Layout.rightMargin : 20
      Layout.minimumHeight : 300

      model : (currentEquipment) ? currentEquipment.parameters : []

      onParamaterModified : {
        update_equipment(currentEquipment);
      }
      onParameterAdded : {
        var parms = currentEquipment.parameters;
        parms.push(equipment_parameter_g.make());
        parms[parms.length - 1].name = "field %1".arg(parms.length);
        update_equipment(currentEquipment);
        model = currentEquipment.parameters;
      }
      onParameterRemoved : {
        currentEquipment.removeParameter(index);
        update_equipment(currentEquipment);
        model = currentEquipment.parameters;
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
      CrossReferenceCitations {
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
    (currentEquipment) ? currentEquipment.parameters : []
  }

  Connections {
    target : backend

  }

  function update_equipment(equipment) {
    if (equipment) {
      root.backend.update_equipment(equipment)
    }
  }
  function refresh_citations() {
    if (backend && currentEquipment) {
      citationStack.equipmentCitations = []
      let citations = currentEquipment.citations;
      for (var ii = 0; ii < citations.length; ++ ii) {
        citationStack.equipmentCitations.push(citation_g.make());
        citationStack.equipmentCitations[citationStack.equipmentCitations.length - 1].assign(citations[ii]);
      }
      referenceList.model = citationStack.equipmentCitations;
    }
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
