import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "tabs"

import com.ara.pfc.ScenarioModel.SQL 1.0

Page {
  id : root
  signal closed
  property var stack
  property SQLBackend backend
  property int currentTab : 0
  ColumnLayout {
    id : columns
    spacing : 5
    anchors.horizontalCenter : parent.horizontalCenter
    anchors.fill : parent
    Rectangle {
      id : titleCard
      radius : 10
      Layout.alignment : Qt.AlignHCenter
      Layout.fillWidth : true
      Layout.minimumHeight : 50
      Text {
        id : titleCardText
        anchors.centerIn : parent
        text : 'SCENARIO TITLE'
        font.pointSize : 18
      }
    }
    TabBar {
      id : tabs
      font.pointSize : 8
      height : 12
      TabButton {
        text : "General"
        width : implicitWidth
      }
      TabButton {
        text : "Syllabus"
        width : implicitWidth
      }
      TabButton {
        text : "Narrative"
        width : implicitWidth
      }
      TabButton {
        text : "Summary"
        width : implicitWidth
      }
      currentIndex : root.currentTab
    }
    Shortcut {
      id : alt1
      sequence : "ALT+1"
      onActivated : {
        tabs.currentIndex = 0
      }
    }
    Shortcut {
      id : alt2
      sequence : "ALT+2"
      onActivated : {
        tabs.currentIndex = 1
      }
    }
    Shortcut {
      id : alt3
      sequence : "ALT+3"
      onActivated : {
        tabs.currentIndex = 2
      }
    }
    Shortcut {
      id : alt4
      sequence : "ALT+4"
      onActivated : {
        tabs.currentIndex = 3
      }
    }
    StackLayout {
      Layout.alignment : Qt.AlignHCenter
      Layout.fillHeight : true
      Layout.fillWidth : true

      currentIndex : tabs.currentIndex
      width : parent.width
      height : parent.height - 10
      // TAB:SUMMARY

      ScenarioGeneral {
        id : general
        index : tabs.currentIndex
        backend : root.backend
        onScenarioTitleChanged : {
          titleCardText.text = general.scenarioTitle
        }
      }
      // TAB:SYLLABUS
      ScenarioSyllabus {
        id : syllabus
        backend : root.backend
        topIndex : tabs.currentIndex
      }
      // TAB:NARRATIVE
      ScenarioNarrative {
        id : narrative
        backend : root.backend
        topIndex : tabs.currentIndex
      }
      ScenarioSummary {
        id : summary
        backend : root.backend
        index : tabs.currentIndex
      }
    }
  }
}