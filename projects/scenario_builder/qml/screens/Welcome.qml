import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import com.ara.pfc.ScenarioModel.SQL 1.0
import com.ara.pfc.ScenarioModel.XML 1.0

Page {
    id: root
    property SQLBackend backend    
    signal loadClicked()
    signal createClicked()
    signal listUpdated()
    signal recentClicked(string path)
   onListUpdated : {
      recentFileList.clear();
      var recent_files = backend.get_recent_scenario_files();
      var i = 0;
      for (i = 0;i < 10;i++) {
        if (recent_files[i] != "") {
          recentFileList.append({'path':recent_files[i],'name':'NAME','last':'LAST'})
        }
      }
    }
    Rectangle {
        id  : mainArea
        color : "transparent"
        border.color : "steelblue"
        anchors.centerIn : parent
        anchors.top : parent.top
        anchors.bottom : parent.bottom
        width : mainColumnLayout.implicitWidth + 20
        height: mainColumnLayout.implicitHeight+ 10

        ColumnLayout {
            id: mainColumnLayout
            Layout.alignment : Qt.AlignCenter
            Layout.preferredWidth : 400
            Layout.preferredHeight :300
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
                  id : recentFileList
                  Component.onCompleted : {
                    root.listUpdated()
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
//                     Label {
//                       Layout.preferredWidth : 150
//                       text : model.name + ":"
//                     }
                     Text {
                       Layout.preferredWidth : 150
                       text : model.path
                     }
//                     Text {
//                       Layout.preferredWidth : 50
//                       text : model.last
//                     }
                   }
                   width : childrenRect.width
                   height: childrenRect.height
                   onClicked: {
                     listArea.currentIndex = index
                   }
                   onDoubleClicked: {
                     listArea.currentIndex = index
                     recentClicked(recentFileList.get(listArea.currentIndex).path)
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
                Layout.alignment : Qt.AlignRight
                text : "Load"

                onClicked : { root.loadClicked() }

            }
            
        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:600;width:800}
}
 ##^##*/
