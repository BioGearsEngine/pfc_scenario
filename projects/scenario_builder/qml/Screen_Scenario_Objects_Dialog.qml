import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
  id : root
  focus: true
  property var data_model

  header: ToolBar {
    RowLayout {
        anchors.fill: parent
        ToolButton {
            text: qsTr("‹")
            onClicked: closed();
        }
       Label {
            text: "Edit: " + data_model.symbol
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
    }
    
  }
  Flickable {
    id : mainArea
    anchors.fill : parent
    contentHeight: flickableColumn.height
    ColumnLayout {
      id : flickableColumn
      anchors.left : parent.left
      anchors.right : parent.right
      spacing: 5
      RowLayout {
        spacing: 5
        Label {
          text: 'Name:'
          Layout.preferredWidth : 100
          anchors { 
            verticalCenter : parent.verticalCenter
          }
          Layout.fillWidth: false
        }
        TextField {
          id : nameField
          text : data_model.name
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.name = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'ID:'
          Layout.preferredWidth : 100
        }
        TextField {
          id : idField
          text : data_model.id
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          readOnly : true
          function save() {
            data_model.id = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Symbol ID:'
          Layout.preferredWidth : 100
        }
        TextField {
          id : symbolField
          text : data_model.symbol
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.symbol = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Latitude:'
          Layout.preferredWidth : 100
        }
        TextField {
          id : latField
          text : data_model.lat
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.lat = parseFloat(text.replace(",", ""))
          }
        }
      }
      RowLayout {
        Label {
          text: 'Longitude:'
          Layout.preferredWidth : 100
        }
        TextField {
          id : lonField
          text : data_model.lon
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          inputMethodHints: Qt.ImhDigitsOnly
          function save() {
            data_model.lon = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Altitude:'
          Layout.preferredWidth : 100
        }
        TextField {
          id : altField
          text : data_model.alt
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.alt = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Organic Relation ID:'
          Layout.preferredWidth : 130
        }
        TextField {
          id : organicRelationField
          text : data_model.origin
          selectByMouse : true
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.origin   = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Description:'
          Layout.preferredWidth : 100
        }
        TextArea {
          id :  descField
          text : data_model.description
          selectByMouse : true
          wrapMode: Text.Wrap
          horizontalAlignment : Text.AlignJustify
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.description = text
          }
        }
      }
      
    }

    Button {
      text : "Save"
      anchors.right : parent.right
      anchors.rightMargin : 10
      onClicked : {
        console.log("Saving " + root.data_model.id)
        nameField.save()
        symbolField.save()
        latField.save()
        lonField.save()
        altField.save()
        organicRelationField.save()
        descField.save()
        closed();
      }
    }

    Keys.onBackPressed: {
      console.log("onBackPressed");
      closed();
    }
    Keys.onEscapePressed: {
      console.log("onEscapePressed");
      closed();
    }
    Keys.onPressed: {
      console.log("onPressed");
      if (event.key == Qt.Key_Backspace) {
        console.log("backspace");
        event.accepted = true;
        closed();
      }
    }
  }
}