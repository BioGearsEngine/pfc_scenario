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

    InjurySet {
      id : obj
    }
    Injury {
      id : injury
    }

    function update_injurySet(entry){

    }
    
    TextEntry {
      Layout.fillWidth: true
      id: nameEntry
      label : "Name"

      onEditingFinished : {
        var entry = model.get(index)
        if ( text != entry.name){
          entry.name = text
          console.log("updating Name filed for InjurySet %1".arg(entry.injury_id))
          update_injurySet(entry)
        }
      }
    }

    TextAreaEntry {
       Layout.fillWidth: true

      id: descriptionEntry
      label : "Description"

      onEditingFinished : {
        var entry = model.get(index)
        if ( text != entry.description){
          entry.description = text
          console.log("updating Description filed for InjurySet %1".arg(entry.injury_id))
          update_injurySet(entry)
        }
      }
    }

    InjuryListEntry {
      id : injuryList
      Layout.fillWidth : true
      backend : root.backend

      onInjuryAdded : {
        console.log("RP_Objective Added a Reference")
      }

      onInjuryRemoved : {
        console.log("RP_Objective Removed a Reference")
      }
    }

    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.text = values.name
        descriptionEntry.text = values.description
        injuryList.model.clear()
        for (var  i in  values.injuries) {
           injury.injury_id = values.injuries[i]
           injury.medical_name = ""
           injury.common_name  = ""
           root.backend.select_injury(injury)
           injuryList.model.insert(injuryList.model.count,
               {
                 "injury_id" : "%1".arg(injury.injury_id),
                 "medical_name" : "%1".arg(injury.medical_name),
                 "common_name" : "%1".arg(injury.common_name), 
                 "description":  "%1".arg(injury.description),
                 "citations" : "%1".arg(injury.citations),
                 "min" : "%1".arg(injury.min),
                 "max" : "%1".arg(injury.max)
              }
           );
        }
      }
    }
  }
