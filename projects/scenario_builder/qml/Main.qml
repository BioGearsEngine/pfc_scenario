import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3

ApplicationWindow {
  id:object1
  width: 600; height: 800
  minimumWidth:  600
  minimumHeight: 800 
  visible: true
  
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
         mainView.push( scenarioScreen )
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
          mainView.push( scenarioScreen, { model : null} )
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