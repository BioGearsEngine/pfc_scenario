import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../../common/"

import com.ara.pfc.ScenarioModel.SQL 1.0

CrossReferenceForm {
  id : root
  property SQLBackend backend

  signal parameterAdded(int index, EquipmentParameter parameter)
  signal parameterRemoved(int index, EquipmentParameter parameter)

  label : "Parameter"
  labelPlaural : "Parameters"
  spacing : 0
  buttonTwoVisible : false

  delegate : MouseArea {
    id : parameterMouseArea
    property int selfID : index

    anchors {
      left : parent.left
      right : parent.right
    }

    height : childrenRect.height
    enabled : true
    propagateComposedEvents : true
    onClicked : {
      root.current = index
    }
    
    ColumnLayout {
      id : parameter
      anchors.left : parent.left
      anchors.right : parent.right
      spacing : 0
      Rectangle {
        id: parameterHeader
        Layout.fillWidth : true
        height : childrenRect.height
        color : (enabled) ? Material.primary : Material.color(Material.BlueGrey, Material.Shade100)
        enabled : false
        Label {
          id : nameLabel
          anchors {
            left : parameterHeader.left
            top : parameterHeader.top
            leftMargin : 5
            topMargin : 2
          }
          font.pointSize : (enabled) ? 12 : 10
          text : "Name"
          color : (enabled) ? "White" : Material.primaryTextColor 

        }
        TextField {
          id : nameField
          anchors {
            left : nameLabel.right
            top : parameterHeader.top
            leftMargin : 50
            topMargin : 2
          }
          font.pointSize : (enabled) ? 12 : 10
          text : "commonDelegate"
          color : (enabled) ? "White" : Material.primaryTextColor 
          enabled : false
          readOnly : true
          activeFocusOnPress : false
          hoverEnabled : false
        }
        Label {
          id : typeLabel
          anchors {
            right : typeBox.left
            rightMargin : 50
            top : parameterHeader.top
            topMargin : 2
          }
          font.pointSize : (enabled) ? 12 : 10
          text : "Type"
          color : (enabled) ? "White" : Material.primaryTextColor 
          enabled : false
        }
        ComboBox {
          id : typeBox
          focus : true
          anchors {
            right : parameterHeader.right
            top : parameterHeader.top
            rightMargin : 50
            topMargin : 2
          }
          textRole : "text"
          currentIndex : root.model[index].type
          enabled : false
          onActivated : {
            if (root.model[selfID]) {
              root.model[selfID].type = typeBox.model.get(currentIndex).enumValue
            }
          }
          onHoveredChanged : {}
          onPressedChanged : {}
          flat : true
          model : ListModel {
            ListElement {
              text : "UNKNOWN";
              enumValue : Sustain.UNKNOWN
            }
            ListElement {
              text : "STRING";
              enumValue : Sustain.STRING
            }
            ListElement {
              text : "BOOLEAN";
              enumValue : Sustain.BOOLEAN
            }
            ListElement {
              text : "INTEGRAL";
              enumValue : Sustain.INTEGRAL
            }
            ListElement {
              text : "RANGE";
              enumValue : Sustain.RANGE
            }
            ListElement {
              text : "SCALAR";
              enumValue : Sustain.SCALAR
            }
            ListElement {
              text : "ENUM";
              enumValue : Sustain.ENUM
            }
          }
        }
      }

      Rectangle {
        id : parameterBody
        Layout.fillWidth : true
        height : parameterBodyLoader.height
        color : Material.backgroundColor
        border.color : Material.color(Material.Blue)
        
        visible : false
        Loader {
          id : parameterBodyLoader
          anchors {
            left : parameterBody.left;
            right : parameterBody.right;
            top : parameterBody.top;
          }
          height : (item !== null && typeof(item)!== 'undefined')? item.height: 0
          
          property EquipmentParameter curParam : (root.model) ? root.model[index] : undefined
          
          property Component rangeDelegate : Component {
            RowLayout {
              anchors.left : parent.left
              anchors.right : parent.right
                      
              height : minLabel.height
              Label {
                id: minLabel
                text : "min"
                height : 50
              }
              TextField {
                id: minField
              }
              Label {
                id: maxLable
                text : "max"
              }
              TextField {
                id: maxField
              }
            }
          }
          property Component scalarDelegate : Component {
            RowLayout{      
              anchors.left : parent.left
              anchors.right : parent.right
                      
              height : unitBox.height        
              Label {
                id: unitLabel
                text : "Unit:"
                Layout.minimumWidth : 100
              }
              ComboBox {
                id : unitBox
                focus : true
                Layout.leftMargin : 10
                textRole : "text"
                currentIndex : root.model[index].type
                enabled : false
                onActivated : {
                  if (root.model[selfID]) {
                    root.model[selfID].type = unitBox.model.get(currentIndex).enumValue
                  }
                }
                onHoveredChanged : {}
                onPressedChanged : {}
                flat : true
                model : ListModel {
                  ListElement {
                    text : "Volume";
                    umit : "ml"
                  }
                  ListElement {
                    text : "Area";
                    unit : "mm^2"
                  }
                  ListElement {
                    text : "Length";
                    unit : "mm"
                  }
                  ListElement {
                    text : "Power";
                    unit : "W"
                  }
                  ListElement {
                    text : "Time";
                    unit : "s"
                  }
                }
              }
            }
          }
          property Component enumDelegate : Component {
            ColumnLayout {
              anchors {
                fill : parent
              }
              TwoButtonRow {
                id : enumButtonRow
                Layout.fillWidth : true
                firstButtonText : "Add"
                secondButtonText : "Remove"

                onFirstButtonClicked : {
                  enumList.model.push("VALUE_%1".arg(enumList.model.length))
                }

                onSecondButtonClicked : {
                  enumList.model.splice(enumList.currentIndex, 1)
                }
              }
              ListView {
                id : enumList
                model : []
                Layout.fillWidth : true
                delegate : Rectangle {
                  TextField {
                    text : model.text
                  }
                }
                ScrollBar.vertical : ScrollBar {}
              }
            }
          }
          property Component commonDelegate : Component {
            Text {
              text : "No additional fields"
              font.pointSize : 12
              height : 25
            }
          }

          sourceComponent : {
            switch (curParam.type) {
              case Sustain.RANGE:
                return rangeDelegate
              case Sustain.SCALAR:
                return scalarDelegate
              case Sustain.ENUM:
                return enumDelegate
              case Sustain.STRING:
              case Sustain.BOOLEAN:
              case Sustain.INTEGRAL:
              default:
                return commonDelegate
            }
          }
        }
      }
    }
    
    states : State {
      name : "Selected"
      PropertyChanges { target : parameterHeader; enabled : true }
      //PropertyChanges { target : parameterBoarder; enabled : true }
      PropertyChanges { target : nameLabel; enabled : true }
      PropertyChanges { target : typeLabel; enabled : true }
      
      PropertyChanges { target : nameField; enabled : true }
      PropertyChanges { target : nameField; readOnly : false }
      PropertyChanges { target : nameField; activeFocusOnPress : true }
      PropertyChanges { target : nameField; hoverEnabled : true }
      PropertyChanges { target : nameField; mouseSelectionMode : TextInput.SelectCharacters      }
      
      PropertyChanges { target : typeBox; enabled : true }
      
      PropertyChanges { target : parameterBody; visible : true }
      // PropertyChanges { target : parameterBodyLoader; enabled : true }
      PropertyChanges { target : parameterMouseArea; enabled : false }
    }

    Connections {
      target : root
      onCurrentChanged : {
        if (root.current == index) {
          state = 'Selected';
        } else {
          state = '';
        }
      }
    }  
  }
  onList : {}
  onAdded : {}
  onRemoved : {}
}
