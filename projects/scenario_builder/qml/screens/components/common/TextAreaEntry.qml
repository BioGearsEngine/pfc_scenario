import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root

  property alias label : label.text
  property alias placeholderText : input_area.placeholderText
  property alias text : input_area.text
  property bool required : false

  signal editingFinished()
  signal completed()

  Layout.preferredHeight : flickable.height
  Layout.fillWidth : true
  Layout.rightMargin : 20

  FontMetrics {
    id: font_metrics
    font: input_area.font
  }
  
  Rectangle {
        color: "transparent"
        border.color: "grey"
        radius: 2
        anchors.fill : flickable
  }

  Label {
    id: label
    text: 'Purpose'
    font.pointSize : 10
    color: "steelblue"
    width: (text.width > 90 ) ? text.width + 10 : 100
    Layout.alignment: Qt.AlignTop
  }

  Flickable {
    id: flickable
    height : Math.min( Math.max(contentHeight,font_metrics.height * 7) ,font_metrics.height * 12)
    rightMargin : 20
    contentWidth: width
    clip: true

    anchors { left : label.right; right: parent.right}
    TextArea.flickable: TextArea {
      id : input_area
      leftPadding: 5
      font.pointSize : 10
      placeholderText: qsTr("Purpose Description")
      background : Rectangle { color: "transparent"}
      selectByMouse:true
      wrapMode: TextArea.Wrap
      onEditingFinished :{
        root.editingFinished();
      }
    }
    ScrollBar.vertical: ScrollBar { }
  Component.onCompleted : {
    root.completed();
  }
  }
}