import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "components/common"
// main window
Page {
  id: root
  signal loadClicked
  signal createClicked

  ColumnLayout {
    anchors.margins: 25
    anchors.fill: parent
    spacing: 1
    Layout.margins: 100

    Rectangle {
      id: titleArea
      color : "transparent"

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredHeight: 50
      Layout.preferredWidth: 400
      Text {
        anchors.centerIn: parent
        text: "SUSTAIN Scenario Builder"

        font.family: "Gotham"
        font.pixelSize: 25
        font.weight: Font.Bold
      }
    } 

    Rectangle {
      id: hotList

      Layout.alignment:  Qt.AlignTop
      Layout.preferredHeight: hotListLable.height
      Layout.preferredWidth: 400
      color : "transparent"
      Text {
        id: hotListLable
        anchors.left: parent.left
        text: "Recent Scenarios"
        horizontalAlignment : Text.AlignRight
        font.family: "Gotham"
        font.pixelSize: 13
        font.weight: Font.Bold
      }
      Button {
        text : "Load"
        anchors.left: hotList.right
        anchors.top : hotList.top
        onClicked : root.loadClicked()
      }
    } 

    ListView  {
      id : listArea

      Layout.alignment: Qt.AlignTop
      Layout.preferredHeight: 300
      Layout.preferredWidth: 400

      spacing : 5
      clip: true
      highlightFollowsCurrentItem : true

      model : ListModel {
        ListElement { 
          path :'C:/SomePath/Scenario.pfc'; 
          name : 'Scenario'
          last : '2019-07-04'
        }
        ListElement { 
          path :'C:/SomePath/Scenario2.pfc'; 
          name : 'Scenario2'
          last : '2019-01-31'
        }
      }

      delegate : MouseArea {
          Layout.fillWidth  : true
        RowLayout {
          spacing : 5
          Label {
            text : model.index + 1
          }
          Label {
            text : model.name + ":"
          }
          Text {
            text : model.path
          }
          Text {
            text : model.last
          }
        }
        width : childrenRect.width
        height: childrenRect.height
        onClicked: {
          listArea.currentIndex = index
          console.log ( "%1 , %2 ".arg(listArea.currentIndex).arg(index))
        }
      }

      highlight: Rectangle {
        color: 'red'
        Layout.fillWidth : true
        anchors.margins: 5
      }
      ScrollBar.vertical: ScrollBar { }
    }

    Button {
      text :"New"
      onClicked : root.createClicked()
    } 
  }
}

