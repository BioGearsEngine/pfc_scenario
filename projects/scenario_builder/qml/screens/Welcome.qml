import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Page {
    id: root
    signal loadClicked()
    signal createClicked()
    signal recentClicked(string path)

    Rectangle {
        id: mainArea
        color : "transparent"
        border.color : "steelblue"
        anchors.centerIn : parent
        anchors.top : parent.top
        anchors.bottom : parent.bottom
        width : childrenRect.width + 50
        height: childrenRect.height +75

        ColumnLayout {
            Layout.alignment : Qt.AlignCenter
            anchors.centerIn : parent
            spacing : 15
            RowLayout {
              Layout.alignment: Qt.AlignTop
              Layout.fillWidth: true
              spacing : 10
              Text {
                  color: "#df4949"
                 text : "SUSTAIN Scenario Builder"
                 font.italic: true
                 font.bold: true
                 font.underline: true
                 font.pointSize: 24
              }
            }
            Rectangle {
              Layout.fillWidth: true
              Layout.alignment: Qt.AlignTop
              Layout.preferredHeight: childrenRect.height
              Layout.preferredWidth: childrenRect.width

              color : "transparent"
              Label {
                    text : "Getting Started:"
                    font.pointSize: 12
                    font.bold: true
                    color : 'steelblue'
                    anchors.left : parent.left
              }

              Button {
                  id: openButton
                  text : "Open"
                  anchors.right : newButton.left
                  anchors.rightMargin : 6

                  onClicked : { root.loadClicked() }
              }
              Button {
                id: newButton
                text : "New Scenario"
                anchors.right : parent.right

                onClicked : { root.createClicked() }
              }
            }
            Label {
                text : "Recent:"
                font.pointSize: 12
                font.bold: true
                color : 'steelblue'
            }
            ListView {
                id: listArea
                 Layout.alignment: Qt.AlignTop | Qt.AllignCenter
                 Layout.fillWidth: true
                 Layout.preferredHeight: 300
                 Layout.preferredWidth: 400
                 Layout.leftMargin : 20
                 spacing : 5
                 clip: true
                 highlightFollowsCurrentItem : true

                 model : ListModel {
                   ListElement {
                     path :'C:/SomePath/Scenario.pfc'; name : 'Scenario'; last : '2019-07-04'
                   }
                   ListElement {
                     path :'C:/SomePath/Scenario2.pfc';  name : 'Scenario2'; last : '2019-01-31'
                   }
                 }

                 delegate : MouseArea {
                  anchors.left : parent.left
                  anchors.right : parent.right
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
                     Text {
                      Layout.fillWidth : true
                       text : "FillSPACE"
                     }
                   }
                   width : childrenRect.width
                   height: childrenRect.height
                   onClicked: {
                     listArea.currentIndex = index
                     }
                 }

                 highlight: Rectangle {
                   color: 'red'
                   Layout.fillWidth : true
                   anchors.margins: 5
                 }
                 ScrollBar.vertical: ScrollBar { }
            }
            Item {
              Layout.fillWidth  : true
              Button {
                  anchors.right : parent.right
                  text : "Load"

                  onClicked : { root.loadClicked() }

              }
            }
        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:600;width:800}
}
 ##^##*/
