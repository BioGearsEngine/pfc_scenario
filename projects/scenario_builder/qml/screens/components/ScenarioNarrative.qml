import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "narrative"
import "common"

RowLayout {
  id: root
  property var backend
  
  Layout.fillWidth : true
  Layout.fillHeight: true
  spacing: 5

  //TAB:SUMMARY_LEFTWINDOW 
  LeftPane{
    id : narrative_left
  }
  //TAB:SUMMARY_RIGHTWINDOW
  RightPane{
    id : narrative_right
  }
}
