import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation
  Equipment {
    id : equipment_g
  }
  EquipmentMap {
    id : equipmentMap_g
  }
  Scene {
    id : self_scene
  }

  StackLayout {
    id : equipment_stack
    currentIndex : 1
    Rectangle {
      id : knownListRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      color : "transparent"
      border.color : "black"

      PFCButton {
        id : knownExitButton
        text : "Exit"
        anchors {
          top : knownListRectangle.top
          horizontalCenter : knownListRectangle.horizontalCenter
        }

        onClicked : {
          equipment_stack.currentIndex = 1
        }
      }

      ListView {
        id : knownEquipmentList
        property var equipment: (backend) ? backend.equipment : []
        anchors {
          top : knownExitButton.bottom
          left : knownListRectangle.left
          right : knownListRectangle.right
          bottom : knownListRectangle.bottom
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }
        model : equipment
        delegate : Rectangle {
          id : fullequipment
          color : (selected) ? Material.color(Material.Cyan, Material.Shade500) : Material.color(Material.Cyan, Material.Shade100)
          border.color : "steelblue"
          height : equipment_value_text.height + equipment_title_label.height + 5
          property bool selected : false
          property var currentDef: knownEquipmentList.equipment[index]
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          Label {
            id : equipment_title_label
            anchors.left : parent.left
            anchors.leftMargin : 5
            text : "Name: "
            width : 50
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? "white" : Material.secondaryTextColor
          }
          Text {
            id : equipment_title_text
            anchors.left : equipment_title_label.right
            anchors.leftMargin : 5
            text : (knownEquipmentList.equipment[index]) ? knownEquipmentList.equipment[index].name : "Undefined"

            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : equipment_value_label
            anchors {
              top : equipment_title_text.bottom
              left : parent.left
              leftMargin : 5
              rightMargin : 10
            }
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Description: "
            enabled : false
            color : enabled ? "white" : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Text {
            id : equipment_value_text
            anchors.top : equipment_title_text.bottom
            anchors.left : equipment_value_label.right
            anchors.right : parent.right

            anchors.leftMargin : 2
            font.pointSize : 10
            wrapMode : Text.Wrap
            text : {
              if (currentDef) {
                if (!enabled) {
                  return currentDef.summary
                } else {
                  if (currentDef.description === "") {
                    return currentDef.summary
                  } else {
                    return currentDef.description
                  }
                }
              } else {
                return ""
              }
            }
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Label {
            id : equipment_parameter_label
            anchors {
              top : parent.top
              right : equipment_parameter_text.right
              leftMargin : 5
              rightMargin : 10
            }
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Parameters: "
            enabled : false
            color : enabled ? "white" : Material.secondaryTextColor
            elide : Text.ElideRight
          }
          Text {
            id : equipment_parameter_text
            anchors.top : parent.top
            anchors.right : parent.right

            anchors.leftMargin : 2
            font.pointSize : 10
            text : (knownEquipmentList.equipment[index]) ? knownEquipmentList.equipment[index].parameters.length : "0"
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              knownEquipmentList.currentIndex = index
            }
            onDoubleClicked : {
              if (currentScene && knownEquipmentList.equipment[knownEquipmentList.currentIndex]) {
                equipmentMap_g.clear()
                equipmentMap_g.scene.assign(currentScene);
                equipmentMap_g.equipment.assign(knownEquipmentList.equipment[knownEquipmentList.currentIndex]);
                root.backend.update_equipment_in_scene(equipmentMap_g);
                equipment_stack.currentIndex = 1
              }
            }
          }
          states : State {
            name : "Selected"
            PropertyChanges { target : fullequipment; selected : true }
            PropertyChanges { target : equipment_title_label; enabled : true }
            PropertyChanges { target : equipment_title_text; enabled : true }
            PropertyChanges { target : equipment_value_label; enabled : true }
            PropertyChanges { target : equipment_value_text; enabled : true }
            PropertyChanges { target : equipment_parameter_label; enabled : true }
            PropertyChanges { target : equipment_parameter_text; enabled : true }
          }
          onFocusChanged : {
            if (knownEquipmentList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }
    }

    Rectangle {
      id : listRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5

      border.color : "black"

      TwoButtonRow {
        id : controls
        anchors.top : listRectangle.top
        anchors.left : listRectangle.left
        anchors.right : listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5

        property int next : 1

        firstButtonText : "Add"
        secondButtonText : "Remove"

        onFirstButtonClicked : {
          equipment_stack.currentIndex = 0;
        }
        onSecondButtonClicked : {
          equipmentMap_g.clear();
          equipmentMap_g.assign(equipmentInSceneList.equipmentMaps[equipmentInSceneList.currentIndex]);
          var prevIndex = equipmentInSceneList.currentIndex;
          root.backend.remove_equipment_map(equipmentMap_g);
          equipmentInSceneList.currentIndex = Math.max(0, prevIndex);
        }
      }
      ListView {
        id : equipmentInSceneList
        property var equipmentMaps: []

        anchors {
          top : controls.bottom;
          bottom : parent.bottom;
          left : parent.left;
          right : parent.right
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }

        delegate : MouseArea {
          id : equipmentMouseArea

          property int selfID : index
          property var currentMap       : (equipmentInSceneList.equipmentMaps[index]) ? equipmentInSceneList.equipmentMaps[index] : null
          property var currentEquipment : (equipmentInSceneList.equipmentMaps[index]) ? equipmentInSceneList.equipmentMaps[index].equipment : null
          property var parameters       : (equipmentInSceneList.equipmentMaps[index]) ? equipmentInSceneList.equipmentMaps[index].equipment.parameters : null

          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          height : childrenRect.height
          enabled : true
          propagateComposedEvents : true
          onClicked : {
            equipmentInSceneList.currentIndex = index
          }

          states : State {
            name : "Selected"
            PropertyChanges { target : instanceHeader; enabled : true }
            PropertyChanges { target : instanceNameLabel; enabled : true }
            PropertyChanges { target : instanceTypeLabel; enabled : true }
            
            PropertyChanges { target : instanceNameField; enabled : true }
            PropertyChanges { target : instanceNameField; readOnly : false }
            PropertyChanges { target : instanceNameField; activeFocusOnPress : true }
            PropertyChanges { target : instanceNameField; hoverEnabled : true }
            PropertyChanges { target : instanceNameField; mouseSelectionMode : TextInput.SelectCharacters      }

            PropertyChanges { target : instanceBody; enabled : true }
            PropertyChanges { target : instanceBody; visible : true }
          }

          Connections {
            target : equipmentInSceneList
            onCurrentIndexChanged : {
              if (equipmentInSceneList.currentIndex == index) {
                state = 'Selected';
              } else {
                state = '';
              }
            }
          }

          ColumnLayout {
            id : equipmentColumn
            anchors.left : parent.left
            anchors.right : parent.right
            spacing : 0
            Rectangle {
              id : instanceHeader
              Layout.fillWidth : true
              Layout.minimumHeight : childrenRect.height
              color : (enabled) ? Material.color(Material.Cyan, Material.Shade500) : Material.color(Material.Cyan, Material.Shade100)
              enabled : false

              Label {
                id : instanceTypeLabel
                anchors {
                  left : instanceHeader.left
                  top : instanceHeader.top
                  leftMargin : 5
                  topMargin : 2
                }
                font.pointSize : (enabled) ? 12 : 10
                text : (currentEquipment) ? "Type: %1".arg(currentEquipment.name) : "Type: Unknown"
                color : (enabled) ? "White" : Material.primaryTextColor
              }
              Label {
                id : instanceNameLabel
                anchors {
                  left : instanceHeader.left
                  top : instanceTypeLabel.bottom
                  leftMargin : 5
                  topMargin : 2
                }
                font.pointSize : (enabled) ? 12 : 10
                text : "Name:"
                color : (enabled) ? "White" : Material.primaryTextColor
              }
              TextField {
                id : instanceNameField
                anchors {
                  left : instanceTypeLabel.right
                  right : parent.right
                  top : instanceHeader.top
                  leftMargin : 50
                  topMargin : 2
                }
                font.pointSize : (enabled) ? 12 : 10
                text : (currentMap) ? currentMap.name : "Unknown"
                color : (enabled) ? "White" : Material.primaryTextColor
                enabled : false
                readOnly : true
                activeFocusOnPress : false
                hoverEnabled : false
              }
            }
            Rectangle {
              id : instanceBody
              
              Layout.fillWidth : true
              Layout.minimumHeight : childrenRect.height
              color : Material.color(Material.Grey, Material.Shade100)
              enabled : false
              visible : false
              ListView {
                id : parameter
                anchors {
                  left : parent.left
                  right : parent.right
                  top : instanceBody.top
                }
                spacing : 0
                model : equipmentMouseArea.parameters
                height : childrenRect.height
                delegate : Rectangle {
                  id: parameterHeader

                  property int paramID : index
                  property var currentParameter : equipmentMouseArea.parameters[index]

                  Layout.fillWidth : true
                  Layout.minimumHeight : 25
                  height : childrenRect.height
                  color : (enabled) ? Material.primary : Material.color(Material.BlueGrey, Material.Shade100)
                  enabled : false
                  Label {
                    id : parameterNameLabel
                    anchors {
                      left : parameterHeader.left
                      top : parameterHeader.top
                      leftMargin : 5
                      topMargin : 2
                    }
                    font.pointSize : (enabled) ? 12 : 10
                    text : currentParameter.name
                    color : (enabled) ? "White" : Material.primaryTextColor 
                  }
                  Text {
                    id : parameterNameText
                    anchors {
                      left : parameterNameLabel.right
                      top : parameterHeader.top
                      leftMargin : 5
                      topMargin : 2
                    }
                    font.pointSize : (enabled) ? 12 : 10
                    text : currentParameter.typeString
                    color : (enabled) ? "White" : Material.primaryTextColor 
                  }
                  ListView {
                    id : fieldView
                    anchors {
                      top : parameterNameText.bottom
                      left : parent.left
                      right : parent.right
                    }
                    model : parameters[selfID].fields
                    delegate : Rectangle {
                       height : 50
                       width : 100
                       color : 'steelblue'
                    }
                  }
                }
              }
            }
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }
    }
  }

  onCurrentSceneChanged : {}

  onBackendChanged : {
    refresh_knownEquipment_list()
  }

  Connections {
    target : backend
    onEquipmentChanged : {
      refresh_knownEquipment_list()
    }
    onEquipmentMapsChanged : {
      refresh_equipment_list()
    }
  }

  function refresh_equipment_list() {
    var lastIndex = equipmentInSceneList.currentIndex
    equipmentInSceneList.equipmentMaps = []
    var equipmentMaps = root.backend.equipmentMaps;
    for (var ii = 0; ii < equipmentMaps.length; ++ ii) {
      if (equipmentMaps[ii].scene.id == currentScene.id) {
        equipmentInSceneList.equipmentMaps.push(equipmentMap_g.make());
        var index = equipmentInSceneList.equipmentMaps.length - 1
        equipmentInSceneList.equipmentMaps[index].assign(equipmentMaps[ii]);
      }
    }
    equipmentInSceneList.model = equipmentInSceneList.equipmentMaps
    equipmentInSceneList.currentIndex = lastIndex
  }
  function refresh_knownEquipment_list() {
    knownEquipmentList.equipment = []
    let equipments = root.backend.equipment;
    for (var ii = 0; ii < equipments.length; ++ ii) {
      knownEquipmentList.equipment.push(equipment_g.make());
      knownEquipmentList.equipment[knownEquipmentList.equipment.length - 1].assign(equipments[ii]);
    }
    knownEquipmentList.equipment.model = knownEquipmentList.equipment;
  }
}
