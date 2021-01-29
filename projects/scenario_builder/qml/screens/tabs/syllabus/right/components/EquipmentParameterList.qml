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

  signal paramaterModified(int index, EquipmentParameter parameter)
  signal parameterAdded(int index, EquipmentParameter parameter)
  signal parameterRemoved(int index, EquipmentParameter parameter)

  EquipmentParameter {
    id : equipment_parameter_g
  }

  focus : true
  label : "Parameter"
  labelPlaural : "Parameters"
  spacing : 0
  buttonTwoVisible : false

   listButton.text : "Add"
   removeButton.text : "Remove"

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
        Layout.minimumHeight : childrenRect.height
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
          text : root.model[selfID].name 
          color : (enabled) ? "White" : Material.primaryTextColor 
          enabled : false
          readOnly : true
          activeFocusOnPress : false
          hoverEnabled : false
          selectByMouse : true
          onAccepted: {     
            if( root.model[selfID] ){
              root.model[selfID].name  = text
              paramaterModified(selfID, root.model[selfID].name )
            }   
          }
          onFocusChanged : {
            if( root.model[selfID] ){
              root.model[selfID].name  = text
              paramaterModified(selfID, root.model[selfID].name )
            } 
          }
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
          currentIndex :  root.model[selfID].type 
          enabled : false
          
          onActivated : {
            if (root.model[selfID]) {
              //NOTE : I'm putting this aside for now. We have a problem Calling Equipment update here
              //       Causes a recursive call stack. Something wrong with combobox. We need to find away to guard against this
                       
              if(root.model[selfID].type != typeBox.model.get(currentIndex).enumValue){
                 root.model[selfID].type = typeBox.model.get(currentIndex).enumValue
              }
            }
          }

          onFocusChanged : {
            paramaterModified(index, root.model[index])
          }
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
              text : "REAL";
              enumValue : Sustain.REAL
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
        Layout.minimumHeight : childrenRect.height
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
          onSourceComponentChanged : {
 
          }
          property EquipmentParameter curParam : (root.model) ? root.model[index] : undefined
          
          property Component rangeDelegate : Component {
            RowLayout {
              anchors.left : parent.left
              anchors.right : parent.right
              height : 50      
              Label {
                id: minLabel
                text : "min"
                Layout.preferredHeight : height
              }
              TextField {
                id: minField
                focus: true
                inputMethodHints: Qt.ImhDigitsOnly
                text: root.model[index].fields[0].value
                selectByMouse : true
                onAccepted: {
                  if ( text != root.model[index].fields[0].value) {
                    root.model[index].fields[0].value = text
                    paramaterModified(index, root.model[index])
                  }
                }
              }
              Label {
                id: maxLable
                text : "max"
              }
              TextField {
                id: maxField
                focus: true
                inputMethodHints: Qt.ImhDigitsOnly
                text: root.model[index].fields[1].value
                selectByMouse : true
                onAccepted: {
                  if ( text != root.model[index].fields[1].value) {
                    root.model[index].fields[1].value = text
                    paramaterModified(index, root.model[index])
                  }
                }
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
                Layout.minimumWidth : 10
              }
              ComboBox {
                id : unitBox
                focus : true
                Layout.leftMargin : 10
                textRole : "text"
                currentIndex : (root.model[selfID].fields[1] && root.model[selfID].fields[1].value ) ? root.model[selfID].fields[1].value : 0
                enabled : true
                onActivated : {
                  if ( root.model[selfID].fields[0].value != unitBox.model.get(unitBox.currentIndex).text) {
                    root.model[selfID].fields[0].value = unitBox.model.get(unitBox.currentIndex).text
                    root.model[selfID].fields[1].value = currentIndex
                  }
                }
                onFocusChanged : {
                  paramaterModified(index, root.model[index])
                }
                onHoveredChanged : {}
                onPressedChanged : {}
                
                flat : true
                model : ListModel {
                  ListElement {
                    text : "Volume";
                    unit : "ml"
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
            RowLayout{      
              anchors.left : parent.left
              anchors.right : parent.right
                      
              height : 125    
              Label {
                height : 25
                id: optionLabel
                text : "Options:"
                Layout.minimumWidth : 10
              }
              TextArea {
                id: optionField
                height : 100
                Layout.fillWidth : true
                wrapMode: Text.Wrap
                selectByMouse : true
                text : {
                  var str = "";
                  var options = root.model[index].enumOptions;
                  for ( var ii = 0; ii < options.length; ii++ ){
                    if( options[ii] ){
                      str += ",%1".arg( options[ii] )
                    }
                  }
                  str = str.substr(1,str.length)
                  return str;
                }
                placeholderText: "Comma delimited list of enum values"
                onActiveFocusChanged: {
                   root.model[index].enumOptions = text.split(",")
                  paramaterModified(index, root.model[index]);
                }
                onFocusChanged : {
                   root.model[index].enumOptions = text.split(",")
                  paramaterModified(index, root.model[index]);
                }
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
            if(!curParam){
              return commonDelegate
            }
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
  onList : { //Using List Button as the New Parameter Button
     parameterAdded(index, root.model[index])
  }
  onRemoved : {
     parameterRemoved(index, root.model[index])
  }
}
