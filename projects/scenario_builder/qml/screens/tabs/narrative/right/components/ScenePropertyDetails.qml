import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root

  property ListModel properties
  property string name 
  property string notes

  property ListModel properties

  Repeater {
    Layout.fillWidth : true
    Layout.minimumWidth : 200
    Layout.minimumHeight : 50
    Layout.preferredHeight : 50
    model : root.propertyModel
    delegate : RowLayout {
      Text {
        text : model.name
      }
      Text {
        text : model.type
      }
      Text {
        text : model.value
      }
    }
  }
}
