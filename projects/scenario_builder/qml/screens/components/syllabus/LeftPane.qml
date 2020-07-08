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
  property alias assessmentModel : assessments.model
  property alias injuryModel     : injuries.model
  property alias injurySetModel  : injurySets.model
  property alias treatmentModel  : treatments.model
  property alias equipmentModel  : equipment.model

  property alias objectiveIndex : objectives.index
  property alias assessmentIndex: assessments.index
  property alias injuryIndex    : injuries.index
  property alias injurySetIndex : injurySets.index
  property alias treatmentIndex : treatments.index
  property alias equipmentIndex : equipment.index

  property SQLBackend backend
  property ListElement modelData

  ColumnLayout {
    anchors.fill : root

    Shortcut {
      sequence: "F1"
      onActivated: {
        syllabus_tabs.currentIndex = 0
      }
    }
    Shortcut {
      sequence: "F2"
      onActivated: {
        syllabus_tabs.currentIndex = 1
      }
    }
    Shortcut {
      sequence: "F3"
      onActivated: {
        syllabus_tabs.currentIndex = 2
      }
    }
    Shortcut {
      sequence: "F4"
      onActivated: {
        syllabus_tabs.currentIndex = 3
      }
    }
    Shortcut {
      sequence: "F5"
      onActivated: {
        syllabus_tabs.currentIndex = 4
      }
    }
    Shortcut {
      sequence: "F6"
      onActivated: {
        syllabus_tabs.currentIndex = 5
      }
    }
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
        id: injury_tab
        text: "Traumas"
        width: implicitWidth
      }
      TabButton {
        id: injury_set_tab
        text: "Trauma Sets"
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
      TabButton {
        id: assessment_tab
        text: "Assessments"
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
      LP_Injuries {
        id : injuries
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      LP_InjurySets {
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
      LP_Assessments{
        id : assessments
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      onCurrentIndexChanged : {
        if (currentIndex == 4) {
          equipment.reloadEquipmentList()
        }
      }
    }
  }
}
