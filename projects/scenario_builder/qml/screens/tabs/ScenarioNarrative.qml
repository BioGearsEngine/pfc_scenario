import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "narrative"
import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

RowLayout {
  id : root
  property SQLBackend backend
  property int topIndex

  Layout.fillWidth : true
  Layout.fillHeight : true
  spacing : 5

  // TAB:SUMMARY_LEFTWINDOW
  LeftPane {
    id : narrative_left
    backend : root.backend
  }
  // TAB:SUMMARY_RIGHTWINDOW
  RightPane {
    id : narrative_right
    
    Layout.bottomMargin : 5 
    Layout.rightMargin : 5 

    backend : root.backend
    model : narrative_left.model
    index : narrative_left.index
    count : narrative_left.count
    topIndex : root.topIndex
    onIndexChanged : {}
  }
}
