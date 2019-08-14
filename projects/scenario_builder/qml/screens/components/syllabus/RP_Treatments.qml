import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ScrollView {
  id: root

  Layout.fillWidth: true
  Layout.fillHeight: true

  contentWidth: column.width
  contentHeight: column.height
  clip : true  

  property SQLBackend backend
  property ListModel model
  property int index

  ColumnLayout  {
    id: column
    property alias backend : root.backend

    width : root.width

    Treatment {
      id : obj
    }
    Equipment {
      id: equipment
    }
    Citation {
      id : citation
    }

    function update_treatment(values) {
      obj.injury_id = values.id
      obj.medical_name         = values.medical_name
      obj.common_name          = values.common_name
      obj.description          = values.description

      obj.equipment = []
      for (var  i = 0; i < equipmentList.count; ++i) {
          console.log("Pushing equipment %1,%2".arg(i).arg(equipmentList.model.get(i).citation_id))
          obj.citations.push(equipmentList.model.get(i).citation_id)
      }

      obj.citations = []
      for (var  i = 0; i < referenceList.count; ++i) {
          console.log("Pushing citations %1,%2".arg(i).arg(referenceList.model.get(i).citation_id))
          obj.citations.push(referenceList.model.get(i).citation_id)
      }
      column.backend.update_treatment(obj)
    }

    TextEntry {
      Layout.fillWidth: true
      id: medicalNameEntry
      label : "Medical Name"
      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.medical_name) {
          entry.medical_name = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          update_treatment(entry)
        }
      }
    }

    TextEntry {
      Layout.fillWidth: true
      id: commonNameEntry
      label : "Common Name"
      placeholderText: "String Field (128 Characters )"
      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.common_name) {
          entry.common_name = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          update_treatment(entry)
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth: true

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"

      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.description) {
          entry.description = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          update_treatment(entry)
        }
      }
    }

    EquipmentEntry {
      id: equipmentList
      Layout.fillWidth: true
      backend : column.backend

      onEquipmentAdded : {
        console.log("RP_Treatment Added a Equipment")
        var entry = root.model.get(root.index)
        update_treatment(entry)
      }

      onEquipmentRemoved : {
        console.log("RP_Treatment Removed a Equipment")
        var entry = root.model.get(root.index)
        update_treatment(entry)
      }
    }

    CitationListEntry {
      id : referenceList
      Layout.fillWidth : true
      backend : column.backend

      onCitationAdded : {
        console.log("RP_Treatment Added a Reference")
        var entry = root.model.get(root.index)
        update_treatment(entry)
      }

      onCitationRemoved : {
        console.log("RP_Treatment Removed a Reference")
        var entry = root.model.get(root.index)
        update_treatment(entry)
      }
    }
  }

  onIndexChanged : {
    var values = model.get(root.index)
    if(values) {
      medicalNameEntry.text = values.medical_name
      commonNameEntry.text = values.common_name
      descriptionEntry.text = values.description

      equipmentList.model.clear()
      for (var  i in  values.equipment) {
         equipment.equipment_id = values.equipment[i]
         equipment.name = ""
         column.backend.select_equipment(equipment)
         equipmentList.model.insert(equipmentList.model.count,
             {
               "equipment_id" : "%1".arg(equipment.equipment_id),
               "name" : "%1".arg(equipment.name),
               "type" : "%1".arg(equipment.type), 
               "description":  "%1".arg(equipment.description),
               "citations" : "%1".arg(equipment.citations),
               "image" : "%1".arg(equipment.image)
            }
         );
      }

      referenceList.model.clear()
      for (var  i in  values.citations) {
         citation.citation_id = values.citations[i]
         citation.key = ""
         citation.title  = ""
         column.backend.select_citation(citation)
         referenceList.model.insert(referenceList.model.count,
             {
               "citation_id" : "%1".arg(citation.citation_id),
               "key" : "%1".arg(citation.key),
               "title":  "%1".arg(citation.title),
               "authors" : "%1".arg(citation.authors),
               "year" : "%1".arg(citation.year),
               "publisher" : "%1".arg(citation.publisher)
            }
         );
      }
    }
  }
}