import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root

  // property ListModel properties  //So, Loader properties are overriden by properties in a Component file
  // property string name           //This class requires 3 properties, but how to make that apparent if you can't define them in the file
  // property string notes


  RowLayout {
    Layout.fillWidth : true
    Layout.preferredHeight : nameText.contentHeight
    Layout.minimumHeight : nameText.contentHeight
    spacing : 15
    Text {
      id : nameText
      text : name
    }

    Text {
      id : notesText
      text : notes
    }
  }
  // Repeater {
  //   Layout.fillWidth : true
  //   Layout.minimumWidth : 200
  //   Layout.minimumHeight : 40
  //   Layout.preferredHeight : 40
  //   model : root.properties
  //   delegate : RowLayout {
  //     Text {
  //       text : model.name
  //     }
  //     Text {
  //       text : model.type
  //     }
  //     Text {
  //       text : model.value
  //     }
  //   }
  // }
  Component.onCompleted: {
    console.log("root.name = %1, root.notes = %2".arg(name).arg(notes))
  }
}
