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

    function update_injury_set(values){
    obj.injury_set_id = values.id
    obj.name  = values.name
    obj.description   = values.description
    obj.injuries      = values.injuries
    obj.locations     = values.locations
    obj.severities    = values.severities

    root.backend.update_injury_set(obj)
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
        var entry = root.model.get(root.index)
        entry.injuries = (entry)? entry.injuries.concat(";"+injury_id) : entry.injuries.concat(injury_id)
        console.log("OnInjuryAdded")
        update_injury_set(entry)
      }

      onInjuryRemoved : {
        var entry = root.model.get(root.index)
        var injuries = entry.injuries.split(";").filter(item => item).filter(item => item != injury_id);
        entry.injuries = injuries.join(";")
        console.log("OnInjuryRemoved")
        update_injury_set(entry)
      }
    }

    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.text = values.name
        descriptionEntry.text = values.description

        var injuries  = values.injuries.split(";").filter(x => x);  
        var locations  = values.locations.split(";").filter(x => x);  
        var severities = values.severities.split(";").filter(x => x);  
        
        injuryList.model.clear()
        for(var i = 0; i < injuries.length; ++i){
           injury.injury_id = injuries[i]
           injury.medical_name = ""
           injury.common_name  = ""
           root.backend.select_injury(injury)
           injuryList.model.insert(injuryList.model.count,
               {
                 injury_id : "%1".arg(injury.injury_id),
                 medical_name : "%1".arg(injury.medical_name),
                 common_name : "%1".arg(injury.common_name), 
                 description:  "%1".arg(injury.description),
                 citations : "%1".arg(injury.citations),
                 min : "%1".arg(injury.min),
                 max : "%1".arg(injury.max),
                 location : locations[i],
                 severity : severities[i]
              }
           );
        }
      }
    }
  }
