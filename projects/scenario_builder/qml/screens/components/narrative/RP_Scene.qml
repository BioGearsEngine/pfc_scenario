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

    Scene {
      id : self
    }

    function update_scene(values) {
      if (model.count == 0) {
        return
      }
      self.scene_id = values.id
      self.name = nameEntry.text
      self.description = descriptionEntry.text
      self.details = detailEntry.text
      self.weather = weatherEntry.text
      root.backend.update_scene(self)
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
            if (text != entry.name) {
              entry.name = text
              update_scene(entry)
            }
          }
        }   
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: descriptionEntry
          label : "Description"
          required: true
          placeholderText: "Scene Description"
          onEditingFinished : {
            update_scene(root.model.get(root.index))
          }
        }   
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: detailEntry
          label : "Details"
          required: true
          placeholderText: "Scene Details"
          onEditingFinished : {
            update_scene(root.model.get(root.index))
          }  
      }
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: weatherEntry
          label : "Weather"
          required: true
          placeholderText: "Weather Details"
          onEditingFinished : {
            update_scene(root.model.get(root.index))
          }  
      }
        TextEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: timeEntry
          label : "Time in Scenario"
          placeholderText: "Time Input Field (3H20M)"
          onLabelWidthChanged : {
            //nameEntry.nameWidth         = timeScenarioEntry.nameWidth
            //locationNameEntry.nameWidth = timeScenarioEntry.nameWidth
            //timeOfDayEntry.nameWidth    = timeScenarioEntry.nameWidth
            //environmentEntry.nameWidth  = timeScenarioEntry.nameWidth
          }
        } 
    }
    onIndexChanged : {
      var values = model.get(root.index)
      if(values && model.count != 0) {
        nameEntry.text = values.name
        descriptionEntry.text = values.description
        detailEntry.text = values.details
      }
    }
    onCountChanged : {
      if(count == 0) {
          nameEntry.text = ""
          descriptionEntry.text = ""
          detailEntry.text = ""
        } else {
          indexChanged()
        }
    }
}