import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Item {
id:root
property var stack
ListModel {
    id: scenario_model
    ListElement {
        name: "Joe"
    }
    ListElement {
        name: "Jill"
    }
    ListElement {
        name: "ManDoog"
    }
  }

  Component {
    id: delegateComp

    RowLayout
    {
      width: parent.width
      Text {
        id: scenario_model_label
        font.pointSize: 10
        text: "Name:"
        font.weight: Font.Bold
        verticalAlignment : Text.AlignTop
        anchors {
          left : parent.left
        }
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: 75
      }

      Text {
        id: scenario_model_content
        font.pointSize: 10
        text: name
        wrapMode: Text.Wrap
        horizontalAlignment : Text.AlignJustify

         anchors {
          left : scenario_model_label.right
          rightMargin: 10
        }
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
      }

      RoundButton {
        id: control
        text: qsTr("Edit")
        font.pointSize : 8

        Layout.alignment: Qt.AlignTop

        Component {
          id: dialog
          Screen_Scenario_Actors_Dialog { 
          }
        }
        onClicked: {
          stack.push(
            dialog, { data_model : model}
          )
        }
      }
    }
  }

  Label {
    id: header
    text: "Actors"
    font.pointSize: 14
    font.italic: true
    color: "steelblue"

    anchors {
      top: parent.top
      horizontalCenter : parent.horizontalCenter
    }
    Layout.alignment: Qt.AlignVCenter
  }

  ListView {
    id:actorList
    anchors {
      top: header.bottom
      left: parent.left;  leftMargin: 10
      right: parent.right; rightMargin: 10
    }
    spacing: 2
    clip:true
    orientation:ListView.Vertical
    model: scenario_model
    delegate: delegateComp
    onContentHeightChanged: {
      if ( height < contentHeight) {
        height = contentHeight
      }
      root.height =root.height = childrenRect.height+ header.height+button.height;
    }
    onHeightChanged: {
      if ( height < contentHeight) {
           height = contentHeight
      }
    }
  }

  Button {
    id: button
    anchors {
      bottom : parent.bottom
      horizontalCenter: parent.horizontalCenter
    }
   
    text: "New Actor"

    onClicked: {
      scenario_model.append({ name: " Odan is a friendly African nation whose democratic government is being threatened by a warlord who has built a large militia. \
The militia has invaded several cities, is terrorizing locals and is stopping humanitarian aid from getting to the populous to \
feed their militia. To support the host nation, US forces send several small groups to train host nation military operators who will \
lead the campaign against the militia. To stay out of notice from the militia, US military meet host nation personnel in a far forward \
location. They are dropped off by helicopter and assume a 3-day training. The host nation and US military personnel set up a makeshift \
base camp. There are ~20 host nation soldiers and 5 US personnel. During training, a US military person is shot in the inner thigh with \
a AK47 (7.62x39) rifle. The medic was on a short recon mission to ensure perimeter security and make sure no militia were present in the \
area. When they arrive back at the point of injury, a buddy had applied pressure and a tourniquet to the person. At POI, the medic \
assesses the wound, makes sure the tourniquet is secure, gives the person a phentnyl lollipop, and then transports them to the base \
camp to a semi-open tent environment. Within the tent environment, the casualty will be treated and stabilized by the medic." })
    }
  }
  height : actorList.height + header.height + button.height 
}