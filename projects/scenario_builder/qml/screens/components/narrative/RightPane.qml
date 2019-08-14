import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property alias currentIndex : narrative_tabs.currentIndex
  property alias roleModel : roles.model

  property alias roleIndex : roles.index

  property SQLBackend backend
  property ListElement modelData

  clip: true
  Layout.fillWidth: true
  Layout.fillHeight: true
  Layout.margins: 5
  ColumnLayout {
    anchors.fill: root
    width: parent.width
    height: parent.height
    TabBar {
      id: narrative_tabs
      Layout.fillWidth: true
      font.pointSize: 8
      height : 12
      width: parent.width

      TabButton {
        id: location_tab
        text: "Location"
        width: implicitWidth
      }
      TabButton {
        id: roles_tab
        text: "Roles"
        width: implicitWidth
      }
      TabButton {
        id: events_tab
        text: "Events"
        width: implicitWidth
      }
      currentIndex: 1
    }
    StackLayout {
    	id: narrative_stack_right
    	Layout.fillHeight: true
    	Layout.fillWidth: true
    	currentIndex: narrative_tabs.currentIndex

    	RP_Location {
        id: locations
    		Layout.fillHeight: true
    		Layout.fillWidth: true
        backend : root.backend
    	}
    	RP_Roles {
        id: roles
    		Layout.fillHeight: true
    		Layout.fillWidth: true
        backend : root.backend
    	}
    	RP_Events {
        id: events
    		Layout.fillHeight: true
    		Layout.fillWidth: true
        backend : root.backend
    	}
    }
  }
}