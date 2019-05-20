import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

import com.ara.pfc.ScenarioModel 1.0

Page {
  id : root
  focus: true
  property ScenarioModel  data_model

  //anchors.fill : parent

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
    Repeater {
      id : model_repeater
      model: data_model
      delegate: RowLayout {
                  Label {
                    id: label_id
                    text : label + ": "
                    Layout.preferredWidth : 150
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
                    selectByMouse : true
                    wrapMode: Text.Wrap
                    horizontalAlignment : Text.AlignJustify
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
                    function save() { content = text }
                  }
                  Layout.fillWidth: true
                }
      function save () {
        for ( var i = 0 ; i < count; ++i){
          console.log(i)
          itemAt(i).children[1].save();
        }
      }
    }
    Button {
      text : "Save"
      anchors.right : parent.right
      anchors.rightMargin : 10
      onClicked : {
        console.log("clicked")
        model_repeater.save()
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