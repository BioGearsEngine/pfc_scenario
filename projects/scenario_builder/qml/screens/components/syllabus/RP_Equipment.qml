import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout  {
  id: root
  property SQLBackend backend
  property ListModel model
  property int index
  
  Layout.fillWidth: true
  Layout.fillHeight: true

  Equipment {
    id : obj
  }

  Citation {
    id : citation
  }

  function update_equipment(values) {
    obj.equipment_id = values.id
    obj.name         = values.name
    obj.description  = values.description

    obj.citations = []
    for (var  i = 0; i < referenceList.model.count; ++i) {
        obj.citations.push(referenceList.model.get(i).citation_id)
    }

    obj.image = values.image
    root.backend.update_equipment(obj)
  }

  TextEntry {
   Layout.fillWidth: true

    id: nameEntry
    label : "Name"
    placeholderText: "String Field (128 Characters )"

    onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.name) {
          entry.name = text
          console.log("Updating Name filed for Equipment %1".arg(entry.id))
          update_equipment(entry)
        }
      }
  }

  TextAreaEntry {
    Layout.fillWidth: true

    id: descriptionEntry
    label : "Description"
    required: true
    onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.description) {
          entry.description = text
          console.log("Updating Description filed for Treatment %1".arg(entry.id))
          update_equipment(entry)
        }
      }
  }

  TextEntry {
    Layout.fillWidth: true

    id: imageEntry
    label : "Image"
    placeholderText: "String Field (128 Characters )"

    onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.image) {
          entry.image = text
          console.log("Updating Image filed for Equipment %1".arg(entry.id))
          update_equipment(entry)
        }
      }
  }

  CitationListEntry {
    id : referenceList
    Layout.fillWidth : true
    backend : root.backend

    onCitationAdded : {
      console.log("RP_Treatment Added a Reference")
      var entry = root.model.get(root.index)
      update_equipment(entry)
    }

    onCitationRemoved : {
      console.log("RP_Treatment Removed a Reference")
      var entry = root.model.get(root.index)
      update_equipment(entry)
    }
  }

  onIndexChanged : {
    var values = root.model.get(index)
    if(values) {
      nameEntry.text = values.name
      descriptionEntry.text = values.description
      imageEntry.text = values.image
      referenceList.model.clear()
      for (var  i in  values.citaitons) {
         citation.citation_id = values.citaitons[i]
         citation.key = ""
         citation.title  = ""
         root.backend.select_citation(citation)
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
