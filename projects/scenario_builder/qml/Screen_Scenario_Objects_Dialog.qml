import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Page {
  id : root
  focus: true
  property var data_model
  //anchors.fill : parent

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
          text : data_model.name
        }
      }
    }

    Button {
      text : "Save"
      anchors.right : parent.right
      anchors.rightMargin : 10
      onClicked : {
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