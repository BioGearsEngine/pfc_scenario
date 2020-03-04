import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle{
    id: root  
    property SQLBackend backend
    property ListModel model
    property int index
    property int count
    property alias sceneName: nameEntry.text

    Location {
      id : self
    }
    Scene {
      id : selfScene
    }

    function update_location() {
      if (model.count == 0) {
        return
      }
      var obj = self
      obj.location_id = -1
      obj.name = locationNameEntry.text
      obj.scene_name = nameEntry.text
      obj.environment = environmentEntry.text
      console.log("updating_location")
      root.backend.update_location(obj)
      console.log("finished updating_location")
      selfScene.name = root.model.get(index).name
      root.backend.select_scene(selfScene)
      selfScene.name = nameEntry.text
      root.backend.update_scene(selfScene)
      root.model.get(index).name = nameEntry.text
    } 
    border.color: 'black'
    border.width: 1
    ColumnLayout  {
        width: parent.width


        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: nameEntry
          label : "Scene Name"
          placeholderText: "String Field (128 Characters)"
          onEditingFinished : {
            var entry = root.model.get(root.index)
            if ( text != entry.name){
              update_location()
            }
          }
        }   

        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: locationNameEntry
          label : "Location Name"
          placeholderText: "String Field (128 Characters)"
        }
        TimeEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: timeOfDayEntry
          label : "Time of Day"
        }
        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: timeScenarioEntry
          label : "Time in Scenario"
          placeholderText: "Time Input Field (3H20M)"
        }
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: environmentEntry
          label : "Environments"
          required: true
          placeholderText: "Weather Input Area"
        }   

    }
    onIndexChanged : {
      var values = model.get(root.index)
      if(values && model.count != 0) {
        nameEntry.text = root.model.get(root.index).name
      }
    }
    onCountChanged : {
      if(count == 0) {
          nameEntry.text = ""
          locationNameEntry.text = ""
          environmentEntry.text = ""
          timeOfDayEntry.hours = 23
          timeOfDayEntry.minutes = 59
          nameEntry.text.readOnly = true
          locationNameEntry.text.readOnly = true
          timeScenarioEntry.text.readOnly = true
        } else {
          locationNameEntry.text.readOnly = false
          nameEntry.text.readOnly = false
          timeScenarioEntry.text.readOnly = false
          indexChanged()
        }
    }
}