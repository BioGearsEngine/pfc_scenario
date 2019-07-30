import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "syllabus"
import "common"

import com.ara.pfc.ScenarioModel.SQL 1.0

GridLayout {
  id: root
  property SQLBackend backend

  width: parent.width
  height: parent.height
  columns: 2
  
  LeftPane{
    id: syllabus_leftPane
    backend : root.backend
    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
  }

  RightPane{
    id: syllabus_rightPane
    backend : root.backend

    currentIndex : syllabus_leftPane.currentIndex

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.topMargin : 58

    Component.onCompleted: {
      console.log("%1,%2".arg(width).arg(height))
    }
  }
}
