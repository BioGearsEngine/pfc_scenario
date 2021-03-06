import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root

  property alias nameWidth : label.width
  property alias label : label.text
  property alias placeholderText : input_area.placeholderText
  property alias text : input_area.text
  property bool required : false
  property int maximumRows : 12
  property int minimumRows : 7

  height : childrenRect.height
  signal editingFinished()
  //signal labelWidthChanged() This isn't currently necessary, but can be useful for standardizing label width across a pane
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
    width: 100
    Layout.alignment: Qt.AlignTop
  }

  Flickable {
    id: flickable
    height : Math.min( Math.max(contentHeight,font_metrics.height * minimumRows) ,font_metrics.height * root.maximumRows)
    rightMargin : 20
    contentWidth: width
    clip: true

    anchors { left : label.right; right: parent.right}
    TextArea.flickable: TextArea {
      id : input_area
      leftPadding: 5
      font.pointSize : 10
      placeholderText: qsTr("Text Area (5-15 Lines)")
      background : Rectangle { color: "transparent"}
      selectByMouse:true
      wrapMode: TextArea.Wrap

      MouseArea {
        anchors.fill: parent
        cursorShape: Qt.IBeamCursor
        acceptedButtons: Qt.NoButton
      }

      onEditingFinished :{
        root.editingFinished();
      }
    }
    ScrollBar.vertical: ScrollBar { }
  }
  Component.onCompleted : {
    root.completed();
    if (label.contentWidth > label.width) {
      label.width = label.contentWidth + 10
    }
  }
}