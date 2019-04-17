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
    Repeater {
      id : model_repeater
      model: data_model
      delegate: RowLayout {
                  Label {
                    id: label_id
                    text : label + ": "
                    Layout.preferredWidth : 100
                    anchors { 
                      verticalCenter : parent.verticalCenter
                    }
                    Layout.fillWidth: false
                    Layout.alignment: (content_id.width >
                    content_id.contentWidth ) ? Qt.AlignCenter:Qt.AlignTop
                  }
                  TextField   {
                    id: content_id
                    text : content
                    wrapMode: Text.Wrap
                    horizontalAlignment : Text.AlignJustify
                    selectByMouse : true
                    anchors { 
                      left : label_id.right
                      verticalCenter : parent.verticalCenter
                    }
                    Layout.fillWidth: true
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.IBeamCursor
                        acceptedButtons: Qt.NoButton
                    }
                  }
                  Layout.fillWidth: true
                }
    }
    Button {
      text : "Save"
      anchors.right : parent.right
      anchors.rightMargin : 10
      onClicked : {
        for ( var i = 0 ; i < root.data_model.count; ++i){
          root.data_model.get(i).content = model_repeater.itemAt(i).children[1].text
        }
        closed();
      }
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