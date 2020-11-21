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
  property Objective currentObjective : (objectiveList.objectiveDefinitions[objectiveList.currentIndex]) ? objectiveList.objectiveDefinitions[objectiveList.currentIndex] : objective_g

  Objective {
    id : objective_g
  }

  function update_objectives() {
    objectiveList.objectiveDefinitions = []
    let objectives = root.backend.objectives;
    for (var ii = 0; ii < objectives.length; ++ ii) {
      objectiveList.objectiveDefinitions.push(currentObjective.make());
      objectiveList.objectiveDefinitions[objectiveList.objectiveDefinitions.length - 1].assign(objectives[ii]);
    }
    objectiveList.model = objectiveList.objectiveDefinitions;
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
        var likely_id = root.backend.nextID(SQLBackend.OBJECTIVES);
        objective_g.clear(likely_id);
        root.backend.update_objective(objective_g);
      }
      onSecondButtonClicked : {
        if ( ! objectiveList.objectiveDefinitions || objectiveList.objectiveDefinitions.length < 2) {
          return
        }
        objective_g.clear();
        objective_g.assign(objectiveList.objectiveDefinitions[objectiveList.currentIndex]);
        root.backend.remove_objective(objective_g);
        update_objectives();
        objectiveList.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : objectiveList
      property var objectiveDefinitions

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
        id : objective
        color : 'transparent'
        border.color : "steelblue"
        height : objective_title_text.height + objective_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            objectiveList.currentIndex = index

          }
        }

        Text {
          id : objective_title_text
          anchors.left : objective.left
          anchors.leftMargin : 5
          text : (objectiveList.objectiveDefinitions[index]) ? objectiveList.objectiveDefinitions[index].name : ""
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : objective_value_text
          anchors.top : objective_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
            let l_description = (objectiveList.objectiveDefinitions[index]) ? objectiveList.objectiveDefinitions[index].description : "";
            if (!enabled) {
              return l_description.split("\n")[0].trim()
            } else {
              var details = l_description.split("\n")
              details.splice(0, 1)
              var details_str = details.join('\n').trim()
              return(details_str === "") ? l_description.trim() : details_str
            }
          }
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges {
            target : objective_title_text;
            enabled : true
          }
          PropertyChanges {
            target : objective_value_text;
            enabled : true
          }
        }

        onFocusChanged : {
          if (objectiveList.currentIndex == index) {
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
    update_objectives()
  }
  
  Connections {
    target : backend
    onObjectivesChanged : {
      update_objectives()
    }
  }
}
