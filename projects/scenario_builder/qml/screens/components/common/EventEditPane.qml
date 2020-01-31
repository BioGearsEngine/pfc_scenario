import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0



ScrollView {
	id: root
	Layout.fillWidth: true
	Layout.fillHeight: true
	property SQLBackend backend
	property ListElement modelData
	property alias name : name.text
	property alias description : description.text
	property int eventID
	signal exit() 
	onWidthChanged: {
		eventColumn.width = width
	}
ColumnLayout {
	id: eventColumn
Event {
	id: self
}
	//anchors.fill : parent
	Layout.fillWidth: true
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
	TextAreaEntry {
		id: actor_1
		Layout.fillWidth : true
		label : "Actor 1"
		placeholderText : "String Field"
	}
	TextAreaEntry {
		id: actor_2
		Layout.fillWidth : true
		label : "Actor 2"
		placeholderText : "String Field"
	}
	TextAreaEntry {
		id: details
		Layout.fillWidth : true
		label : "Details"
		placeholderText : "String Field"
	}
    RowLayout {
      id : fidelity
      Layout.leftMargin: 5
      Label {
        text : "Fidelity"
      }
      ComboBox {
          id : fidelitySelect
          width: 200
          model: [ "Low", "Medium", "High"]

          contentItem: Text {

              text: typeSelect.displayText
              font.pointSize: 8
              verticalAlignment: Text.AlignVCenter
              elide: Text.ElideRight
          }
      }
    }	
    RowLayout {
      id : type
      Layout.leftMargin: 5
      Label {
        text : "Type"
      }
      ComboBox {
          id : typeSelect
          width: 200
          model: [ "Action", "Dialogue", "Movement", "Sound", "Environment"]

          contentItem: Text {
              text: typeSelect.displayText
              font.pointSize: 8
              verticalAlignment: Text.AlignVCenter
              elide: Text.ElideRight
          }
      }
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
				root.exit()
			}
		}
	}

}

}