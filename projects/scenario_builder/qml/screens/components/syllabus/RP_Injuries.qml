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
    for (var  i in  values.citations) {
        obj.citations.push(values.citations[i])
    }

    obj.min          = values.min
    obj.max          = values.max
    root.backend.update_objective(obj)
  }


  TextEntry {
    Layout.fillWidth: true

    id: medicalNameEntry
    label : "Medical Name"
    placeholderText: "String Field (128 Characters)"
  }

  TextEntry {
    Layout.fillWidth: true

    id: commonNameEntry
    label : "Common Name"
    placeholderText: "String Field (128 Characters)"
  }

    CitationListEntry {
      id : referenceList
      Layout.fillWidth : true
      Layout.fillHeight : true
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
  }

  TextEntry {
    Layout.fillWidth: true

    id: severityEntry
    label : "Severity Range"
    placeholderText: "TODO: Make a Spinner Box"
  }

  onIndexChanged : {
    var values = model.get(index)
    if(values) {
      medicalNameEntry.value  =  values.medical_name
      commonNameEntry.value   =  values.common_name
      descriptionEntry.text   =  values.description
      severityEntry.value     =  "(%1,%2)".arg(values.min).arg(values.max)
      console.log(values.citations)
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
