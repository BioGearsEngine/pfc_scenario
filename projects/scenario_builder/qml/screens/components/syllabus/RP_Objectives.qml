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
        console.log( "Removed %1".arg(root.index))
      }
    }

    function update_objective(values) 
    {
      obj.objective_id = values.id
      obj.name         = values.name
      obj.description  = values.description
      obj.citations = []

      for (var  i = 0; i < referenceList.count; ++i) {
        var citation_id = referenceList.model.get(i).citation_id
        obj.citations.push(citation_id)
      }

      root.backend.update_objective(obj)
    }

    TextEntry {
      Layout.fillWidth: true

      id: nameEntry
      label : "Name"
      placeholderText: "String Field (128 Characters)"
      
      onEditingFinished : {
        var entry = root.model.get(root.index)
        if ( text != entry.name){
          entry.name = text
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
        var entry = root.model.get(root.index)
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
        var entry = root.model.get(root.index)
        entry.citations = entry.citations.concat(";"+citation_id)
        update_objective(entry)
      }

      onCitationRemoved : {
        console.log("RP_Objective Removed a Reference")
        var entry = root.model.get(root.index)
        var citations = entry.citations.split(";").filter(item => item).filter(item => item != citation_id);
        entry.citations = citations.join(";")
        update_objective(entry)
      }
    }
    
    onIndexChanged : {
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

