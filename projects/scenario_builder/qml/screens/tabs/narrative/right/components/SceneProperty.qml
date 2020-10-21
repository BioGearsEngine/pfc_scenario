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
  }
  color : "transparent"
  border.color : "red"
  height : loader.item.height
  width : loader.item.width

  Loader {
    id : loader
    anchors.left : parent.left
    anchors.right: parent.right

    property string name : root.itemInScene.name 
    property string type : root.itemInScene.equipment.name 
    property string notes :  root.itemInScene.notes
    property ListModel properties : root.propertyModel
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

    backend.select_equipment(itemInScene.equipment)
    backend.select_scene(itemInScene.scene)
    var item_properties = itemInScene.equipment.properties.split(';');
    var item_values     = itemInScene.values.split(';');
    // Properties from a Syllabus.Equipment are in the form of Name:Type[:fields][;Name:Type:[fields]]
    //                                                         fields=name,type[:name,type]
    // The intiial split will give a list of Name,Type Pairs which needs to be split

    // Values are of the form value-list[;value-list] where a value list is based on the Type from Equipment
    // Boolean = true|false
    // Scalar  = #[,kind,unit]   - kind = "length|mass|volume|velocity|..."  unit = "m|mm|ml|m/s"
    // Range   = #,min,max       -  min/max are integral types

    root.propertyModel.clear();
    for (var ii = 0; ii < item_properties.length; ++ ii) {
      var property_details = item_properties[ii].split(':');
      var property_values = item_values[ii].split(':');
      switch (property_details[1].toLowerCase()) {
        case "boolean":
          root.propertyModel.append({"name": property_details[0], "type": "boolean", "value": property_values[0]})
          break;
        case "scalar":
          root.propertyModel.append({"name": property_details[0], "type": "Scalar", "value": property_values[0], "unit" : property_values[1]})
          break
        case "integral":
          root.propertyModel.append({"name": property_details[0], "type": "integral", "value": property_values[0]})
          break
        case "range":
          root.propertyModel.append({"name": property_details[0], "type": "range", "value": property_values[0],
                                     "min": property_values[1],"max": property_values[2],"step": property_values[3]})
          break
        case "string":
          root.propertyModel.append({"name": property_details[0], "type": "string", "value": property_values[0]})
          break
        default:
          root.propertyModel.append({"name": property_details[0], "type": "Unsupported Type", "value": property_values[0]})
      }
    }
  }
}
