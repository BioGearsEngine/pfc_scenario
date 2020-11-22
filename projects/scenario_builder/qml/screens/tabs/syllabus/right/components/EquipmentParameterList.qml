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

  buttonTwoVisible : false

  delegate : Rectangle {
    id : parameter
    
    property int selfID : index

    color : 'transparent'
    border.color : "transparent"
    height : childrenRect.height
    anchors {
      left : parent.left;
      right : parent.right;
      margins : 5
    }

    Rectangle {
      id : parameterHeader
      color : (enabled) ? Material.primary : Material.color(Material.BlueGrey, Material.Shade100)
      enabled : false
      height : childrenRect.height
      anchors {
        left : parent.left;
        right : parent.right;

      }
      Label {
        id : nameLabel
        anchors {
          left : parameterHeader.left
          top : parameterHeader.top
          topMargin : 2
        }
        font.pointSize : (enabled) ? 12 : 10
        text : "Name"
        color : 'white'

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
        color : 'white'
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
        color : 'white'
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
          if ( root.model[selfID] ){
            root.model[selfID].type = typeBox.model.get(currentIndex).enumValue
          }
        }
        onHoveredChanged : {        }
        onPressedChanged : {        }
        flat : true
        model : ListModel {
          ListElement { text : "UNKNOWN"; enumValue : Sustain.UNKNOWN }
          ListElement { text : "STRING"; enumValue : Sustain.STRING }
          ListElement { text : "BOOLEAN"; enumValue : Sustain.BOOLEAN }
          ListElement { text : "INTEGRAL"; enumValue : Sustain.INTEGRAL }
          ListElement { text : "RANGE"; enumValue : Sustain.RANGE }
          ListElement { text : "SCALAR"; enumValue : Sustain.SCALAR }
          ListElement { text : "ENUM"; enumValue : Sustain.ENUM }
        }
      }
    }

    Rectangle {
      id : parameterBody
      anchors.top : parameterHeader.bottom
      anchors.left : parent.left
      anchors.right : parent.right
      height : childrenRect.height
      color : Material.backgroundColor
      border.color : Material.color(Material.Blue)
      Loader {
        id: parameterBodyLoader
        property EquipmentParameter curParam : (root.model) ? root.model[index] : undefined
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

    MouseArea {
      id: mouseArea
      anchors.fill : parent
      enabled : true
      propagateComposedEvents: true
      onClicked : {
        root.current = index
      }
    }

    property Component rangeDelegate : Component {
      Text {
        text : "rangeDelegate"
        font.pointSize : 12
      }
    }
    property Component scalarDelegate : Component {
      Text {
        text : "scalarDelegate"
        font.pointSize : 12
      }
    }
    property Component enumDelegate : Component {
      Text {
        text : "enumDelegate"
        font.pointSize : 12
      }
    }
    property Component commonDelegate : Component {
      Text {
        text : "commonDelegate"
        font.pointSize : 12
      }
    }

    states : State {
      name : "Selected"
      PropertyChanges { target : parameterHeader; enabled : true }
      PropertyChanges { target : nameLabel; enabled : true }
      PropertyChanges { target : typeLabel; enabled : true }
      
      PropertyChanges { target : typeBox; enabled : true }
      PropertyChanges { target : typeBox; enabled : true }
      
      PropertyChanges { target : parameterBody; enabled : true }
      PropertyChanges { target : parameterBodyLoader; enabled : true }
      PropertyChanges { target : mouseArea; enabled : false }

      PropertyChanges { target : nameField; readOnly : false }
      PropertyChanges { target : nameField; activeFocusOnPress : true }
      PropertyChanges { target : nameField; hoverEnabled : true }
      PropertyChanges { target : nameField; enabled : true }
      PropertyChanges { target : nameField; mouseSelectionMode : TextInput.SelectCharacters }
    }

    Connections {
      target : root
      onCurrentChanged :  {
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
