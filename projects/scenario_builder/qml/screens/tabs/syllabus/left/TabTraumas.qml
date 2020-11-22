import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property int topIndex
  property Trauma currentTrauma : (traumaList.traumaDefinitions[traumaList.currentIndex]) ? traumaList.traumaDefinitions[traumaList.currentIndex] : trauma_g

  Trauma {
    id : trauma_g
  }

  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight : true

    border.color : "black"

    TwoButtonRow {
      id : controls
      anchors.top : listRectangle.top
      anchors.left : listRectangle.left
      anchors.right : listRectangle.right
      anchors.topMargin : 2
      anchors.rightMargin : 5
      anchors.leftMargin : 5

      property int next : 1

      firstButtonText : "Add"
      secondButtonText : "Remove"

      onFirstButtonClicked : {
        var likely_id = root.backend.nextID(SQLBackend.TRAUMAS);
        trauma_g.clear(likely_id);
        root.backend.update_trauma(trauma_g);
      }
      onSecondButtonClicked : {
        if ( ! traumaList.traumaDefinitions || traumaList.traumaDefinitions.length < 2) {
          return
        }
        trauma_g.clear();
        trauma_g.assign(traumaList.traumaDefinitions[traumaList.currentIndex]);
        root.backend.remove_trauma(trauma_g);
        update_traumas();
        traumaList.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : traumaList
      property var traumaDefinitions
      anchors {
        top : controls.bottom;
        bottom : parent.bottom;
        left : parent.left;
        right : parent.right
      }
      spacing : 5
      clip : true
      highlightFollowsCurrentItem : true
      highlightMoveDuration : 1
      highlight : Rectangle {
        color : '#1111110F'
        Layout.alignment : Qt.AlignTop
        Layout.fillWidth : true
        Layout.margins : 5
      }

      model : ListModel {}

      delegate : Rectangle {
        id : trauma
        color : 'transparent'
        border.color : "steelblue"
        height : trauma_title_text.height + trauma_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            traumaList.currentIndex = index
          }
        }

        Text {
          id : trauma_title_text
          anchors.left : trauma.left
          anchors.leftMargin : 5
          text : (traumaList.traumaDefinitions[index]) ? traumaList.traumaDefinitions[index].medicalName : ""
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : trauma_value_text
          anchors.top : trauma_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
            if (traumaList.traumaDefinitions[index]) {
              if (!enabled) {
                return traumaList.traumaDefinitions[index].commonName;
              } else {
                if (traumaList.traumaDefinitions[index].description === "") {
                  return traumaList.traumaDefinitions[index].commonName;
                } else {
                  return traumaList.traumaDefinitions[index].description;
                }
              }
            } else {
              return "";
            }
          }
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges {
            target : trauma_title_text;
            enabled : true
          }
          PropertyChanges {
            target : trauma_value_text;
            enabled : true
          }
        }

        onFocusChanged : {
          if (traumaList.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}

    }
  }

  Component.onCompleted : {
    update_traumas()
  }

  Connections {
    target : backend
    onTraumasChanged : {
      update_traumas()
    }
    onCitationsChanged : {
      update_traumas()
    }
  }

  function set_current_index( uuid ) {
    for (var ii = 0; ii < traumaList.traumaDefinitions.length; ++ ii) {
      if ( traumaList.traumaDefinitions[ii].uuid == uuid ){
        traumaList.currentIndex = ii
      }
    }
  }

  function update_traumas() {
    var index = ( traumaList.currentIndex >= 0) ? traumaList.currentIndex : 0
    traumaList.traumaDefinitions = [];
    let traumas = root.backend.traumas;
    for (var ii = 0; ii < traumas.length; ++ ii) {
      traumaList.traumaDefinitions.push(currentTrauma.make());
      traumaList.traumaDefinitions[traumaList.traumaDefinitions.length - 1].assign(traumas[ii]);
      set_current_index(trauma_g.uuid)
    }
    traumaList.model = traumaList.traumaDefinitions;
    traumaList.currentIndex = index
  }
}
