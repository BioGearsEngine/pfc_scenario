import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

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
         loadScreen.open()
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
      id: loadScreen
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
          console.log("You chose: " + fileUrls)
          //TODO: Handle Failure
          scenario_model.Load( loadScreen.fileUrls );
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