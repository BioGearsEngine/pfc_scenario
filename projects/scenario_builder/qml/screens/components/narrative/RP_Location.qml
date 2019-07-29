import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

//Rectangle {
//	id: root
//	property SQLBackend backend
//	focus: true
//	Layout.fillWidth: true
//	Layout.fillHeight: true
//	Layout.margins: 5
//	border.color: 'black'//

//	Row {
//		Rectangle {//

//		}
//		Rectangle {
//			
//		}
//	}
//}

ColumnLayout  {
    id: root
    property SQLBackend backend
    focus: true
    Layout.fillWidth: true
    Layout.fillHeight: true

    TextEntry {
      Layout.fillWidth: true
      id: nameEntry
      label : "Scene Name"
      placeholderText: "String Field (128 Characters)"
    }

    TextEntry {
      Layout.fillWidth: true
      id: locationNameEntry
      label : "Location Name"
      placeholderText: "String Field (128 Characters)"
    }

    Row {
    	Layout.fillWidth: true
    	id: timeOfDayEntry
    	height: locationNameEntry.height
    	Rectangle {
    		width: 20 + hourTime.up.implicitIndicatorWidth
    		height: parent.height
    		Text {
    			text: "Time of Day"
    		}
    	}
    	SpinBox {
    		id: hourTime
    		width: parent.width / 3
    		value: 23
    		from: 0
    		to: 23
    		editable: true
    	}
    	Rectangle {
    		width: 20
    		height: parent.height
    		Text {
    			text: ":"
    		}
    	}
    	SpinBox {
    		id: minuteTime
    		width: parent.width / 3
    		from: 0
    		to: 59
    		value: 59
    		editable: true
    	}
    }
    TextEntry {
      Layout.fillWidth: true
      id: locationNameEntry
      label : "Time in Scenario"
      placeholderText: "Time Input Field (3H20M)"
    }
}
