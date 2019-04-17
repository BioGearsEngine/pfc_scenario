import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import com.ara.pfc.ScenarioModel 1.0
// main window
Page {
  id: root
  signal loadClicked
  signal createClicked

  ScenarioModel {
    id : scenario_model
  }
  ColumnLayout {
      anchors.margins: 25
      anchors.fill: parent
      spacing: 1
      Layout.margins: 100

      Rectangle {
          color: 'red'
          radius: 10
          Layout.fillWidth: true
          Layout.minimumHeight: 50
          Layout.minimumWidth: Font.contentWidth + 10
          Text {
              anchors.centerIn: parent
              text: "SUSTAIN Scenario Builder" + scenario_model.name
              color: 'white'
              font.family: "Gotham"
              font.pixelSize: 25
              font.weight: Font.Bold
          }
      } 
  
      Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
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
