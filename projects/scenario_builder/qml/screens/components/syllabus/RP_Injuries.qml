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
      console.log( "Connection backend -> RP_Injury Removed Citation %1".arg(index))
    }

    onEquipmentRemoved : {
      console.log( "Connection backend -> RP_Injury Removed Equipment %1".arg(index))
    }
  }


  function update_injury(values) {
    obj.injury_id = values.id
    obj.medical_name  = values.medical_name
    obj.common_name   = values.common_name
    obj.description   = values.description
    obj.citations     = values.citations
    obj.min           = values.min
    obj.max           = values.max

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
          update_injury(entry)
        }
    }
  }

  CitationListEntry {
    id : referenceList
    Layout.fillWidth : true
    backend : root.backend

    onCitationAdded : {
      var entry = root.model.get(root.index)
      entry.citations = (entry)? entry.citations.concat(";"+citation_id) : entry.citations.concat(citation_id)
      update_injury(entry)
    }

    onCitationRemoved : {
      var entry = root.model.get(root.index)
      var citations = entry.citations.split(";").filter(item => item).filter(item => item != citation_id);
      entry.citations = citations.join(";")
      update_injury(entry)
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

      var citations = values.citations.split(";").filter(x => x);  
      for(var i = 0; i < citations.length; ++i){
         citation.citation_id = citations[i]
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
