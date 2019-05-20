import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import Qt.labs.platform 1.0

import com.ara.pfc.ScenarioModel 1.0

ApplicationWindow {
  id:object1
  width: 600; height: 800
  minimumWidth:  600
  minimumHeight: 800 
  visible: true
  
  ScenarioModel {
    id : scenario_model
  }

  StackView {
    id: mainView
    anchors.fill: parent

    initialItem : startScreen
    Screen_Start {
       id: startScreen
       onLoadClicked: {
         loadDialog.open()
        }
       onCreateClicked: {
         mainView.push( scenarioScreen, { model : scenario_model} )
        }
    }

    Component {
      id: scenarioScreen
      Screen_Scenario {
        visible:false
        stack: mainView
        onClosed : mainView.pop()
      }
    }

        FileDialog {
          id: loadDialog
            title: "Please choose a file"
            folder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
            fileMode: FileDialog.OpenFile;
            nameFilters: ["MSDL Files(*.xml)"]
            onAccepted: {
              console.log("You chose: " + file)
              scenario_model.Load( file );
              mainView.push( scenarioScreen, { model : scenario_model} )
            }
            onRejected: {
              console.log("Canceled")
            }
        }

    onCurrentItemChanged: {
        currentItem.forceActiveFocus()
    }
  }
}