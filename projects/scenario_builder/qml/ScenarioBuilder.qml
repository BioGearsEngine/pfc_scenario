import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.3

import "screens"

import com.ara.pfc.ScenarioModel 1.0 
//This is importing C++ code


ApplicationWindow {
  title: qsTr("Test")
  width: 1280; height: 768
  visible: true

  menuBar: MenuBar {
    visible: true
    Menu {
      title: "File"
        MenuItem { text: "New Scenario";     }
        MenuItem { text: "Save Scenario";    }
        MenuItem { text: "Save Scenario As"; }
        MenuItem { text: "Load Scenario";    }
    }
    Menu {
      title: "Edit"
        MenuItem { text: "Authorship" }
    }
    Menu {
      title: "View"
        MenuItem { text: "Narrative"}
        MenuItem { text: "Syllabus"}
    }
    Menu {
      title: "About"
        MenuItem { text: "Help"}
        MenuItem { text: "About"}
    }
  }
  
  ScenarioModel {
    id : scenario_model
  }
  StackView {
    id: mainView
    anchors.fill: parent

    initialItem : startScreen
    Welcome {
       id: startScreen
       onLoadClicked: {
         loadDialog.open()
       }
       onCreateClicked: {
         //mainView.push( scenarioScreen, { model : scenario_model} )
         mainView.push( scenarioScreen )
        }
    }
    FileDialog {
      id: loadDialog
      title: "Please Choose a File:"
      visible: false
      //folder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
      onAccepted: {
        console.log("You chose: " + fileDialog.fileUrls)
      }
      onRejected: {
        console.log("Canceled")
      }
    }
    Component {
      id: scenarioScreen
      Main {
        visible:false
        stack: mainView
        onClosed : mainView.pop()
      }
    }
  }
}