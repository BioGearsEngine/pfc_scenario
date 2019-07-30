import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "components"

import com.ara.pfc.ScenarioModel.SQL 1.0

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
      currentIndex : 2

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
        backend : root.backend
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
