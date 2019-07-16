import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Rectangle {
  width: parent.width / 2 - 10
  height: parent.height
  id: syllabus_leftWindow
  TabBar {
    id: syllabus_tabs
    width: parent.width
    Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
    TabButton {
      text: "Objectives"
      width: implicitWidth
    }
    TabButton {
      text: "Assessments"
      width: implicitWidth
    }
    TabButton {
      text: "Injuries"
      width: implicitWidth
    }
    TabButton {
      text: "Treatments"
      width: implicitWidth
    }
    TabButton {
      text: "Equipment"
      width: implicitWidth
    }
  }

  Row {
    id: syllabus_leftWindow_buttons
    width: parent.width
    height: 25
    anchors.top: syllabus_tabs.bottom
    Rectangle {
      color: 'green'
      width: parent.width / 4
      height: 25
      Text {
        text: 'New'
        anchors.centerIn: parent
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {

        }
      }
    }
    Rectangle {
      color: 'orange'
      width: parent.width / 4
      height: 25
      Text {
        text: 'Move Up'
        anchors.centerIn: parent
      }
    }
    Rectangle {
      color: 'blue'
      width: parent.width / 4
      height: 25
      Text {
        text: 'Move Down'
        anchors.centerIn: parent
      }
    }
    Rectangle {
      color: 'plum'
      width: parent.width / 4
      height: 25
      Text {
        text: 'Delete'
        anchors.centerIn: parent
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          
        }
      }
    }
  }
  Rectangle {
    width: parent.width
    height: parent.height  - syllabus_tabs.height - syllabus_leftWindow_buttons.height
    anchors.top: syllabus_leftWindow_buttons.bottom
    clip: true
    color: 'red'
    ScrollBar {
      id: syllabus_leftWindow_vbar
      hoverEnabled: true
      active: hovered || pressed
      orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
      size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.bottom: parent.bottom
    }
    StackLayout {
      id: syllabus_stack_left
      width: parent.width
      height: parent.height
      currentIndex: syllabus_tabs.currentIndex
      //TAB:SYLLABUS_TAB:OBJECTIVES_LEFTWINDOW
      Rectangle {
        color: 'red'
        width: parent.width
        height: parent.height
        Layout.alignment: Qt.AlignHCenter
        Row {
          width: parent.width
          height: parent.height
          Rectangle {
            color: 'pink'
            width: parent.width
            height: parent.height
            Row {
              id: syllabus_objectives_leftWindow_buttons
              width: parent.width
              height: 25
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                color: 'lightblue'
                Text {
                  text: 'Add'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    objectives_model.insert(1,{objective : objective_text.text})
                    objective_text.text = 'String Field'
                  }
                }
              }
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                color: 'orange'
                Text {
                  text: 'Remove'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    objectives_model.remove(1)
                  }
                }
              }
            }
            Rectangle {
              color: 'lightgreen'
              width: parent.width
              height: 25
              anchors.top: syllabus_objectives_leftWindow_buttons.bottom
              TextInput {
                id: objective_text
                text: 'String Field (64 Characters)'
                maximumLength: 64
                cursorVisible: false
                anchors.centerIn: parent
              }
            }
            ListModel {
              id: objectives_model
              ListElement { objective: '' }
            }
            Component {
              id: objective_list_entry
              Item {
                width: parent.width
                height: 25
                Label {
                  text: objective
                } 
              }
            }
            ListView {
              id: objectives_list
              height: parent.height - syllabus_objectives_leftWindow_buttons.height
              model: objectives_model
              delegate: objective_list_entry
              anchors.top: syllabus_objectives_leftWindow_buttons.bottom
            }
          }
        }
      }
      //TAB:SYLLABUS_TAB:ASSESSMENTS_LEFTWINDOW
      Rectangle {
        color: 'red'
        width: parent.width
        height: parent.height
        Layout.alignment: Qt.AlignHCenter
        Row {
          width: parent.width
          height: parent.height
          Rectangle {
            color: 'pink'
            width: parent.width
            height: parent.height
            Row {
              id: syllabus_assessments_leftWindow_buttons
              width: parent.width
              height: 25
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                color: 'lightblue'
                Text {
                  text: 'Add'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    assessments_model.insert(1,{assessment : assessment_text.text})
                    assessment_text.text = 'String Field'
                  }
                }
              }
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                color: 'orange'
                Text {
                  text: 'Remove'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    assessments_model.remove(1)
                  }
                }
              }
            }
            Rectangle {
              color: 'lightgreen'
              width: parent.width
              height: 25
              anchors.top: syllabus_assessments_leftWindow_buttons.bottom
              TextInput {
                id: assessment_text
                text: 'String Field (64 Characters)'
                maximumLength: 64
                cursorVisible: false
                anchors.centerIn: parent
              }
            }
            ListModel {
              id: assessments_model
              ListElement { assessment: '' }
            }
            Component {
              id: assessment_list_entry
              Item {
                width: parent.width
                height: 25
                Label {
                  text: assessment
                } 
              }
            }
            ListView {
              id: assessments_list
              height: parent.height - syllabus_assessments_leftWindow_buttons.height
              model: assessments_model
              delegate: assessment_list_entry
              anchors.top: syllabus_assessments_leftWindow_buttons.bottom
            }
          }
        }
      }
      //TAB:SYLLABUS_TAB:INJURIES_LEFTWINDOW
      Rectangle {
        width: parent.width
        height: parent.height
        TabBar {
          id: syllabus_injuries_tabs
          width: parent.width
          Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
          TabButton {
            text: "Definitions"
            width: implicitWidth
          }
          TabButton {
            text: "Injury-Sets"
            width: implicitWidth
          }
        }
        Rectangle {
          width: parent.width
          height: parent.height - syllabus_injuries_tabs.height
          anchors.top: syllabus_injuries_tabs.bottom
          StackLayout {
            width: parent.width
            height: parent.height
            currentIndex:  syllabus_injuries_tabs.currentIndex
            Layout.alignment: Qt.AlignHCenter
            //TAB:SYLLABUS_TAB:INJURIES_TAB:DEFINITIONS_LEFTWINDOW
            Rectangle {
              color: 'red'
              height: parent.height
              width: parent.width
              Layout.alignment: Qt.AlignHCenter
              Row {
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                Rectangle {
                  color: 'pink'
                  width: parent.width
                  height: parent.height
                  Row {
                    id: syllabus_injuries_definition_leftWindow_buttons
                    width: parent.width
                    height: 25
                    Rectangle {
                      width: parent.width / 2 - 20
                      height: parent.height
                      anchors.left: parent.left
                      color: 'lightblue'
                      Text {
                        text: 'Add'
                        anchors.centerIn: parent
                      }
                      MouseArea {
                        anchors.fill: parent
                        onClicked: {
                          injuries_definition_model.insert(1,{injury_definition : injury_definition_text.text})
                          injury_definition_text.text = 'String Field'
                        }
                      }
                    }
                    Rectangle {
                      width: parent.width / 2 - 20
                      height: parent.height
                      anchors.right: parent.right
                      color: 'orange'
                      Text {
                        text: 'Remove'
                        anchors.centerIn: parent
                      }
                      MouseArea {
                        anchors.fill: parent
                        onClicked: {
                          injuries_definition_model.remove(1)
                        }
                      }
                    }
                  }
                  Rectangle {
                    color: 'lightgreen'
                    width: parent.width
                    height: 25
                    anchors.top: syllabus_injuries_definition_leftWindow_buttons.bottom
                    TextInput {
                      id: injury_definition_text
                      text: 'String Field (64 Characters)'
                      maximumLength: 64
                      cursorVisible: false
                      anchors.centerIn: parent
                    }
                  }
                  ListModel {
                    id: injuries_definition_model
                    ListElement { injury_definition: '' }
                  }
                  Component {
                    id: injury_definition_list_entry
                    Item {
                      width: parent.width
                      height: 25
                      Label {
                        text: injury_definition
                      } 
                    }
                  }
                  ListView {
                    id: injuries_definition_list
                    height: parent.height - syllabus_injuries_definition_leftWindow_buttons.height
                    model: injuries_definition_model
                    delegate: injury_definition_list_entry
                    anchors.top: syllabus_injuries_definition_leftWindow_buttons.bottom
                  }
                }
              }
            }
            Rectangle {
              //TAB:SYLLABUS_TAB:INJURIES_TAB:INJURY-SETS_LEFTWINDOW
              color: 'red'
              height: parent.height
              width: parent.width
              Row {
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                Rectangle {
                  color: 'pink'
                  width: parent.width
                  height: parent.height
                  Row {
                    id: syllabus_injuries_injurySet_leftWindow_buttons
                    width: parent.width
                    height: 25
                    Rectangle {
                      width: parent.width / 2 - 20
                      height: parent.height
                      anchors.left: parent.left
                      color: 'lightblue'
                      Text {
                        text: 'Add'
                        anchors.centerIn: parent
                      }
                      MouseArea {
                        anchors.fill: parent
                        onClicked: {
                          injuries_injurySet_model.insert(1,{injury_injurySet : injury_injurySet_text.text})
                          injury_injurySet_text.text = 'String Field'
                        }
                      }
                    }
                    Rectangle {
                      width: parent.width / 2 - 20
                      height: parent.height
                      anchors.right: parent.right
                      color: 'orange'
                      Text {
                        text: 'Remove'
                        anchors.centerIn: parent
                      }
                      MouseArea {
                        anchors.fill: parent
                        onClicked: {
                          injuries_injurySet_model.remove(1)
                        }
                      }
                    }
                  }
                  Rectangle {
                    color: 'lightgreen'
                    width: parent.width
                    height: 25
                    anchors.top: syllabus_injuries_injurySet_leftWindow_buttons.bottom
                    TextInput {
                      id: injury_injurySet_text
                      text: 'String Field (64 Characters)'
                      maximumLength: 64
                      cursorVisible: false
                      anchors.centerIn: parent
                    }
                  }
                  ListModel {
                    id: injuries_injurySet_model
                    ListElement { injury_injurySet: '' }
                  }
                  Component {
                    id: injury_injurySet_list_entry
                    Item {
                      width: parent.width
                      height: 25
                      Label {
                        text: injury_injurySet
                      } 
                    }
                  }
                  ListView {
                    id: injuries_injurySet_list
                    height: parent.height - syllabus_injuries_injurySet_leftWindow_buttons.height
                    model: injuries_injurySet_model
                    delegate: injury_injurySet_list_entry
                    anchors.top: syllabus_injuries_injurySet_leftWindow_buttons.bottom
                  }
                }
              }
            }
          }
        }
      }
      //TAB:SYLLABUS_TAB:TREATMENTS_LEFTWINDOW
      Rectangle {
        color: 'red'
        width: parent.width
        height: parent.height
        Row {
          width: parent.width
          height: parent.height
          anchors.top: parent.top
          Rectangle {
            color: 'pink'
            width: parent.width
            height: parent.height
            Row {
              id: syllabus_treatments_leftWindow_buttons
              width: parent.width
              height: 25
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                color: 'lightblue'
                Text {
                  text: 'Add'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    treatments_model.insert(1,{treatment : treatment_text.text})
                    treatment_text.text = 'String Field'
                  }
                }
              }
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                color: 'orange'
                Text {
                  text: 'Remove'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    treatments_model.remove(1)
                  }
                }
              }
            }
            Rectangle {
              color: 'lightgreen'
              width: parent.width
              height: 25
              anchors.top: syllabus_treatments_leftWindow_buttons.bottom
              TextInput {
                id: treatment_text
                text: 'String Field (64 Characters)'
                maximumLength: 64
                cursorVisible: false
                anchors.centerIn: parent
              }
            }
            ListModel {
              id: treatments_model
              ListElement { treatment: '' }
            }
            Component {
              id: treatment_list_entry
              Item {
                width: parent.width
                height: 25
                Label {
                  text: treatment
                } 
              }
            }
            ListView {
              id: treatments_list
              height: parent.height - syllabus_treatments_leftWindow_buttons.height
              model: treatments_model
              delegate: treatment_list_entry
              anchors.top: syllabus_treatments_leftWindow_buttons.bottom
            }
          }
        }
      }
      //TAB:SYLLABUS_TAB:EQUIPMENT_LEFTWINDOW
      Rectangle {
        color: 'red'
        width: parent.width
        height: parent.height
        Row {
          width: parent.width
          height: parent.height
          anchors.top: parent.top
          Rectangle {
            color: 'pink'
            width: parent.width
            height: parent.height
            Row {
              id: syllabus_equipment_leftWindow_buttons
              width: parent.width
              height: 25
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.left: parent.left
                color: 'lightblue'
                Text {
                  text: 'Add'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    equipment_model.insert(1,{equipment : equipment_text.text})
                    equipment_text.text = 'String Field'
                  }
                }
              }
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                color: 'orange'
                Text {
                  text: 'Remove'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    equipment_model.remove(1)
                  }
                }
              }
            }
            Rectangle {
              color: 'lightgreen'
              width: parent.width
              height: 25
              anchors.top: syllabus_equipment_leftWindow_buttons.bottom
              TextInput {
                id: equipment_text
                text: 'String Field (64 Characters)'
                maximumLength: 64
                cursorVisible: false
                anchors.centerIn: parent
              }
            }
            ListModel {
              id: equipment_model
              ListElement { equipment: '' }
            }
            Component {
              id: equipment_list_entry
              Item {
                width: parent.width
                height: 25
                Label {
                  text: equipment
                } 
              }
            }
            ListView {
              id: equipment_list
              height: parent.height - syllabus_equipment_leftWindow_buttons.height
              model: equipment_model
              delegate: equipment_list_entry
              anchors.top: syllabus_equipment_leftWindow_buttons.bottom
            }
          }
        }
      }
    }
  }
}