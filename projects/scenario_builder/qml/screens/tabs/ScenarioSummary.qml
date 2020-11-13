import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "summary"
import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

GridLayout {
  id : root
  property SQLBackend backend
  property int index
  width : parent.width
  height : parent.height
  columns : 2

  // TAB:SUMMARY_LEFTWINDOW
  Rectangle {
    border.color : 'black'
    border.width : 1
    Layout.preferredWidth : parent.width / 2
    Layout.fillHeight : true
    Layout.leftMargin : 5
    // Layout.rightMargin: 5
    LeftPane {
      id : summary_left
      backend : root.backend
      anchors.fill : parent
      index : root.index
    }
  }

  // TAB:SUMMARY_RIGHTWINDOW
  Rectangle {
    border.color : 'black'
    border.width : 1
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.rightMargin : 5
    RightPane {
      id : summary_right
      backend : root.backend
      anchors.fill : parent
      index : root.index
    }
  }
}
