import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property SQLBackend backend
  property ListModel model
  property int index
  property int count

  /***Helper function to convert length of time to clock time for display purposes***/
  function seconds_to_clock_time(time_s) {

    var v_seconds = time_s % 60;
    var v_minutes = Math.floor(time_s / 60) % 60;
    var v_hours = Math.floor(time_s / 3600);
    v_seconds = (v_seconds < 10) ? "0%1".arg(v_seconds) : "%1".arg(v_seconds);
    v_minutes = (v_minutes < 10) ? "0%1".arg(v_minutes) : "%1".arg(v_minutes);
    v_hours = (v_hours < 10) ? "0%1".arg(v_hours) : "%1".arg(v_hours);
    return "%1:%2:%3".arg(v_hours).arg(v_minutes).arg(v_seconds);
  }

  function clock_time_to_seconds(timeString) {

    let timeUnits = timeString.split(':'); // splits into [hh, mm, ss]
    try {
      let hours = Number.fromLocaleString(timeUnits[0]);
      let minutes = Number.fromLocaleString(timeUnits[1]);
      let seconds = Number.fromLocaleString(timeUnits[2]);
      if (hours < 0.0 || minutes < 0.0 || seconds < 0.0) {
        throw "Negative time";
      }
      return 3600 * hours + 60 * minutes + seconds;
    } catch (err) {
      return null;
    }
  }

  function setup_details(detail_string) {
    let categories = detail_string.split(':')
    let type = categories[0]
    let kind = categories[1]
    let style = categories[2]

    let index = typeBox.find(type)
    typeBox.currentIndex = (index === -1) ? 0 : index

    index = kindBox.find(kind)
    kindBox.currentIndex = (index === -1) ? 0 : index

    index = styleBox.find(style)
    styleBox.currentIndex = (index === -1) ? 0 : index

  }


  Scene {
    id : self
  }

  function update_scene(values) {

    if (model.count == 0) {
      return
    }
    self.scene_id = values.id;
    self.name = nameEntry.text;
    self.description = descriptionEntry.text;
    self.details = detailEntry.text;
    self.weather = weatherEntry.text;
    self.time_of_day = clock_time_to_seconds(timeEntry.text);
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

    // /////////////////////////////////////////////////////////////////
    RowLayout {
      id : detailEntry

      property alias typeBox : typeBox
      property alias kindBox : kindBox
      property alias styleBox : styleBox
      property bool loadComplete : false

      property string text : ""
      Layout.fillWidth : true
      Layout.preferredHeight : typeBox.height
      Layout.leftMargin : 5
      Layout.rightMargin : 20

      Label {
        id : categoryLabel
        text : "Details:"
        font.pointSize : timeEntry.pointSize
        color : "steelblue"
        width : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : typeBox
        currentIndex : 0
        editable : true

        model : ["Exterior", "Interior"]
        width : 200
        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
        onCurrentIndexChanged : {
          var entry = root.model.get(root.index);
          if (detailEntry.text != entry.details && detailEntry.loadComplete) {
            detailEntry.text = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            update_scene(entry)
          }
        }
      }
      ComboBox {
        id : kindBox
        currentIndex : 0
        editable : true
        model : (typeBox.currentIndex == 0) ? ["Tesstrial", "Marine"] : ["Temporary", "Perminate"]
        width : 200

        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
        onCurrentIndexChanged : {
          var entry = root.model.get(root.index);
          if (detailEntry.text != entry.details && detailEntry.loadComplete) {
            detailEntry.text = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            update_scene(entry)
          }
        }
      }
      ComboBox {
        id : styleBox
        currentIndex : 0
        editable : true
        model : {
          if (typeBox.currentIndex == 0) {
            switch (kindBox.currentIndex) {
              case 0:
                return [
                  "Temperate deciduous forest",
                  "Coniferous forest",
                  "Woodland",
                  "Chaparral",
                  "Tundra",
                  "Grassland",
                  "Desert",
                  "Tropical savanna",
                  "Tropical forest"
                ];
              case 1:
                return [
                  "River Bed",
                  "Swamp",
                  "Lake",
                  "Ocean",
                  "Bracish Water",
                  "Coral reef"
                ];
            }
          } else {
            switch (kindBox.currentIndex) {
              case 0:
                return ["Plywood", "Prefrab", "Mobile", "Decayed"];
              case 1:
                return ["Classical", "Modern", "Ruined", "Futuristic"];
            }
          }
        }

        width : 200
        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
        onCurrentIndexChanged : {
          var entry = root.model.get(root.index);
          if (detailEntry.text != entry.details && detailEntry.loadComplete) {
            detailEntry.text = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            update_scene(entry)
          }
        }
      }
    }
    // /////////////////////////////////////////////////////////////////
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
      entryField.inputMask : "99:99:99"
      entryField.validator : RegExpValidator {
        regExp : /^([0-1\s]?[0-9\s]|2[0-3\s]):([0-5\s][0-9\s]):([0-5\s][0-9\s])$ /
      }
      onEditingFinished : {
        var seconds = clock_time_to_seconds(text)
        var entry = root.model.get(root.index);
        if (seconds != entry.time_of_day) {
          entry.time_of_day = seconds
          update_scene(entry)
        }
      }
      Keys.onBackPressed : {
        var index = entryField.cursorPosition;
        var value = entryField.text.charAt(index);
        if (value != ":") {
          entryField.text = entryField.text.substr(0, index) + "0" + entryField.text.substr(index);
        }
      }
    }
  }
  onIndexChanged : {
    var values = model.get(root.index);
    if (values && model.count != 0) {
      detailEntry.loadComplete = false
      nameEntry.text = values.name;
      descriptionEntry.text = values.description;
      setup_details(values.details)

      weatherEntry.text = values.weather;
      if (Number.isInteger(Number(values.time))) {
        timeEntry.text = seconds_to_clock_time(values.time);
      } else {

        timeEntry.text = "06:00:00"
      }
      detailEntry.loadComplete = true
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
