import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
  id: root
  property alias label: name.text

  Layout.preferredHeight : hourTime.up.implicitIndicatorHeight 
  Layout.rightMargin : 20
  
  Label {
  id: name
   text: 'Time:'
   font.pointSize : 10
   color: "steelblue"
   width: 100
  }
  SpinBox {
    id: hourTime
    value: 23
    from: 0
    to: 23
    editable: true
    width: parent.width / 4
    height: parent.height
    anchors {left : name.right }
    leftPadding : down.implicitIndicatorWidth
    rightPadding: up.implicitIndicatorWidth
  }
  SpinBox {
    id: minuteTime
    from: 0
    to: 59
    value: 59
    editable: true
    width: parent.width / 4
    height: parent.height
    anchors {left : hourTime.right }
    leftPadding : down.implicitIndicatorWidth
    rightPadding: up.implicitIndicatorWidth
  }
}