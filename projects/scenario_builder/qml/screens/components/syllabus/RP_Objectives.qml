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

    Objective {
      id : obj
    }
    Citation {
      id : citation
    }
    Connections {
      target: backend
      onCitationRemoved : {
        console.log( "Removed %1".arg(index))
      }
    }

    function update_objective(values) 
    {
      obj.objective_id = values.id
      obj.name         = values.name
      obj.description  = values.description
      obj.citations = []

      for (var  i in  values.citations) {
          obj.citations.push(values.citations[i])
      }
      root.backend.update_objective(obj)
    }

    TextEntry {
      Layout.fillWidth: true

      id: nameEntry
      label : "Name"
      placeholderText: "String Field (128 Characters)"
      
      onEditingFinished : {
        var entry = model.get(index)
        if ( value != entry.name){
          entry.name = value
          console.log("updating Name filed for entry")
          update_objective(entry)
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
        var entry = model.get(index)
        if ( text != entry.description){
          entry.description = text
          console.log("updating Description filed for entry")
          update_objective(entry)
        }
      }
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
    
    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.value = values.name
        descriptionEntry.text = values.description
        referenceList.model.clear()
        for (var  i in  values.citations) {
           citation.citation_id = values.citations[i]
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

