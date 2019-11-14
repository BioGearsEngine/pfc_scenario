import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0




ColumnLayout {
	id: root

Role {
	id: self
}
	property SQLBackend backend
	property ListElement modelData
	property alias name : name.text
	property alias description : description.text
	property int roleID
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
				self.role_id = root.roleID
				self.name = root.name
				self.description = root.description
				root.backend.update_role(self)
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
