import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Label {
  id: root

  property int pointSize : 10

   text: 'Title:'
   font.pointSize : root.pointSize
   color: "steelblue"
   width: (text.width > 90 ) ? text.width + 10 : 100
  }