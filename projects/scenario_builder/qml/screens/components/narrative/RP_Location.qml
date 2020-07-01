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
      obj.location_id = self.location_id
      obj.name = locationNameEntry.text
      obj.environment = environmentEntry.text
      root.backend.update_location(obj)
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
            update_location()
          }
        }   

    }
    onIndexChanged : {
      var values = model.get(root.index)
      if(values && model.count != 0) {
        update_location()
        //nameEntry.text = root.model.get(root.index).name
        selfScene.scene_id = model.get(root.index).id
        root.backend.locations_in_scene(selfScene)
        while ( root.backend.next_location(self) ) {
          locationNameEntry.text = self.name
          environmentEntry.text = self.environment
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