import QtQuick 2.10

import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../../common"
import "right"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property alias currentIndex : narrative_tabs.currentIndex
  property int topIndex : 0
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation


  clip : true
  Layout.fillWidth : true
  Layout.fillHeight : true
  Layout.margins : 5
  ColumnLayout {
    anchors.fill : root
    Shortcut {
      id : f1
      sequence : "F1"
      enabled : false
      onActivated : {
        narrative_tabs.currentIndex = 0
      }
    }
    Shortcut {
      id : f2
      sequence : "F2"
      enabled : false
      onActivated : {
        narrative_tabs.currentIndex = 1
      }
    }
    Shortcut {
      id : f3
      sequence : "F3"
      enabled : false
      onActivated : {
        narrative_tabs.currentIndex = 2
      }
    }
    Shortcut {
      id : f4
      sequence : "F4"
      enabled : false
      onActivated : {
        narrative_tabs.currentIndex = 3
      }
    }
    Shortcut {
      id : f5
      sequence : "F5"
      enabled : false
      onActivated : {
        narrative_tabs.currentIndex = 4
      }
    }
    width : parent.width
    height : parent.height
    TabBar {
      id : narrative_tabs
      Layout.fillWidth : true
      font.pointSize : 8
      height : 12
      width : parent.width

      TabButton {
        id : scenes_tab
        text : "Details"
        width : implicitWidth
      }
      TabButton {
        id : location_tab
        text : "Location"
        width : implicitWidth
      }
      TabButton {
        id : roles_tab
        text : "Roles"
        width : implicitWidth
      }
      TabButton {
        id : events_tab
        text : "Events"
        width : implicitWidth
      }
      TabButton {
        id : objects_tab
        text : "Objects"
        width : implicitWidth
      }
      currentIndex : 0
    }
    StackLayout {
      id : narrative_stack_right
      Layout.fillHeight : true
      Layout.fillWidth : true
      currentIndex : narrative_tabs.currentIndex

      TabScene {
        id : scenes
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        currentScene : root.currentScene
        currentLocation : root.currentLocation
      }
      TabLocation {
        id : locations
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        currentScene : root.currentScene
        currentLocation : root.currentLocation
      }
      TabRoles {
        id : roles
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        currentScene : root.currentScene
        currentLocation : root.currentLocation
      }
      TabEvents {
        id : events
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        currentScene : root.currentScene
        currentLocation : root.currentLocation
      }
      TabEquipment {
        id : objects
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        currentScene : root.currentScene
        currentLocation : root.currentLocation
        Rectangle {
          width : parent.width
          height : parent.height
          color : "transparent"
          border.color : "cyan"
        }
      }
    }
  }
  onTopIndexChanged : {
    if (topIndex == 2) {
      f1.enabled = true
      f2.enabled = true
      f3.enabled = true
      f4.enabled = true
    } else {
      f1.enabled = false
      f2.enabled = false
      f3.enabled = false
      f4.enabled = false
    }
  }
}
