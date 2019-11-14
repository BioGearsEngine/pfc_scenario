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
    border.color: 'black'
    border.width: 1
    ColumnLayout  {
        width: parent.width
        function update_location(values) {
          obj.location_id = values.id
          obj.name = values.name
          obj.scene_name = values.scene_name
          obj.time_of_day = values.time_of_day
          obj.environment = values.environment
          obj.fk_scene = values.fk_scene  
          root.backend.update_location(obj)
        }   

        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: nameEntry
          label : "Scene Name"
          placeholderText: "String Field (128 Characters)"
          onEditingFinished : {
            var entry = root.model.get(root.index)
            if ( text != entry.name){
              entry.name = text
              update_objective(entry)
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
      if(values) {
        nameEntry.text = root.model.get(root.index).name
      }
    }
}