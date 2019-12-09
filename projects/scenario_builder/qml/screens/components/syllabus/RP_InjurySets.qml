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
      Layout.leftMargin: 5
      id: nameEntry
      label : "Name"

      onEditingFinished : {
        var entry = model.get(index)
        if ( text != entry.name){
          entry.name = text
          update_injurySet(entry)
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5

      id: descriptionEntry
      label : "Description"

      onEditingFinished : {
        var entry = model.get(index)
        if ( text != entry.description){
          entry.description = text
          update_injurySet(entry)
        }
      }
    }

    InjuryListEntry {
      id : injuryList
      Layout.fillWidth : true
      Layout.leftMargin: 5
      backend : root.backend

      onSeverityChanged : {
         var entry = root.model.get(root.index)
         var severities = entry.severities.split(";").filter(item => item)
         severities.splice(index,1,severity)
         entry.severities = severities.join(";")
         update_injury_set(entry)
      }

      onLocationChanged : {
        var entry = root.model.get(root.index)
        var locations = entry.locations.split(";").filter(item => item)
        locations.splice(index,1,location)
        entry.locations = locations.join(";")
        update_injury_set(entry)
      }

      onInjuryAdded : {
        var entry = root.model.get(root.index)
        entry.injuries    = (entry)? entry.injuries.concat(";"+injury_id) : entry.injuries.concat(injury_id)
        entry.severities  = (entry)? entry.severities.concat(";"+severity) : entry.injuries.concat(severity)
        entry.locations   = (entry)? entry.locations.concat(";"+location) : entry.injuries.concat(location)
        update_injury_set(entry)
      }

      onInjuryRemoved : {
        var entry = root.model.get(root.index)
        var injuries = entry.injuries.split(";").filter(item => item)
        var severities = entry.severities.split(";").filter(item => item)
        var locations = entry.locations.split(";").filter(item => item)
        
        injuries.splice(index,1);
        severities.splice(index,1)
        locations.splice(index,1)
        
        entry.injuries = injuries.join(";")
        entry.severities = severities.join(";")
        entry.locations = locations.join(";")
        
        update_injury_set(entry)
      }
    }

    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.text = values.name
        descriptionEntry.text = values.description

        var injuries   = (values.injuries) ? values.injuries.split(";").filter(x => x)  : "";
        var locations  = (values.locations) ? values.locations.split(";").filter(x => x) : "";
        var severities = (values.severities) ? values.severities.split(";").filter(x => x): "";
        injuryList.model.clear()
        for(var i = 0; i < injuries.length; ++i){
           injury.injury_id = injuries[i]
           injury.medical_name = ""
           injury.common_name  = ""
           root.backend.select_injury(injury)
           injuryList.model.insert(injuryList.model.count,
               {
                 injury_id : injury.injury_id,
                 medical_name : "%1".arg(injury.medical_name),
                 common_name : "%1".arg(injury.common_name), 
                 description:  "%1".arg(injury.description),
                 citations : "%1".arg(injury.citations),
                 min : "%1".arg(injury.min),
                 max : "%1".arg(injury.max),
                 location : (locations.length) ? locations[i] : "",
                 severity : (severities.length) ?severities[i] : 0.5
              });
        }
      }
    }
  }
