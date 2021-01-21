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

  Location {
    id : location_g
  }
  Scene {
    id : scene_g
  }

  function update_location(location) {
    backend.update_location(location)
  }

  border.color : 'black'
  border.width : 1
  ColumnLayout {
    width : parent.width
    TextEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : locationNameEntry
      label : "Location Name"
      placeholderText : "String Field (128 Characters)"
      text : (currentLocation) ? currentLocation.name : "unknown"
      onEditingFinished : {
        if (currentLocation) {
          if (text != currentLocation.name) {
            currentLocation.name = text
            update_location(currentLocation);
          }
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5
      id : environmentEntry
      label : "Description"
      required : true
      placeholderText : "Weather Input Area"
      text : (currentLocation) ? currentLocation.description : "unknown"
      onEditingFinished : {
        if (currentLocation) {
          if (text != currentLocation.description) {
            currentLocation.description = text
            update_location(currentLocation);
          }
        }
      }
    }
  }
}
