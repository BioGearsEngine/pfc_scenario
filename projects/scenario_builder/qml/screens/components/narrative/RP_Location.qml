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
    //property alias sceneName: nameEntry.text

    Location {
      id : currentLocation
    }
    Scene {
      id : currentScene
    }

    function update_location() {
      if (model.count == 0) {
        return
      }

      currentLocation
      currentLocation.location_id
      currentLocation.name = locationNameEntry.text
      currentLocation.environment = environmentEntry.text
      console.log("RP_Location updating_location on \"%1\"".arg(currentLocation.name))
      root.backend.update_location(currentLocation)
    } 
    border.color: 'black'
    border.width: 1
    ColumnLayout  {
        width: parent.width
        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: locationNameEntry
          label : "Location Name"
          placeholderText: "String Field (128 Characters)"
          onEditingFinished : {
            console.log("locationNameEntry.onEditingFinished")
            update_location()
          }
        }
//        TimeEntry {
//          Layout.fillWidth: true
//          Layout.leftMargin: 5
//          id: timeOfDayEntry
//          label : "Time of Day"
//        }
//        TextEntry {
//          Layout.fillWidth: true
//          Layout.leftMargin: 5
//          id: timeScenarioEntry
//          label : "Time in Scenario"
//          placeholderText: "Time Input Field (3H20M)"
//          onLabelWidthChanged : {
//            //nameEntry.nameWidth         = timeScenarioEntry.nameWidth
//            locationNameEntry.nameWidth = timeScenarioEntry.nameWidth
//            timeOfDayEntry.nameWidth    = timeScenarioEntry.nameWidth
//            environmentEntry.nameWidth  = timeScenarioEntry.nameWidth
//          }
//        }
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: environmentEntry
          label : "Environments"
          required: true
          placeholderText: "Weather Input Area"
          onEditingFinished : {
            console.log("environmentEntry.onEditingFinished")
            update_location()
          }
        }   

    }
    onIndexChanged : {
      var values = model.get(root.index)
      if(values && model.count != 0) {
        //nameEntry.text = values.name
        currentScene.scene_id = values.id
        root.backend.locations_in_scene(currentScene)
        if (root.backend.next_location(currentLocation)) {        
          locationNameEntry.text = currentLocation.name
          environmentEntry.text = currentLocation.environment
        } else {
          console.log("Unable to find Scenes \"%1\" location ".arg(currentScene.name))   
        }
      }
    }
    onCountChanged : {
      if(count == 0) {
          locationNameEntry.text = ""
          environmentEntry.text = ""
          locationNameEntry.text.readOnly = true
          environmentEntry.text.readOnly = true
        } else {
          locationNameEntry.text.readOnly = false
          environmentEntry.text.readOnly = false
          indexChanged()
        }
    }
}