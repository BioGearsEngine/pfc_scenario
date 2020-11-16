import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation

  Scene {
    id : scene_g
  }
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

  function update_scene(scene) {
    scene_g.timeOfDay = clock_time_to_seconds(timeEntry.text);
    root.backend.update_scene(scene)
  }
    function update_location(location) {
    
    root.backend.update_location(location)
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
      text : (currentScene) ? currentScene.name : ""
      onEditingFinished : {
        if (text != currentScene.name) {
          currentScene.name = text
          update_scene(currentScene)
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
      text : (currentScene) ? currentScene.description : ""
      onEditingFinished : {
        if (text != currentScene.description) {
          currentScene.description = text
          update_scene(currentScene)
        }
      }
    }

    // -----------------------Begin Category Selection -------------------------------------//
    Item {
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
        text : "Category:"
        font.pointSize : timeEntry.pointSize
        color : "steelblue"
        width : (text.width > 90) ? text.width + 10 : 100
      }
      ComboBox {
        id : typeBox
        currentIndex : 0
        editable : true
        anchors {
          left : categoryLabel.right
        }
        model : ["Exterior", "Interior"]
        width : 100
        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
        onCurrentIndexChanged : {
          if (currentLocation && detailEntry.loadComplete) {
            var environmentStr = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            if (environmentStr != currentLocation.environment ) {              
              currentLocation.environment = environmentStr
              update_location(currentLocation)
            }
          }
        }
      }
      ComboBox {
        id : kindBox
        currentIndex : 0
        editable : true
        anchors {
          left : typeBox.right
        }
        model : (typeBox.currentIndex == 0) ? ["Terrestrial", "Marine"] : ["Temporary", "Perminate"]
        width : 120

        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
        onCurrentIndexChanged : {
          if (currentLocation && detailEntry.loadComplete) {
            var environmentStr = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            if (environmentStr != currentLocation.environment ) {              
              currentLocation.environment = environmentStr
              update_location(currentLocation)
            }
          }
        }
      }
      ComboBox {
        id : styleBox
        currentIndex : 0
        editable : true
        anchors {
          left : kindBox.right
          right : parent.right
        }
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
        onModelChanged : {
          currentIndex = 0;
        }
        onAccepted : {
          if (find(editText) === -1) {
            currentIndex = 0;
          }
        }
         onCurrentIndexChanged : {
          if (currentLocation && detailEntry.loadComplete) {
            var environmentStr = "%1:%2:%3".arg(typeBox.model[typeBox.currentIndex]).arg(kindBox.model[kindBox.currentIndex]).arg(styleBox.model[styleBox.currentIndex]);
            if (environmentStr != currentLocation.environment ) {              
              currentLocation.environment = environmentStr
              update_location(currentLocation)
            }
          }
        }
      }
    }
    // -----------------------END Category Selection -------------------------------------//
    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : weatherEntry
      label : "Weather"
      required : true
      placeholderText : "Weather Details"
      text : (currentScene) ? currentScene.weather : ""
      onEditingFinished : {
        if (text != currentScene.weather) {
          currentScene.weather = text
          update_scene(currentScene)
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
      // entryField.validator : RegExpValidator {
      //   regExp : /^([0-1\s]?[0-9\s]|2[0-3\s]):([0-5\s][0-9\s]):([0-5\s][0-9\s])$ /
      // }
      text : (currentScene) ? seconds_to_clock_time(currentScene.timeOfDay) : "06:00:00"
      onEditingFinished : {
        var secondsInDay = clock_time_to_seconds(text);
        if (secondsInDay != currentScene.timeOfDay) {
          currentScene.timeOfDay = secondsInDay;
          update_scene(currentScene);
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

  onCurrentLocationChanged : {
    setup_details(currentLocation.environment)
  }
  Component.onCompleted : {
    scene_g.clear()
    if(currentLocation){
      setup_details(currentLocation.environment)
    }
    detailEntry.loadComplete = true
  }
  //   onBackendChanged : {
  //   if (backend) {
  //     backend.citationsChanged.connect(refresh_citations);
  //     backend.equipmentChanged.connect(refresh_equipment);
  //   }
  // }
}
