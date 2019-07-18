import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "syllabus"

RowLayout {
  id: root
  property var backend

  width: parent.width
  height: parent.height
  spacing: 5
  
  LeftPane{
    id: syllabus_leftPane
  }
  RightPane{
    id: syllabus_rightPane
  }
}