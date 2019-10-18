import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0




ColumnLayout {
	id: root

Event {
	id: self
}
	property SQLBackend backend
	property ListElement modelData
	property alias name : name.text
	property alias description : description.text
	property int eventID
	signal exit() 
	Layout.fillWidth: true
	Layout.fillHeight: true

	TextEntry {
		id: name
		Layout.fillWidth : true
		label: "Name"
		placeholderText: "String Field"
	}
	TextAreaEntry {
		id: description
		Layout.fillWidth : true
		label: "Description"
		placeholderText: "String Field"
	}
	Rectangle {
		id: section_1
		width: 300
		height: description.height
		color: 'transparent'
		PFCButton {
			id: saveButton
			anchors.right : section_1.right
			text : 'Save'
			onClicked : {
				console.log('ID:'+root.eventID+',NAME:'+root.name+',DESCRIPTION:'+root.description)
				self.event_id = root.eventID
				self.name = root.name
				self.description = root.description
				root.backend.update_event(self)
			}
		}
	}
	Rectangle {
		id: section_2
		width: 300
		height: description.height
		color: 'transparent'
		PFCButton {
			id: exitButton
			anchors.right : section_2.right
			text : 'Exit'
			onClicked : {
				exit()
			}
		}
	}
}
