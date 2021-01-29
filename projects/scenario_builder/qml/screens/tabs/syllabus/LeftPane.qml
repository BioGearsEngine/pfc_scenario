import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../../common"
import "left"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
 
  property alias currentIndex : syllabus_tabs.currentIndex
  property alias currentObjective : objectives.currentObjective
  property alias currentAssessment : assessments.currentAssessment
  property alias currentTrauma : traumas.currentTrauma
  property alias currentProfile : traumaProfiles.currentProfile
  property alias currentTreatment : treatments.currentTreatment
  property var currentEquipment : equipment.currentEquipment

  property SQLBackend backend
  property ListElement modelData
  property int topIndex

  ColumnLayout {
    anchors.fill : root

    Shortcut {
      id : f1
      sequence : "F1"
      onActivated : {
        syllabus_tabs.currentIndex = 0
      }
    }
    Shortcut {
      id : f2
      sequence : "F2"
      onActivated : {
        syllabus_tabs.currentIndex = 1
      }
    }
    Shortcut {
      id : f3
      sequence : "F3"
      onActivated : {
        syllabus_tabs.currentIndex = 2
      }
    }
    Shortcut {
      id : f4
      sequence : "F4"
      onActivated : {
        syllabus_tabs.currentIndex = 3
      }
    }
    Shortcut {
      id : f5
      sequence : "F5"
      onActivated : {
        syllabus_tabs.currentIndex = 4
      }
    }
    Shortcut {
      id : f6
      sequence : "F6"
      onActivated : {
        syllabus_tabs.currentIndex = 5
      }
    }

    TabBar {
      id : syllabus_tabs
      Layout.fillWidth : true
      font.pointSize : 8
      currentIndex : 0
      height : 12

      TabButton {
        id : objective_tab
        text : "Objectives"
        width : implicitWidth
      }
      TabButton {
        id : trauma_tab
        text : "Traumas"
        width : implicitWidth
      }
      TabButton {
        id : trauma_set_tab
        text : "Trauma Profiles"
        width : implicitWidth
      }
      TabButton {
        id : treatment_tab
        text : "Treatments"
        width : implicitWidth
      }
      TabButton {
        id : equipment_tab
        text : "Equipment"
        width : implicitWidth
      }
      TabButton {
        id : assessment_tab
        text : "Assessments"
        width : implicitWidth
      }
    }

    StackLayout {
      focus:true
      id : syllabus_stack_left
      Layout.fillHeight : true
      Layout.fillWidth : true
      currentIndex : syllabus_tabs.currentIndex

      TabObjectives {
        id : objectives
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      TabTraumas {
        id : traumas
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        topIndex : root.topIndex
      }
      TabTraumaProfiles {
        id : traumaProfiles
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      TabTreatments {
        id : treatments
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
      }
      TabEquipment {
        id : equipment
        Layout.fillHeight : true
        Layout.fillWidth : true
        backend : root.backend
        topIndex : root.topIndex

        onCurrentEquipmentChanged : {
          root.currentEquipment = equipment.currentEquipment
        }
      }
      TabAssessments {
        focus:true
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
  onTopIndexChanged : {
    if (topIndex == 1) {
      f1.enabled = true
      f2.enabled = true
      f3.enabled = true
      f4.enabled = true
      f5.enabled = true
      f6.enabled = true
    } else {
      f1.enabled = false
      f2.enabled = false
      f3.enabled = false
      f4.enabled = false
      f5.enabled = false
      f6.enabled = false
    }
  }
}
