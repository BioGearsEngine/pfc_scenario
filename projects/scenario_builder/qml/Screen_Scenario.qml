import QtQuick 2.10
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0

import com.ara.pfc.ScenarioModel 1.0
import com.ara.pfc.ScenarioModel.Locations 1.0
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
          text: "PFC: Scenario Builder"
          elide: Label.ElideRight
          horizontalAlignment: Qt.AlignHCenter
          verticalAlignment: Qt.AlignVCenter
          Layout.fillWidth: true
      }
      ToolButton {
        text: qsTr("⋮")

        FileDialog {
          id: dialog
            title: "Please choose a file"
            folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
            property bool saveMode : false
            nameFilters: ["MSDL Files(*.xml)"]
            onAccepted: {
              console.log("You chose: " + file)
              //TODO: Handle Failure
              if(saveMode)
                root.model.SaveAs( file );
              else
                root.model.Load( file );
            }
            onRejected: {
              console.log("Canceled")
            }
        }
        Menu {
          id: contextMenu
          MenuItem { 
           text: "New"  ; 
           font.pointSize: 8; 
           onTriggered : { 
             console.log("New Clicked") ; 
             root.model.reset() 
           }
          }
          MenuItem { 
           text: "Load" ; 
           font.pointSize: 8; 
           onTriggered : { 
             console.log("Load Clicked");
             if(root.model.onDisk) {
               root.model.load()
             } else {
               dialog.saveMode = false; 
               dialog.folder = StandardPaths.writableLocation(StandardPaths.DesktopLocation)
               dialog.currentFile = "";
               dialog.fileMode = FileDialog.OpenFile;
               dialog.open();
             }
           }
          }
          MenuItem { 
           text: "Save" ; 
           font.pointSize: 8; 
           onTriggered : { 
             console.log("Save Clicked"); 
             if(root.model.onDisk) {
               root.model.save()
             } else {
               dialog.saveMode = true; 
               dialog.fileMode = FileDialog.SaveFile;
               dialog.folder = StandardPaths.writableLocation(StandardPaths.DesktopLocation)
               dialog.currentFile = "./Scenario.xml";
               dialog.open();
             }
           }
          }
        }
        onClicked: contextMenu.open()
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
        model : root.model.locations
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
        model : root.model.actors
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
        model : root.model.objects
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
