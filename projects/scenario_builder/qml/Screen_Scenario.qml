import QtQuick 2.10
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import com.ara.pfc.ScenarioModel 1.0
Page {
  id: root
  focus: true
  Keys.forwardTo: [scenarioFlickable]
  signal closed

  property var stack
  property ScenarioModel model

  header: ToolBar {
    RowLayout {
        anchors.fill: parent
        ToolButton {
            text: qsTr("‹")
            onClicked: root.closed();
        }
        Label {
            text: "Title"
            elide: Label.ElideRight
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            Layout.fillWidth: true
        }
        ToolButton {
            text: qsTr("⋮")
            onClicked: menu.open()
        }
    }
  }
  Flickable{
      id : scenarioFlickable
      anchors { fill : parent }
      flickableDirection: Flickable.VerticalFlick
      boundsBehavior: Flickable.StopAtBounds
  
      function calculateNeededHeight() {
          scenarioFlickable.contentHeight = summary.height + locations.height + actors.height + objects.height
      }

      Scenario_Summary{ 
        id: summary
        stack : root.stack
        model : root.model
        anchors {
          top: parent.top
          left : parent.left
          right: parent.right
        }
        onHeightChanged: {
         scenarioFlickable.calculateNeededHeight()
        }
      }
      Scenario_Locations{ 
        id:locations
        stack : root.stack
        model : root.model
        anchors {
          top : summary.bottom
          left : parent.left
          right: parent.right
        }
        onHeightChanged: {
         scenarioFlickable.calculateNeededHeight()
        }

       }

       Scenario_Actors{ 
        id:actors
        stack : root.stack
        model : root.model
        anchors {
          top : locations.bottom
          left : parent.left
          right: parent.right
        }
        onHeightChanged: {
         scenarioFlickable.calculateNeededHeight()
        }
       }

      Scenario_Objects{ 
        id:objects
        stack : root.stack
        model : root.model
        anchors {
          top : actors.bottom
          left : parent.left
          right: parent.right
        }
        onHeightChanged: {
         scenarioFlickable.calculateNeededHeight()
        }
       }
  }
  Keys.onBackPressed: {
      console.log("onBackPressed");
      root.closed();
  }
  Keys.onEscapePressed: {
      console.log("onEscapePressed");
      root.closed();
  }
  Keys.onPressed: {
      console.log("onPressed");
      if (event.key == Qt.Key_Backspace) {
          console.log("backspace");
          event.accepted = true;
          root.closed();
      }
  }
}
