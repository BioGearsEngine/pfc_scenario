import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
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
    self.time_of_day = timeEntry.text
    root.backend.update_scene(self)
  }

  border.color : 'black'
  border.width : 1
  ColumnLayout {
    width : parent.width


    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : nameEntry
      label : "Scene Name"
      placeholderText : "String Field (128 Characters)"
      onEditingFinished : {
        var entry = root.model.get(root.index);
        if (text != entry.name) {
          entry.name = text
          update_scene(entry)
        }
      }
    }
    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : descriptionEntry
      label : "Description"
      required : true
      placeholderText : "Scene Description"
      onEditingFinished : {
        var entry = root.model.get(root.index);
        if (text != entry.description) {
          entry.description = text;
          update_scene(root.model.get(root.index))
        }
      }
    }
    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : detailEntry
      label : "Details"
      required : true
      placeholderText : "Scene Details"
      onEditingFinished : {
        var entry = root.model.get(root.index);
        if (text != entry.details) {
          entry.details = text;
          update_scene(root.model.get(root.index))
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : weatherEntry
      label : "Weather"
      required : true
      placeholderText : "Weather Details"
      onEditingFinished : {
        var entry = root.model.get(root.index);
        if (text != entry.weather) {
          entry.weather = text;
          update_scene(root.model.get(root.index))
        }
      }
    }
    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : timeEntry
      label : "Time in Scenario"
      placeholderText : "06:00"
      inputMethodHints : Qt.ImhTime
      onEditingFinished : {
        console.log(text)
      }
    }

    // /////////////////////////////////////////////////////////////////
    RowLayout {
      id : dropDownArea
      Layout.fillWidth : true
      Layout.preferredHeight : flickable.height
      Layout.leftMargin : 5
      Layout.rightMargin : 20

      Label {
        id : categoryLabel
        text : 'Details:'
        font.pointSize : root.pointSize
        color : "steelblue"
        width : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : typeBox
        currentIndex : 0
        editable : true

        model : ["Interior", "Exterior"]
        width : 200
        onAccepted: {
        if (find(editText) === -1)
            currentIndex = 0
        }
      }
      ComboBox {
        id : kindBox

        currentIndex : 0
        editable : true
        model : (typeBox.currentIndex)? ["Tundra", "Desert"] : ["Shack", "Urban"]
        width : 200

        onModelChanged : {
          currentIndex = 0
        }
        onAccepted: {
        if (find(editText) === -1)
            currentIndex = 0
        }
      }
      ComboBox {
        id : styleBox

        currentIndex : 0
        editable : true
        model : (typeBox.currentIndex)? ["Urban", "Rural"] : ["Prefab", "Shambles"]
        width : 200
        onAccepted: {
        if (find(editText) === -1)
            currentIndex = 0
        }
        onCurrentIndexChanged : console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).color)
      }
    }
    // /////////////////////////////////////////////////////////////////
  }
    onIndexChanged : {
      var values = model.get(root.index);
      if (values && model.count != 0) {
        nameEntry.text = values.name;
        descriptionEntry.text = values.description;
        detailEntry.text = values.details;
        weatherEntry.text = values.weather;
        timeEntry.text = values.time;

        console.log("nameEntry.text = $1\n".arg(values.name), "descriptionEntry.text = $1\n".arg(values.description), "detailEntry.text = $1\n".arg(values.details), "weatherEntry.text =  $1\n".arg(values.weather), "timeEntry.text =  $1\n".arg(values.time))
      }
    }
    onCountChanged : {
      if (count == 0) {
        nameEntry.text = ""
        descriptionEntry.text = ""
        detailEntry.text = ""
        weatherEntry.text = ""
        timeEntry.text = ""

      } else {
        indexChanged()
      }
    }
}
