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


  Injury {
    id : obj
  }

  Citation {
    id : citation
  }

  Connections {
    target: backend
    onCitationRemoved : {
      console.log( "Removed Citation %1".arg(index))
    }

    onEquipmentRemoved : {
      console.log( "Removed Equipment %1".arg(index))
    }
  }


  function update_injury(values) {
    obj.injury_id = values.id
    obj.medical_name         = values.medical_name
    obj.common_name          = values.common_name
    obj.description          = values.description

    obj.citations = []
    for (var  i in  referenceList.model) {
        console.log("Pushing citation %1,%2".arg(i).arg(referenceList.model.get(i).citation_id))
        obj.citations.push(referenceList.model.get(i).citation_id)
    }

    obj.min          = values.min
    obj.max          = values.max
    root.backend.update_injury(obj)
  }


  TextEntry {
    Layout.fillWidth: true

    id: medicalNameEntry
    label : "Medical Name"
    placeholderText: "String Field (128 Characters)"

    onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.medical_name) {
          entry.medical_name = text
          console.log("Updating Name filed for Injury %1".arg(entry.id))
          update_injury(entry)
        }
    }
  }

  TextEntry {
    Layout.fillWidth: true

    id: commonNameEntry
    label : "Common Name"
    placeholderText: "String Field (128 Characters)"

    onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.common_name){
          entry.common_name = text
          console.log("Updating Name filed for Injury %1".arg(entry.id))
          update_injury(entry)
        }
    }
  }

  CitationListEntry {
    id : referenceList
    Layout.fillWidth : true
    backend : root.backend

    onCitationAdded : {
      console.log("RP_Objective Added a Reference")
    }

    onCitationRemoved : {
      console.log("RP_Objective Removed a Reference")
    }
  }

  TextAreaEntry {
    Layout.fillWidth: true

    id: descriptionEntry
    label : "Description"
    required: true
    placeholderText: "Text Area (5-15 Lines)"

    onEditingFinished : {
      var entry = model.get(root.index)
      if ( text != entry.description){
        entry.description = text
        console.log("Updating Name filed for Injury %1".arg(entry.id))
        update_injury(entry)
      }
    }
  }

  RangeEntry {
    Layout.fillWidth: true

    id: severityEntry
    label : "Severity Range"

    onRangeModified : {
        var entry = model.get(root.index)
        entry.min = min
        entry.max = max

        console.log("Updating Name filed for Injury %1".arg(entry.id))
        update_injury(entry)
    }
  }

  onIndexChanged : {
    var values = model.get(index)
    if(values) {
      medicalNameEntry.text  =  values.medical_name
      commonNameEntry.text   =  values.common_name
      descriptionEntry.text   =  values.description
      severityEntry.min     =  values.min
      severityEntry.max     =  values.max
      referenceList.model.clear()
      for (var  i in  values.citations) {
         citation.citation_id = values.citations[i]
         citation.key = ""
         citation.title = ""
         root.backend.select_citation(citation)
         referenceList.model.insert(referenceList.model.count,
             {
               "citation_id" : "%1".arg(citation.citation_id),
               "key" : "%1".arg(citation.key),
               "title" : "%1".arg(citation.title), 
               "authors":  "%1".arg(citation.authors),
               "year" : "%1".arg(citation.year)
            }
         );
      }
    }
  }
}
