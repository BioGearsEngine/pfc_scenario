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
  readonly property alias model : listArea.model
  readonly property alias index : listArea.currentIndex
  property int topIndex
  property int syllabusIndex

  Injury {
    id : currentInjury
  }
  function refresh_traumas() {
    var r_count = backend.injury_count();
    root.backend.injuries();
    listArea.model.clear();
    while (root.backend.next_injury(currentInjury)) {
      listArea.model.insert(listArea.model.count, {
        id: currentInjury.injury_id,
        medical_name: "%1".arg(currentInjury.medical_name),
        common_name: "%1".arg(currentInjury.common_name),
        description: "%1".arg(currentInjury.description),
        min: currentInjury.min,
        max: currentInjury.max,
        citations: currentInjury.citations
      });
    }
  }
  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.margins : 5

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
        currentInjury.injury_id = -1;
        currentInjury.medical_name = "New Trauma %1".arg(next);
        currentInjury.common_name = "New Trauma %1".arg(next);
        currentInjury.description = "Description of Trauma %1".arg(next);
        currentInjury.citations = "";
        currentInjury.min = 0.0;
        currentInjury.max = 1.0;
        while (root.backend.select_injury(currentInjury)) {
          next = next + 1
          currentInjury.injury_id = -1;
          currentInjury.medical_name = "New Trauma %1".arg(next);
          currentInjury.common_name = "New Trauma %1".arg(next);
          currentInjury.description = "Description of Trauma %1".arg(next)
        }
        currentInjury.uuid = "";
        root.backend.update_injury(currentInjury);
        root.model.insert(root.model.count, {
          "id": currentInjury.injury_id,
          "medical_name": "%1".arg(currentInjury.medical_name),
          "common_name": "%1".arg(currentInjury.common_name),
          "description": "%1".arg(currentInjury.description),
          "citations": currentInjury.citations,
          "min": currentInjury.min,
          "max": currentInjury.max

        });
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        currentInjury.injury_id = -1;
        currentInjury.medical_name = root.model.get(root.index).medical_name;
        root.backend.remove_injury(currentInjury);
        root.model.remove(root.index);
		if (listArea.currentIndex == 0) { // If the index was 0 this wasn't registering as an index change and the right pane wasn't reloading
          listArea.currentIndex = 1
        }
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
            listArea.currentIndex = index

          }
        }

        Text {
          id : trauma_title_text
          anchors.left : trauma.left
          anchors.leftMargin : 5
          text : model.medical_name
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : trauma_value_text
          anchors.top :trauma_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
            if (!enabled) {
              return model.common_name
            } else {
              if (description === "") {
                return model.common_name
              } else {
                return description
              }
            }
          }
          enabled : false
          color : enabled ?   Material.primaryTextColor : Material.secondaryTextColor
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
          if (listArea.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}

      Component.onCompleted : {
        refresh_traumas()
      }
    }
  }
  onSyllabusIndexChanged : {
    if (syllabusIndex == 1) {
      refresh_traumas()
    }
  }
  onTopIndexChanged : {
    if (topIndex == 1) {
      refresh_traumas()
    }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
