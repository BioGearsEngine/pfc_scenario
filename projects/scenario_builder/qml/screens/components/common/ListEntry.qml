import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id: root
  property alias label: name.text

  height : content.height
  Layout.rightMargin : 20
  Layout.alignment : Qt.AlignTop

  Label {
  id: name
  Layout.alignment : Qt.AlignTop
   text: 'Retrictions'
   font.pointSize : 10
   color: "steelblue"
   width: 100
  }

  Rectangle {
    id : content
    color: "transparent"
    border.color: "grey"
    radius: 2
    height : 300
    anchors { left : name.right; right: parent.right ; }

    Button {
      id : addButton
      text : "Add " + name.text
      font.pointSize : 10
      anchors.left : content.left
      anchors.leftMargin : 5
      background : Rectangle {
        color : 'grey'
      }
    }

    Button {
      id: removeButton
      text : "Remove " + name.text
      font.pointSize : 10
      anchors.right : content.right
      anchors.rightMargin : 5
      background : Rectangle {
        color : 'grey'
      }
    }

    ListView {
      id : listArea
      anchors { left : parent.left; right: parent.right ; 
                top   : addButton.bottom ; topMargin : 5
                bottom : content.bottom
              }
      spacing : 5
      clip: true
      model : ListModel {
        id: dataModel
        ListElement {
            name: "John Smith"
            value: "1111-1111"
        }
        ListElement {
            name: "Peter Poter"
            value: "2222-2222"
        }
        ListElement {
            name: "Anna Lasalle"
            value: "3333-3333"
        }
                ListElement {
            name: "John Smith"
            value: "1111-1111"
        }
        ListElement {
            name: "Peter Poter"
            value: "2222-2222"
        }
        ListElement {
            name: "Anna Lasalle"
            value: "3333-3333"
        }
                ListElement {
            name: "John Smith"
            value: "1111-1111"
        }
        ListElement {
            name: "Peter Poter"
            value: "2222-2222"
        }
        ListElement {
            name: "Anna Lasalle"
            value: "3333-3333"
        }
                ListElement {
            name: "John Smith"
            value: "1111-1111"
        }
        ListElement {
            name: "Peter Poter"
            value: "2222-2222"
        }
        ListElement {
            name: "Anna Lasalle"
            value: "3333-3333"
        }
                ListElement {
            name: "John Smith"
            value: "1111-1111"
        }
        ListElement {
            name: "Peter Poter"
            value: "2222-2222"
        }
        ListElement {
            name: "Anna Lasalle"
            value: "3333-3333"
        }
      }

      delegate : Rectangle {
        id : delegateArea
        
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        color : 'transparent'
        border.color: "steelblue"
        height : 30

        Text {
          anchors.left : delegateArea.left
          anchors.leftMargin : 5
          text : name + " " + value
        }
      }
    }
  }
}