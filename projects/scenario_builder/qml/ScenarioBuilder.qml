import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.3

import "screens"

import com.ara.pfc.ScenarioModel.SQL 1.0 
import com.ara.pfc.ScenarioModel.XML 1.0 
//This is importing C++ code


ApplicationWindow {
  title: qsTr("Test")
  width: 1280; height: 768
  visible: true
  menuBar: MenuBar {
    visible: true
    font.pointSize : 8
    Menu {
      title: "File"
      font.pointSize : 8
        MenuItem { text: "New Scenario";
                   onTriggered : {
                     console.log (text)
                     scenario_model.open();
                     scenario_model.initialize_db();
                     scenario_model.populate_db();
                     mainView.push( scenarioScreen, { backend : scenario_model} )
                 } }
        MenuItem { text: "Save Scenario";
                   onTriggered : {
                    console.log (text)
                    scenario_serializer.save(scenario_model)
                 }  }
        MenuItem { text: "Save Scenario As";
                  onTriggered : {
                    console.log (text)
                    scenario_serializer.save()
                  }  }
        MenuItem { text: "Load Scenario";
                   onTriggered : {
                   console.log (text)
                   scenario_serializer.load("PFC_Scenario.zip")
                   //loadDialog.open()
                 }  }
    }
    Menu {
      title: "Edit"
      font.pointSize : 8
        MenuItem { text: "Authorship" }
    }
    Menu {
      title: "View"
      font.pointSize : 8
        MenuItem { text: "Narrative"}
        MenuItem { text: "Syllabus"}
    }
    Menu {
      title: "About"
      font.pointSize : 8
        MenuItem { text: "Help"}
        MenuItem { text: "About"}
    }
  }
  
  SQLBackend {
    id : scenario_model
    name : "pfc_sb_working.sqlite"
  }
  XMLSeralizer {
    id : scenario_serializer
    db : scenario_model
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
        scenario_model.open();
        scenario_model.initialize_db();
        scenario_model.populate_db();
         mainView.push( scenarioScreen, { backend : scenario_model} )
       }
    }

    FileDialog {
      id: loadDialog
      title: "Please Choose a File:"
      folder:shortcuts.home
      visible: false
      selectMultiple : false
       nameFilters: [ "Scenarios (*.pfc)", "All files (*)" ]
      //folder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
      onAccepted: {
        console.log("You chose: " + loadDialog.fileUrls)
        var archive = loadDialog.fileUrls.toString();
        archive = archive.replace(/file:\/{3}/,"")
        scenario_serializer.load(archive)
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