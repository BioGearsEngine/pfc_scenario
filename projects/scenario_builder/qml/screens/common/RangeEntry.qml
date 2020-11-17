import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

RowLayout {
  id: root
  property int rangeMin : 0
  property int rangeMax : 100000

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
	
	property int decimals: 2
    property real realValue: value / 100

    validator: DoubleValidator {
        bottom: Math.min(minEntry.from, minEntry.to)
        top:  Math.max(minEntry.from, minEntry.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', minEntry.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }

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
	
	property int decimals: 2
    property real realValue: value / 100

    validator: DoubleValidator {
        bottom: Math.min(maxEntry.from, maxEntry.to)
        top:  Math.max(maxEntry.from, maxEntry.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', maxEntry.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }

    onValueModified : {
        if ( maxEntry.value > value) {
          maxEntry.value = value
        }
        root.rangeModified()
    }
  }
}