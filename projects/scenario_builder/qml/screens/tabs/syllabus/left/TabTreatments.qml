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
  property Treatment currentTreatment : (treatmentList.treatmentDefinitions[treatmentList.currentIndex]) ? 
                                         treatmentList.treatmentDefinitions[treatmentList.currentIndex] : currentTreatment

  Treatment {
    id : currentTreatment
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
        currentTreatment.treatment_id = -1;
        currentTreatment.medicalName = "New Treatment %1".arg(next);
        currentTreatment.commonName = "New Treatment %1".arg(next);
        currentTreatment.description = "Description of Treatment %1".arg(next);
        currentTreatment.equipment = "";
        currentTreatment.citations = "";
        while (root.backend.select_treatment(currentTreatment)) {
          next = next + 1
          currentTreatment.treatment_id = -1;
          currentTreatment.medicalName = "New Treatment %1".arg(next);
          currentTreatment.commonName = "New Treatment %1".arg(next);
          currentTreatment.description = "Description of Treatment %1".arg(next)
        }
        currentTreatment.uuid = "";
        root.backend.update_treatment(currentTreatment);
        root.model.insert(root.model.count, {
          "id": currentTreatment.treatment_id,
          "medicalName": "%1".arg(currentTreatment.medicalName),
          "commonName": "%1".arg(currentTreatment.commonName),
          "description": "%1".arg(currentTreatment.description),
          "citations": currentTreatment.citaitons,
          "equipment": currentTreatment.equipment

        });
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        currentTreatment.treatment_id = -1;
        currentTreatment.medicalName = root.model.get(root.index).medicalName;
        root.backend.remove_treatment(currentTreatment);
        root.model.remove(root.index);
        if (treatmentList.currentIndex == 0) { // If the index was 0 this wasn't registering as an index change and the right pane wasn't reloading
          treatmentList.currentIndex = 1
        }
        treatmentList.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : treatmentList
      property var treatmentDefinitions 

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
        id : treatment
        property var currentDef : treatmentList.treatmentDefinitions[index]
        color : 'transparent'
        border.color : "steelblue"
        height : treatment_title_text.height + treatment_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            treatmentList.currentIndex = index
          }
        }

        Text {
          id : treatment_title_text
          anchors.left : treatment.left
          anchors.leftMargin : 5
          text : (currentDef) ? currentDef.medicalName : ""
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }

        Text {
          id : treatment_value_text
          anchors.top : treatment_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          wrapMode : Text.Wrap
          text : {
             let l_commonName  = (currentDef) ? currentDef.commonName : ""
             let l_description = (currentDef) ? currentDef.description : ""
            if (!enabled) {
              return l_commonName
            } else {
              if (l_description === "") {
                return l_commonName
              } else {
                return l_description
              }
            }
          }
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges {
            target : treatment_title_text;
            enabled : true
          }
          PropertyChanges {
            target : treatment_value_text;
            enabled : true
          }
        }

        onFocusChanged : {
          if (treatmentList.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }
      ScrollBar.vertical : ScrollBar {}
    }
  }
  function rebuildTreatments() {
    treatmentList.treatmentDefinitions = []
    let treatments = root.backend.treatments;
    for (var ii = 0; ii < treatments.length; ++ ii) {
      treatmentList.treatmentDefinitions.push(currentTreatment.make());
      treatmentList.treatmentDefinitions[treatmentList.treatmentDefinitions.length - 1].assign(treatments[ii]);
    }
    treatmentList.model = treatmentList.treatmentDefinitions;
  }

  Component.onCompleted : {
    rebuildTreatments()
  }

  onBackendChanged : {
    if (backend) {
      backend.treatmentsChanged.connect(rebuildTreatments)
    }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
