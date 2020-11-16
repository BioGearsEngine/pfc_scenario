import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "components"
import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation

  EquipmentMap {
    id : currentEquipmentMap
  }

  function update_role(values) {
    obj.role_id = values.id
    obj.name = values.name
    obj.description = values.description
    root.backend.update_role(obj)
  }

  ListView {
    id : objectsInSceneView

    property var equipmentInScene

    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.margins : 5
    model : []

    delegate : SceneProperty {
      Layout.fillWidth : true
      Layout.minimumHeight : 25
      Layout.preferredHeight : 25
      anchors.left : parent.left
      anchors.right : parent.right

      itemInScene : {
        objectsInSceneView.equipmentInScene[index]
      }
      backend : root.backend
    }

    Rectangle {
      height : parent.height
      width : parent.width
      x : 0
      y : 0
      z : -1
      border.color : "yellow"
    }
  }
}
