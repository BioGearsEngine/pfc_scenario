import QtQuick 2.12
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
  property Assessment currentAssessment : (assessmentList.assessmentDefinitions[assessmentList.currentIndex]) ? assessmentList.assessmentDefinitions[assessmentList.currentIndex] : assessment_g


  Assessment {
    id : assessment_g
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
        var likely_id = root.backend.nextID(SQLBackend.ASSESSMENTS);
        assessment_g.clear(likely_id);
        root.backend.update_assessment(assessment_g);
        assessmentList.currentIndex = assessmentList.assessmentDefinitions.length - 1;
        set_current_index(assessment_g.uuid);
      }
      onSecondButtonClicked : {
        if (!assessmentList.assessmentDefinitions || assessmentList.assessmentDefinitions.length < 2) {
          return
        }
        assessment_g.clear();
        assessment_g.assign(assessmentList.assessmentDefinitions[assessmentList.currentIndex]);
        root.backend.remove_assessment(assessment_g);
        update_assessments();
        assessmentList.currentIndex = Math.max(0, root.index - 1);
      }
    }

    ListView {
      id : assessmentList
      property var assessmentDefinitions;
      focus:true

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
        id : assessment
        property var currentDef: assessmentList.assessmentDefinitions[index]
        color : 'transparent'
        border.color : "steelblue"
        height : assessment_title_text.height + assessment_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            assessmentList.currentIndex = index

          }
        }

        Text {
          id : assessment_title_text
          anchors.left : assessment.left
          anchors.leftMargin : 5
          text : (currentDef) ? currentDef.name : ""
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : assessment_value_text
          anchors.top : assessment_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
            let l_description = (currentDef) ? currentDef.description : "";
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
            target : assessment_title_text;
            enabled : true
          }
          PropertyChanges {
            target : assessment_value_text;
            enabled : true
          }
        }

        onFocusChanged : {
          if (assessmentList.currentIndex == index) {
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
    update_assessments()
  }

  Connections {
    target : backend
    onAssessmentsChanged : {
      update_assessments()
    }
  }

  function set_current_index(uuid) {
    for (var ii = 0; ii < assessmentList.assessmentDefinitions.length; ++ ii) {
      if (assessmentList.assessmentDefinitions[ii].uuid == uuid) {
        assessmentList.currentIndex = ii
      }
    }
  }
  function update_assessments() {
    var index = (assessmentList.currentIndex >= 0) ? assessmentList.currentIndex : 0
    assessmentList.assessmentDefinitions = []
    let assessments = root.backend.assessments;
    for (var ii = 0; ii < assessments.length; ++ ii) {
      assessmentList.assessmentDefinitions.push(currentAssessment.make());
      assessmentList.assessmentDefinitions[assessmentList.assessmentDefinitions.length - 1].assign(assessments[ii]);
    }
    assessmentList.model = assessmentList.assessmentDefinitions;
    assessmentList.currentIndex = index
  }
}
