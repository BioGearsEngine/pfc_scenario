import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "narrative"

RowLayout {
  anchors.fill: parent
  anchors.margins: 5
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