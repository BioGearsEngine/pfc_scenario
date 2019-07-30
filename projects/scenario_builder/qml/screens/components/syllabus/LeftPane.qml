import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property alias currentIndex : syllabus_tabs.currentIndex
  property SQLBackend backend

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
      focus: true
      currentIndex: syllabus_tabs.currentIndex

      LP_Objectives{
        Layout.fillHeight : true
        Layout.fillWidth : true
        focus : true
      }
      LP_Assessments{
          Layout.fillHeight : true
          Layout.fillWidth : true
          focus : true
      }
      LP_Injuries {
          Layout.fillHeight : true
          Layout.fillWidth : true
          focus : true
      }
      LP_Injuries {
          Layout.fillHeight : true
          Layout.fillWidth : true
          focus : true
      }
      LP_Treatments {
          Layout.fillHeight : true
          Layout.fillWidth : true
          focus : true
      }
      LP_Equipment {
          Layout.fillHeight : true
          Layout.fillWidth : true
          focus : true
      }
    }
  }
}
