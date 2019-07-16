import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
  id: syllabus_rightWindow
  width: (parent.width / 2) - 10
  height: parent.height
  StackLayout {
    id: syllabus_stack_right
    currentIndex: syllabus_tabs.currentIndex
    width: syllabus_rightWindow.width
    height: syllabus_rightWindow.height
    Layout.fillWidth: true
    Layout.fillHeight: true
    //TAB:SYLLABUS_TAB:OBJECTIVES_RIGHTWINDOW
    Rectangle {
      id: syllabus_rightWindow_objectives
      width: (syllabus_rightWindow.width / 2) - 10 
      height: syllabus_rightWindow.height
      Layout.alignment: Qt.AlignHCenter
      clip: true
      Row {
        id: syllabus_rightWindow_objectives_medicalName
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        topPadding: 5
        width: syllabus_rightWindow.width
        height: syllabus_rightWindow.height / 10
        Rectangle {
          color: 'orange'
          width: parent.width / 5 - 10
          height: 25
          Text {
            text: 'Medical Name:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
              width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
              height: 25
              color: 'pink'
              TextInput {
                id: medicalName
                text: 'String Field (128 Characters)'
                maximumLength: 128
                cursorVisible: false
                anchors.centerIn: parent
              }
        }
      }
      Row {
        id: syllabus_rightWindow_objectives_commonName
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        width: syllabus_rightWindow.width
        height: syllabus_rightWindow.height / 10
        anchors.top: syllabus_rightWindow_objectives_medicalName.bottom
        Rectangle {
          color: 'orange'
          width: parent.width / 5 - 10
          height: 25
          Text {
            text: 'Common Name:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
              width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
              height: 25
              color: 'pink'
              TextInput {
                id: commonName
                text: 'String Field (128 Characters)'
                maximumLength: 128
                cursorVisible: false
                anchors.centerIn: parent
              }
        }
      }
      Row {
        id: syllabus_rightWindow_objectives_references
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        width: parent.width
        height: parent.height / 3
        anchors.top: syllabus_rightWindow_objectives_commonName.bottom
        Rectangle {
          color: 'orange'
          width: parent.width / 5 - 10
          height: 25
          Text {
            text: 'References:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'pink'
          width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
          height: parent.height
          Row {
            width: parent.width
            height: parent.height
            Rectangle {
              color: 'pink'
              width: parent.width - 25
              height: parent.height
              Row {
                spacing: 10
                leftPadding: 5
                rightPadding: 5
                topPadding: 5
                width: parent.width
                height: 25
                Rectangle { // Add
                  color: 'lightblue'
                  width: parent.width / 2 - 20
                  height: 25
                  Text {
                    text: 'Add'
                    anchors.centerIn: parent
                  }
                }
                Rectangle { // Remove
                  color: 'plum'
                  width: parent.width / 2 - 20
                  height: 25
                  Text {
                    text: 'Remove'
                    anchors.centerIn: parent
                  }
                }
              }
              ListView { // Reference List

              }
            }
            Rectangle { // Expander
              color: 'green'
              height: parent.height
              width: 25
              Text {
                text: 'Expander'
                rotation: 90
                anchors.centerIn: parent
              }
            }
          }
        }
      }
      Row {
        id: syllabus_rightWindow_objectives_description
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        topPadding: 20
        width: parent.width
        height: parent.height / 3
        anchors.top: syllabus_rightWindow_objectives_references.bottom
        Rectangle {
          color: 'orange'
          width: parent.width / 5 - 10
          height: 25
          Text {
            text: 'Description:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'pink'
          width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
          height: parent.height 
          Row {
            width: parent.width
            height: parent.height
            TextInput { // Text Area
              width: parent.width - 25
              height: parent.height
              Text {
                text: 'Text Area'
              }
            }
            Rectangle { // Expander
              color: 'green'
              width: 25
              height: parent.height
              Text {
                text: 'Expands'
                rotation: 90
                anchors.centerIn: parent
              }
            }
          }
        }
      }
      Row {
        id: syllabus_rightWindow_objectives_severityRange
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        topPadding: 40
        width: parent.width
        height: parent.height / 3
        anchors.top: syllabus_rightWindow_objectives_description.bottom
        Rectangle {
          color: 'red'
          width: parent.width / 6
          height: 25
          Text {
            text: 'Severity Range:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'yellow'
          width: parent.width / 6
          height: 25
          Text {
            text: 'Min:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'lightblue'
          width: parent.width / 6
          height: 25
          Text {
            text: 'Number Spinner'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'green'
          width: parent.width / 6
          height: 25
          Text {
            text: 'Max:'
            anchors.centerIn: parent
          }
        }
        Rectangle {
          color: 'plum'
          width: parent.width / 6
          height: 25
          Text {
            text: 'Number Spinner'
            anchors.centerIn: parent
          }
        }
      }
      ScrollBar {
        id: syllabus_rightWindow_objectives_vbar
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
        size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
        anchors.top: syllabus_rightWindow_objectives.top
        anchors.right: syllabus_rightWindow_objectives.right
        anchors.bottom: syllabus_rightWindow_objectives.bottom
      }
    }
    //TAB:SYLLABUS_TAB:ASSESSMENTS_RIGHTWINDOW
    Rectangle {
      id: syllabus_rightWindow_assessments
      width: (syllabus_rightWindow.width / 2) - 10
      height: syllabus_rightWindow.height
      clip: true
      color: 'blue'
      ScrollBar {
        id: syllabus_rightWindow_assessments_vbar
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
        size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
        anchors.top: syllabus_rightWindow_assessments.top
        anchors.right: syllabus_rightWindow_assessments.right
        anchors.bottom: syllabus_rightWindow_assessments.bottom
      }
    }
    StackLayout {
      width: (syllabus_rightWindow.width / 2) - 10
      height: syllabus_rightWindow.height
      currentIndex: syllabus_injuries_tabs.currentIndex
      //TAB:SYLLABUS_TAB:INJURIES_TAB:DEFINITIONS_RIGHTWINDOW
      Rectangle {
        id: syllabus_rightWindow_injuries_definitions
        width: parent.width
        height: parent.height
        clip: true
        color: 'blue'
        ScrollBar {
          id: syllabus_rightWindow_injuries_definitions_vbar
          hoverEnabled: true
          active: hovered || pressed
          orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
          size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
          anchors.top: syllabus_rightWindow_injuries_definitions.top
          anchors.right: syllabus_rightWindow_injuries_definitions.right
          anchors.bottom: syllabus_rightWindow_injuries_definitions.bottom
        }
      }
      Rectangle {
        //TAB:SYLLABUS_TAB:INJURIES_TAB:INJURY-SETS_RIGHTWINDOW
        id: syllabus_rightWindow_injuries_injurySets
        width: parent.width
        height: parent.height
        clip: true
        color: 'blue'
        ScrollBar {
          id: syllabus_rightWindow_injuries_injurySets_vbar
          hoverEnabled: true
          active: hovered || pressed
          orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
          size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
          anchors.top: syllabus_rightWindow_injuries_injurySets.top
          anchors.right: syllabus_rightWindow_injuries_injurySets.right
          anchors.bottom: syllabus_rightWindow_injuries_injurySets.bottom
        }
      }
    }
    //TAB:SYLLABUS_TAB:TREATMENTS_RIGHTWINDOW
    Rectangle {
      id: syllabus_rightWindow_treatments
      width: (syllabus_rightWindow.width / 2) - 10
      height: syllabus_rightWindow.height
      clip: true
      color: 'blue'
      ScrollBar {
        id: syllabus_rightWindow_treatments_vbar
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
        size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
        anchors.top: syllabus_rightWindow_treatments.top
        anchors.right: syllabus_rightWindow_treatments.right
        anchors.bottom: syllabus_rightWindow_treatments.bottom
      }
    }
    //TAB:SYLLABUS_TAB:EQUIPMENT_RIGHTWINDOW
    Rectangle {
      id: syllabus_rightWindow_equipment
      width: (syllabus_rightWindow.width / 2) - 10
      height: syllabus_rightWindow.height
      clip: true
      color: 'blue'
      ScrollBar {
        id: syllabus_rightWindow_equipment_vbar
        hoverEnabled: true
        active: hovered || pressed
        orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
        size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
        anchors.top: syllabus_rightWindow_equipment.top
        anchors.right: syllabus_rightWindow_equipment.right
        anchors.bottom: syllabus_rightWindow_equipment.bottom
      }
    }
  }
}