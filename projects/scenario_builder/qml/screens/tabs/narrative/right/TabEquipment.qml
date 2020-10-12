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
  property ListModel sceneList
  property int sceneIndex

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
        console.log(index,objectsInSceneView.equipmentInScene[index].name)
        objectsInSceneView.equipmentInScene[index]
      }
      backend : root.backend
    }

    Rectangle {
      height : parent.height
      width  : parent.width
      x: 0
      y : 0
      z : -1
      color : "grey"
      border.color : "yellow"
    }
  }

  onSceneIndexChanged : {
    if (sceneList.count != 0 && sceneIndex < sceneList.count) {
      console.log("TabEquipment on Index Changed %1".arg(sceneIndex));
      var values = sceneList.get(sceneIndex);
      objectsInSceneView.equipmentInScene = []
      currentEquipmentMap.scene.scene_id = root.sceneList.get(root.sceneIndex).id
      currentEquipmentMap.scene.name = root.sceneList.get(root.sceneIndex).name
      root.backend.equipment_in_scene(currentEquipmentMap.scene)

      while (root.backend.next_equipment_map(currentEquipmentMap)) {
        objectsInSceneView.equipmentInScene.push(currentEquipmentMap.make())
        objectsInSceneView.equipmentInScene[objectsInSceneView.equipmentInScene.length-1].assign(currentEquipmentMap)
      }
      objectsInSceneView.model = objectsInSceneView.equipmentInScene
    } else {  objectsInSceneView.model = [] }
  }
}
