import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

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
          id: sustainScenarioBuilder
          color: 'red'
          radius: 10
          Layout.fillWidth: true
          Layout.minimumHeight: 50
          Layout.minimumWidth: Font.contentWidth + 10
          Text {
              anchors.centerIn: parent
              text: "SUSTAIN Scenario Builder"
              color: 'white'
              font.family: "Gotham"
              font.pixelSize: 25
              font.weight: Font.Bold
          }
      } 
      Rectangle {
        id: recentScenarios
        color: 'plum'
        radius: 100
        anchors.top: sustainScenarioBuilder.bottom
        anchors.topMargin: 10
        Layout.fillWidth: true
        Layout.minimumHeight: 50
        Layout.minimumWidth: Font.contentWidth + 10
          Text {
              anchors.centerIn: parent
              text: "Recent Scenarios"
              color: 'black'
              font.family: "Gotham"
              font.pixelSize: 25
              font.weight: Font.Bold
          }
      }
      Row {
        anchors.top: recentScenarios.bottom
        anchors.horizontalCenter: recentScenarios.horizontalCenter
        anchors.topMargin: 10
        spacing: 20
        Rectangle {
          id : loadScenarioButton
          color: 'plum'
          radius: 5
        
          height: 25
          width: 100
          Text {
              anchors.centerIn: parent
              text: "Load Scenario"
          }
          MouseArea {
            anchors.fill: parent
            onClicked: {
              root.loadClicked()
            }
          }
        }

        Rectangle { 
          color: 'lightblue'
          radius: 5
          height: 25
          width: 100
          Text {
              anchors.centerIn: parent
              text: "Create Scenario"
          }
          MouseArea {
            id:mouseArea
            anchors.fill: parent
            onClicked: {
              root.createClicked()
            }
          } 
        }
     }
  }
}
