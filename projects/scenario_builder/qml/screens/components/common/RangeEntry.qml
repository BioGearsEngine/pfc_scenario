import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

RowLayout {
  id: root
  property int rangeMin : 0
  property int rangeMax : 100

  property alias label: name.text
  property alias min : minEntry.value
  property alias max : maxEntry.value

  signal rangeModified()

  Layout.preferredHeight : minEntry.up.implicitIndicatorHeight 
  Layout.rightMargin : 20

 PFCLabel {
  id: name
   text: 'Time:'
  }

  SpinBox {
    id: minEntry
    from: root.rangeMin
    to: root.rangeMax
    value: root.rangeMin
    editable: true
    width: parent.width / 4
    height: parent.height
    leftPadding : down.implicitIndicatorWidth
    rightPadding: up.implicitIndicatorWidth

    onValueModified : {
        if ( maxEntry.value < value) {
          maxEntry.value = value
        }
        root.rangeModified()
    }
  }

  SpinBox {
    id: maxEntry
    from: root.rangeMin
    to: root.rangeMax
    value: root.rangeMax
    editable: true
    width: parent.width / 4
    height: parent.height
    leftPadding : down.implicitIndicatorWidth
    rightPadding: up.implicitIndicatorWidth

    onValueModified : {
        if ( minEntry.value > value) {
          minEntry.value = value
        }
        root.rangeModified()
    }
  }
}