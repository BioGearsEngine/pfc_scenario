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
      self = root.model.get(root.index)
      root.backend.select_scene(obj)
      self.name = nameEntry.text
      self.description = nameEntry.description
      self.details = nameEntry.details
      root.backend.update_scene(obj)
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
            update_scene()
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
            update_scene()
        }   
        TextAreaEntry {
          Layout.fillWidth: true
          Layout.leftMargin: 5
          id: detailEntry
          label : "Details"
          required: true
          placeholderText: "Scene Details"
          onEditingFinished : {
            update_scene()
          }
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
          descriptionEntry.text = ""
          detailEntry.text = ""
        } else {
          indexChanged()
        }
    }
}