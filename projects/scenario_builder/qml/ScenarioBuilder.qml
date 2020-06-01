import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.0

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
      MenuItem { 
        text: "New Scenario";
          onTriggered : {
          create_scenario()
        } 
      }
      MenuItem { 
        id : saveOption
        text: "Save Scenario";
        enabled : false
        onTriggered : {
          save_scenario(scenario_serializer)
        }
      }
      MenuItem {
        id: saveAsOption
        text: "Save Scenario As";
        enabled : false
        onTriggered : {
          save_scenario_as(scenario_serializer)
        } 
      }
      MenuItem { 
        text: "Load Scenario";
        onTriggered : {
          load_scenario(scenario_serializer)
        }
       }
       MenuItem { 
         id : closeOption
         text: "Close Scenario";
         enabled : false
        onTriggered : {
          mainView.pop()
          saveOption.enabled = false
          saveAsOption.enabled = false
          closeOption.enabled = false
          mainView.push(welcomeScreen)
        }
      }
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
    file : "PFC_SCENARIO.pfc.zip"
  }
  StackView {
    id: mainView
    anchors.fill: parent

    initialItem : welcomeScreen
    }

    FileDialog {
      id: loadDialog
      title: "Please Choose a File:"
      visible: false
      selectMultiple : false
      selectExisting : true
      nameFilters: [ "Scenarios (*.pfc;*.zip)", "All files (*)" ]
      folder: "./"//StandardPaths.writableLocation(StandardPaths.DesktopLocation)
      onAccepted: {
        console.log("You choose: " + loadDialog.fileUrls)
        var archive = loadDialog.fileUrls.toString();
        archive = archive.replace(/file:\/{3}/,"")
        if(scenario_serializer.load(archive)) {
          if(mainView.depth > 1){
            mainView.pop()
          }
          mainView.push(scenarioScreen, { backend : scenario_model} )
        } else {
          console.log("Failed to Load File: %1".arg(archive))
        }
      }
      onRejected: {
        console.log("Canceled")
      }
    }

    FileDialog {
      id: saveDialog
      title: "Please Choose a File:"
      visible: false
      selectMultiple : false
      selectExisting : false
      nameFilters: [ "Scenarios (*.pfc;*.zip)", "All files (*)" ]
      folder: "./"// StandardPaths.writableLocation(StandardPaths.DesktopLocation)
      onAccepted: {
        console.log("You choose: " + saveDialog.fileUrls)
        var archive = saveDialog.fileUrls.toString();
        archive = archive.replace(/file:\/{3}/,"")
        scenario_serializer.save(archive, scenario_model)
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
    Component {
      id: welcomeScreen
      Welcome {
        onLoadClicked: {
         load_scenario(scenario_model)
        }
        onCreateClicked: {
         create_scenario(scenario_model)
        }
      }
    }
  Settings {
        id: settings
        property var previous_scenarios : []
  }
  function create_scenario( model ) {
        saveOption.enabled = true
        saveAsOption.enabled = true
        closeOption.enabled = true
        model.initialize_db();
        model.populate_db();
        mainView.push( scenarioScreen, { backend : model} )
  }

  function save_scenario() {
       scenario_serializer.save()
  }

  function save_scenario_as() {
       saveDialog.open()
       var archive = saveDialog.fileUrls.toString();
       settings.previous_scenarios.push({"file": archive })
  }

  function load_scenario( file, backend) {
    saveOption.enabled = true
    saveAsOption.enabled = true
    closeOption.enabled = true
    settings.previous_scenarios.push({"file": file })
    loadDialog.open()
  }
}