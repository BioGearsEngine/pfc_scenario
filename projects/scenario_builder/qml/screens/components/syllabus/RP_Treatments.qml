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
      obj.treatment_id = values.id
      obj.medical_name         = values.medical_name
      obj.common_name          = values.common_name
      obj.description          = values.description

      obj.equipment = values.equipment
      obj.citations = values.citations
      column.backend.update_treatment(obj)
    }

    TextEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5
      id: medicalNameEntry
      label : "Medical Name"
      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.medical_name) {
          entry.medical_name = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          column.update_treatment(entry)
        }
      }
    }

    TextEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5
      id: commonNameEntry
      label : "Common Name"
      placeholderText: "String Field (128 Characters )"
      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.common_name) {
          entry.common_name = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          column.update_treatment(entry)
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"

      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.description) {
          entry.description = text
          console.log("Updating Name filed for Treatment %1".arg(entry.id))
          column.update_treatment(entry)
        }
      }
    }

    EquipmentEntry {
      id: equipmentList
      Layout.fillWidth: true
      Layout.leftMargin: 5
      backend : column.backend

      onEquipmentAdded : {
        console.log("RP_Treatment Added a Equipment")
        var entry = root.model.get(root.index)
        entry.equipment = (entry.equipment) ? entry.equipment.concat(";"+equipment_id) : entry.equipment.concat(equipment_id)
        column.update_treatment(entry)
      }

      onEquipmentRemoved : {
        console.log("RP_Treatment Removed a Equipment")
        var entry = root.model.get(root.index)
        var equipment = entry.equipment.split(";").filter(item => item).filter(item => item != equipment_id);
        entry.equipment = equipment.join(";")
        column.update_treatment(entry)
      }
    }

    StackLayout {
      id : listStack
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.leftMargin: 5
      currentIndex : 0
      CitationListEntry {
        id : referenceList
        Layout.fillWidth : true
        Layout.fillHeight : true
        backend : root.backend  
        onList : {
          console.log("on list")
          var values = root.model.get(root.index)
          if(values) {
            console.log("on list values")
            fullReferenceList.model.clear()
            var citations = values.citations.split(";").filter(x=>x);
            root.backend.citations()
            while(root.backend.next_citation(citation)){
              fullReferenceList.model.insert(fullReferenceList.model.count,
                  {
                     citation_id : "%1".arg(citation.citation_id),
                     key : "%1".arg(citation.key),
                     title : "%1".arg(citation.title), 
                     authors:  "%1".arg(citation.authors),
                     year : "%1".arg(citation.year)
                 }
               );
            };
          }
          listStack.currentIndex = 1
        }

        onCitationAdded : {
          var entry = root.model.get(root.index)
          entry.citations = (entry.citations) ? entry.citations.concat(";"+citation_id) : entry.citations.concat(citation_id)
          update_objective(entry)
        } 

        onCitationRemoved : {
          var entry = root.model.get(root.index)
          var citations = entry.citations.split(";").filter(item => item).filter(item => item != citation_id);
          entry.citations = citations.join(";")
          update_objective(entry)
        }
      }
      FullCitationListEntry {
        id : fullReferenceList
        Layout.fillWidth : true
        Layout.fillHeight : true
        Layout.leftMargin: 5
        backend : root.backend   

        onFullAdded : {
          citation.id = fullReferenceList.model.get(currentIndex)
          root.backend.select_citation(citation)
          var entry = root.model.get(root.index)
          entry.citations = (entry.citations) ? entry.citations.concat(";"+citation_id) : entry.citations.concat(citation_id)
          update_objective(entry)
        }

        onFullExit : {
          listStack.currentIndex = 0
          var values = root.model.get(root.index)
          if(values) {
            nameEntry.text = values.name
            descriptionEntry.text = values.description
            referenceList.model.clear()   

            var citations = values.citations.split(";").filter(x => x);  
            for(var i = 0; i < citations.length; ++i){
              citation.citation_id = citations[i]
              citation.key = ""
              citation.title = ""
              if(root.backend.select_citation(citation)){
                referenceList.model.insert(referenceList.model.count,
                    {
                       citation_id : "%1".arg(citation.citation_id),
                       key : "%1".arg(citation.key),
                       title : "%1".arg(citation.title), 
                       authors:  "%1".arg(citation.authors),
                       year : "%1".arg(citation.year)
                   }
                 );
              }
            };
          }
        }    
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
      var equipments = values.equipment.split(";").filter(x => x);  
      for(var i = 0; i < equipments.length; ++i){
         equipment.equipment_id = equipments[i]
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
      var citations = values.citations.split(";").filter(x => x);  
      for(var i = 0; i < citations.length; ++i){
         citation.citation_id = citations[i]
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