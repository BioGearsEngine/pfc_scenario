import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

  Rectangle {
  id: narrative_rightWindow
  clip: true
  Layout.fillWidth: true
  Layout.fillHeight: true
  Layout.margins: 5
  ColumnLayout {
    anchors.fill: narrative_rightWindow
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
        id: props_tab
        text: "Props"
        width: implicitWidth
      }
      TabButton {
          id: events_tab
        text: "Events"
        width: implicitWidth
      }
    }
    StackLayout {
    	id: narrative_stack_right
    	Layout.fillHeight: true
    	Layout.fillWidth: true
    	currentIndex: narrative_tabs.currentIndex

    	RP_Location {
    		Layout.fillHeight: true
    		Layout.fillWidth: true
    	}
    	RP_Roles {
    		Layout.fillHeight: true
    		Layout.fillWidth: true
    	}
    	RP_Props {
    		Layout.fillHeight: true
    		Layout.fillWidth: true
    	}
    	RP_Events {
    		Layout.fillHeight: true
    		Layout.fillWidth: true
    	}
    }
  }

  }