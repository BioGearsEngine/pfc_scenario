import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root

  property bool collapsed : true
  property bool selected : false
  
  property SQLBackend backend 
  property EquipmentMap itemInScene
  property ListModel propertyModel : ListModel {}

  Component.onCompleted : {
    console.log ("SceneProperty Complete with height=%1,width=%2".arg(height).arg(width))
  }
  color : "transparent"
  border.color : "red"
  height : loader.item.height
  width : loader.item.width

  Loader {
    id : loader
    // anchors.fill : parent

    property string name : { console.log(root.itemInScene.name), root.itemInScene.name }
    property string notes : { console.log(root.itemInScene.notes), root.itemInScene.notes}
    property ListModel properties : { console.log(root.itemInScene.propertyModel), root.propertyModel}
    source : "ScenePropertySummary.qml"
    state : "collapsed"

    states : [
      State {
        name : "collapsed"
        when : root.collapsed
        PropertyChanges {
          target : loader;
          source : "ScenePropertySummary.qml"
        }
      },
      State {
        name : "expanded"
        when : !root.collapsed
        PropertyChanges {
          target : loader;
          source : "ScenePropertyDetails.qml"
        }
      }
    ]
  }
  onItemInSceneChanged : {
    var property_array = itemInScene.equipment.properties.split(';');
    var values_array = itemInScene.values.split(';');

    // Properties from a Syllabus.Equipment are in the form of Name,Type[;Name,Type]
    // The intiial split will give a list of Name,Type Pairs which needs to be split

    // Values are of the form value-list[;value-list] where a value list is based on the Type from Equipment
    // Boolean = true|false
    // Scalar  = #[,kind,unit]   - kind = "length|mass|volume|velocity|..."  unit = "m|mm|ml|m/s"
    // Range   = #,min,max       -  min/max are integral types

    root.propertyModel.clear();
    for (var ii = 0; ii < property_array.length; ++ ii) {
      var details = property_array[ii].split(',');
      var values = values_array[ii].split(',');
      switch (details[1]) {
        case "Boolean":
          root.propertyModel.append({"name": details[0], "type": "Boolean", "value": values[0]})
          break;
        case "Scalar":
          root.propertyModel.append({"name": details[0], "type": "Scalar", "value": values[0]})
          break
        case "Range":
          root.propertyModel.append({"name": details[0], "type": "Range", "value": values[0]})
          break
        default:
          root.propertyModel.append({"name": details[0], "type": "Default", "value": values[0]})
      }
    }
  }
}
