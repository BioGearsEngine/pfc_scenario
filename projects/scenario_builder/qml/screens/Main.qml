import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "components"

import com.ara.pfc.ScenarioModel.SQL 1.0
//Still to do 
/*
    Unify Reference Button s
    Give equipment the behavior of roles/events
    Roles/Events needs edit pane
    Left scene needs to not be editable
    Selection of right pane does not submit left pane
*/
Page {
  id: root
  signal closed
  property var stack
  property SQLBackend backend

  ColumnLayout {
    id: columns
    spacing : 5
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.fill : parent
    Rectangle {
      id: titleCard
      radius: 10
      Layout.alignment: Qt.AlignHCenter
      Layout.fillWidth: true
      Layout.minimumHeight: 50
      Text {
        id: titleCardText
        anchors.centerIn: parent
        text: 'SCENARIO TITLE'
        font.pointSize: 18
      }
    }
    TabBar {
      id: tabs
      font.pointSize: 8
      height : 12

      TabButton {
        text: "Summary"
        width: implicitWidth
      }
      TabButton {
        text: "Syllabus"
        width: implicitWidth
      }
      TabButton {
        text: "Narrative"
        width: implicitWidth
      }
      currentIndex : 0

    }

    StackLayout {
      Layout.alignment: Qt.AlignHCenter
      Layout.fillHeight: true
      Layout.fillWidth: true

      currentIndex: tabs.currentIndex
      width: parent.width
      height: parent.height - 10
      //TAB:SUMMARY

      ScenarioSummary{
        id: summary
        index : tabs.currentIndex
        backend : root.backend
        onScenarioTitleChanged : {
          titleCardText.text = summary.scenarioTitle
        }
      }

      //TAB:SYLLABUS
      ScenarioSyllabus {
         id: syllabus
         backend : root.backend
      }
      //TAB:NARRATIVE
      ScenarioNarrative{
        id: narrative
        backend : root.backend
      }
    }
  }
}
