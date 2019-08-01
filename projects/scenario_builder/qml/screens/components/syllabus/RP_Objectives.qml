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

    ListEntry {
      Layout.fillWidth: true
      Layout.fillHeight : true

      id: citationEntry
      label : "Citations"
      model : ListModel { ListElement {citation_id : -1; title : "PlaceHolder Citation"; authors : "No Author"}}
      delegate : Row {
        Layout.fillWidth : true
        Layout.preferredHeight : 100
        spacing : 6
        Label {
          text : "Title:"
        }
        Text {
          text : model.title
        }
        Label {
          text : "Authors:"
        }
        Text {
          text : model.authors
        }
      }

      onAdded : {
        console.log ("Addeds an Item")
      }

      onRemoved : {
        console.log ("Removed an Item")
      }
    }

    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.value = values.name
        descriptionEntry.text = values.description
        citationEntry.model.clear()
        citationEntry.model.insert(citationEntry.model.count,{citation_id : 5, title : "The Great Book on Objectives", authors : "Steven A White, Ph.D"});
        citationEntry.model.insert(citationEntry.model.count,{citation_id : 6, title : "Keystone tactics", authors : "Lucas Marin"});
        citationEntry.model.insert(citationEntry.model.count,{citation_id : 7, title : "The glory of Keyblades", authors : "Nathan Tatum, Et Al."});
      }
    }
}
