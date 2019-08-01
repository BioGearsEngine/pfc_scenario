import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property alias currentIndex : syllabus_tabs.currentIndex
  property alias objectiveModel  : objectives.model
  property alias currentAssessment : assessments.model
  property alias currentInjury     : injuries.model
  property alias currentInjurySet  : injurySets.model
  property alias currentTreatment  : treatments.model
  property alias currentEquipment  : equipment.model

  property alias objectiveIndex : objectives.index
  
  property SQLBackend backend
  property ListElement modelData

  ColumnLayout {
    anchors.fill : root


    TabBar {
      id: syllabus_tabs
      Layout.fillWidth: true
      font.pointSize: 8
      height : 12

      TabButton {
        id: objective_tab
        text: "Objectives"
        width: implicitWidth
      }
      TabButton {
        id: assessment_tab
        text: "Assessments"
        width: implicitWidth
      }
      TabButton {
        id: injury_tab
        text: "Injuries"
        width: implicitWidth
      }
      TabButton {
        id: injury_set_tab
        text: "Injury Sets"
        width: implicitWidth
      }
      TabButton {
          id: treatment_tab
        text: "Treatments"
        width: implicitWidth
      }
      TabButton {
          id: equipment_tab
        text: "Equipment"
        width: implicitWidth
      }
    }

    StackLayout {
      id: syllabus_stack_left
      Layout.fillHeight: true
      Layout.fillWidth: true
      currentIndex: syllabus_tabs.currentIndex

      LP_Objectives{
        id : objectives
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_Assessments{
        id : assessments
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_Injuries {
        id : injuries
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_Injuries {
        id : injurySets
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_Treatments {
        id : treatments
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_Equipment {
        id : equipment
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
    }
  }
}
