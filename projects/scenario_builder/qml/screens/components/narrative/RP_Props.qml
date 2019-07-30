import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
	id: root
    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
    Layout.margins: 5
    
	border.color: 'black'
    FourButtonRow {
      id: controls
      anchors.top : root.top
      anchors.left : root.left
      anchors.right : root.right
      anchors.topMargin : 2
      anchors.rightMargin  : 5
      anchors.leftMargin  : 5
      firstButtonText : "New"
      fourthButtonText : "Up"
      secondButtonText : "Down"
      thirdButtonText : "Delete"
    }
}