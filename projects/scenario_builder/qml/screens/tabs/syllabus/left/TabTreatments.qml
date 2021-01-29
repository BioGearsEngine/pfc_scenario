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
                                         treatmentList.treatmentDefinitions[treatmentList.currentIndex] : treatment_g

  Treatment {
    id : treatment_g
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
        var likely_id = root.backend.nextID(SQLBackend.TREATMENTS);
        treatment_g.clear(likely_id);
        root.backend.update_treatment(treatment_g);
        set_current_index(treatment_g.uuid)
      }
      onSecondButtonClicked : {
        if (!treatmentList.treatmentDefinitions || treatmentList.treatmentDefinitions.length < 2) {
          return
        }
        treatment_g.clear();
        treatment_g.assign(treatmentList.treatmentDefinitions[treatmentList.currentIndex]);
        root.backend.remove_treatment(treatment_g);
        update_treatments();
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
        property var currentDef: treatmentList.treatmentDefinitions[index]
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
            let l_commonName = (currentDef) ? currentDef.commonName : "";
            let l_description = (currentDef) ? currentDef.description : "";
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

  Component.onCompleted : {
    update_treatments()
  }

  Connections {
    target : backend
    onTreatmentsChanged : {
      update_treatments()
    }
    onEquipmentChanged : {
      update_treatments()
    }
    onCitationsChanged : {
      update_treatments()
    }
    
  }

  function set_current_index( uuid ) {
    for (var ii = 0; ii < treatmentList.treatmentDefinitions.length; ++ ii) {
      if ( treatmentList.treatmentDefinitions[ii].uuid == uuid ){
        treatmentList.currentIndex = ii
      }
    }
  }
  function update_treatments() {
      var index = ( treatmentList.currentIndex >= 0) ? treatmentList.currentIndex : 0
      treatmentList.treatmentDefinitions = []
      let treatments = root.backend.treatments;
      for (var ii = 0; ii < treatments.length; ++ ii) {
        treatmentList.treatmentDefinitions.push(currentTreatment.make());
        treatmentList.treatmentDefinitions[treatmentList.treatmentDefinitions.length - 1].assign(treatments[ii]);
      }
      treatmentList.model = treatmentList.treatmentDefinitions;
      treatmentList.currentIndex = index;
  }
}
