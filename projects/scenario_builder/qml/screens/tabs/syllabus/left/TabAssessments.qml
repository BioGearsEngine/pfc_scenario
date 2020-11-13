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
  property Assessment currentAssessment : ( assessmentList.assessmentDefinitions[assessmentList.currentIndex] ) ? assessmentList.assessmentDefinitions[assessmentList.currentIndex] : currentAssessment


  Assessment {
    id : self
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
        if (next < root.model.count) {
          next = root.model.count + 1
        }
        self.assessment_id = -1;
        self.name = "New Assessment %1".arg(next);
        self.description = "Description of Assessment %1".arg(next);
        self.type = "binary" // vs assessment
        self.available_points = "1";
        self.criteria = "Unknown Criteria";
        while (root.backend.select_assessment(self)) {
          next = next + 1
          self.assessment_id = -1;
          self.name = "New Assessment %1".arg(next);
          self.description = "Description of Assessment %1".arg(next)
        }
        self.uuid = "";
        root.backend.update_assessment(self);
        root.model.insert(root.model.count, {
          "id": self.assessment_id,
          "name": "%1".arg(self.name),
          "description": "%1".arg(self.description),
          "type": self.type,
          "available_points": self.available_points,
          "criteria": self.criteria
        });
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        self.assessment_id = -1;
         self.name = root.model.get(root.index).name;
          root.backend.remove_assessment(self);
          root.model.remove(root.index);
          listArea.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : listArea
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
            listArea.currentIndex = index

          }
        }

        Text {
          id : assessment_title_text
          anchors.left : assessment.left
          anchors.leftMargin : 5
          text : model.name
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
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
            if (!enabled) {
              return description.split("\n")[0].trim()
            } else {
              var details = description.split("\n")
              details.splice(0, 1)
              var details_str = details.join('\n').trim()
              return(details_str === "") ? description.trim() : details_str

            }
          }
          enabled : false
          color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
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
          if (listArea.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}
    }
  }
    function rebuildAssessments() {
    assessmentList.assessments = []
    let assessments = root.backend.assessments;
    for (var ii = 0; ii < assessments.length; ++ ii) {
      assessmentList.assessments.push(assessment.make());
      assessmentList.assessments[assessmentList.assessments.length - 1].assign(assessments[ii]);
    }
    assessmentList.model = assessmentList.assessments;
  }

  Component.onCompleted : {
    rebuildAssessments()
  }

  onBackendChanged : {
    if (backend) {
      backend.assessmentsChanged.connect(rebuildAssessments)
    }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
