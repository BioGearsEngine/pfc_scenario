import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "syllabus"

RowLayout {
  anchors.fill: parent
  anchors.margins: 5
  spacing: 5

  width: parent.width
  height: parent.height
  LeftPane{
    id: syllabus_leftPane
  }
  RightPane{
    id: syllabus_rightPane
  }
}