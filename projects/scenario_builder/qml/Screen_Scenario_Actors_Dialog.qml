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
            text: "Title"
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
        ToolButton {
            text: qsTr("⋮")
            onClicked: menu.open()
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
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.alt = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Force Relation:'
          Layout.preferredWidth : 130
        }
        TextField {
          id : forceRelationField
          text : data_model.forceRelation
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.forceRelation   = text
          }
        }
      }
      RowLayout {
        Label {
          text: 'Organic Relation:'
          Layout.preferredWidth : 130
        }
        TextField {
          id : organicRelationField
          text : data_model.organicRelation
          Layout.fillWidth: true
          Layout.rightMargin : 100
          function save() {
            data_model.organicRelation   = text
          }
        }
      }
      // RowLayout {
      //   Label {
      //     text: 'Support Relation:'
      //     Layout.preferredWidth : 130
      //   }
      //   TextField {
      //     id : supportRelationField
      //     text : data_model.supportRelation
      //     Layout.fillWidth: true
      //     Layout.rightMargin : 100
      //     function save() {
      //       data_model.supportRelation   = text
      //     }
      //   }
      // }
      RowLayout {
        Label {
          text: 'Description:'
          Layout.preferredWidth : 100
        }
        TextArea {
          id :  descField
          text : data_model.description
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
        forceRelationField.save()
        organicRelationField.save()
        // supportRelationField.save()
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