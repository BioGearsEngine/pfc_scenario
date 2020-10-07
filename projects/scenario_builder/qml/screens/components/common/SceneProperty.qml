import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

RowLayout {
  id : root
  property var object

  Text {
      text : model.name
  }
  Text {
      text : model.description
  }

  Component.onCompleted : {
    console.log( "Setup %1 - %2".arg(model.name).arg(model.description))
  }
}