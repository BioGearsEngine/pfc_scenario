import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0


ScrollView {
  id : root
  Event {
    id : self
  }

  property SQLBackend backend
  property Event currentEvent
  property int returnTo
  signal exit()
  onWidthChanged : {
    eventColumn.width = width
  }
  ColumnLayout {
    id : eventColumn
    // anchors.fill : parent
    Layout.fillWidth : true
    TextEntry {
      id : name
      Layout.fillWidth : true
      label : "Name"
      text : (currentEvent) ?currentEvent.name: "unknown"
      placeholderText : "String Field"
    }
    TextAreaEntry {
      id : description
      Layout.fillWidth : true
      label : "Description"
      text : (currentEvent) ?currentEvent.description: "unknown"
      placeholderText : "String Field"
    }
    TextAreaEntry {
      id : actor_1
      Layout.fillWidth : true
      label : "Choose Who Acts"
      text : (currentEvent) ?currentEvent.actorOne.name: "unknown"
      placeholderText : "String Field"
    }
    TextAreaEntry {
      id : actor_2
      Layout.fillWidth : true
      label : "Choose Target"
      text : (currentEvent) ?  currentEvent.actorTwo.name: "unknown"
      placeholderText : "String Field"
    }
    TextAreaEntry {
      id : details
      Layout.fillWidth : true
      label : "Details"
       text : (currentEvent) ?currentEvent.details: "unknown"
      placeholderText : "String Field"
    }
    RowLayout {
      id : fidelity
      Layout.leftMargin : 5
      Label {
        text : "Fidelity"
      }
      ComboBox {
        id : fidelitySelect
        width : 200
        model : ["Low", "Medium", "High"]
        currentIndex :{
          let index =  (currentEvent) ? fidelitySelect.find(currentEvent.fidelity) : 0
          fidelitySelect.currentIndex = (index === -1) ? 0 : index
        } 
        contentItem : Text {
          text : fidelitySelect.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
    }
    RowLayout {
      id : type
      Layout.leftMargin : 5
      Label {
        text : "Type"
      }
      ComboBox {
        id : typeSelect
        width : 200
        model : [
          "Action",
          "Dialogue",
          "Movement",
          "Sound",
          "Environment"
        ]
        currentIndex :{
          let index = (currentEvent) ?  typeSelect.find(currentEvent.category) : 0
          typeSelect.currentIndex = (index === -1) ? 0 : index
        } 
        contentItem : Text {
          text : typeSelect.displayText
          font.pointSize : 8
          verticalAlignment : Text.AlignVCenter
          elide : Text.ElideRight
        }
      }
    }
    Row {
      Layout.fillWidth : true
      Rectangle {
        id : section_1
        width : 300
        height : description.height
        color : 'transparent'
        PFCButton {
          id : saveButton
          anchors.right : section_1.right
          text : 'Save'
          onClicked : {
            currentEvent.name = name.text;
            currentEvent.description = description.text;
            currentEvent.actor_1.name = actor_1.text;
            currentEvent.actor_2.name = actor_2.text;
            currentEvent.details = details.text;
            currentEvent.fidelity = fidelity.model[fidelity.currentIndex].toUpperCase();
            currentEvent.category = type.model[fidelity.currentIndex].toUpperCase();
            root.backend.update_event(currentEvent)
            exit()
          }
        }
      }
      Rectangle {
        id : section_2
        width : 300
        height : description.height
        color : 'transparent'
        PFCButton {
          id : exitButton
          anchors.right : section_2.right
          text : 'Exit'
          onClicked : {
            root.exit()
          }
        }
      }
    }
  }
}
