import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

RowLayout {
  anchors.fill: parent
  anchors.margins: 5
  spacing: 5

  //TAB:SUMMARY_LEFTWINDOW 
  Rectangle {
    id: summary_leftWindow
    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
    clip: true

    Row {
      id: summary_leftWindow_title
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_leftWindow_vbar.width
      height: parent.height / 10
      y: (-summary_leftWindow_vbar.position * height)
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Title:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: title
          text: 'String Field (128 Characters)'
          maximumLength: 128
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_leftWindow_version
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      anchors.top: summary_leftWindow_title.bottom
      width: parent.width - summary_leftWindow_vbar.width
      height: parent.height / 10
      y: (-summary_leftWindow_vbar.position * height)
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Version:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
         id: version
          text: 'String Field (128 Characters)'
          maximumLength: 128
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_leftWindow_security
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      anchors.top: summary_leftWindow_version.bottom
      width: parent.width - summary_leftWindow_vbar.width
      height: parent.height / 10
      y: (-summary_leftWindow_vbar.position * height)
          Rectangle {
            width: parent.width / 5 - 10
            height: 25
            color: 'orange'
            Text {
              text: 'Security:'
              anchors.centerIn: parent
            }
          }
          Rectangle {
            color: 'pink'
             width: parent.width * (4 / 5) - 10
            height: 25                    
          ComboBox {  
            id: control
            width: parent.width
            height: 25
           currentIndex: 0
           model: ["Unclassified","Classified","Secret","Top Secret"]
           background: Rectangle {
             color: 'pink'
             width: parent.width
             height: 25
             border.width: 0
             border.color: 'transparent'
             radius: 0
           }
            contentItem: Text {
                leftPadding: 0
                rightPadding: control.indicator.width + control.spacing
                text: control.displayText
                font: titleCard.font
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
          }
        }
    }
    Row {
      id: summary_leftWindow_purpose
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      anchors.top: summary_leftWindow_security.bottom
      width: parent.width - summary_leftWindow_vbar.width
      height: parent.height / 3
      y: (-summary_leftWindow_vbar.position * height)
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Purpose:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        color: 'pink'
        width: parent.width * (4 / 5) - 10
        height: parent.height
        Row {
          width: parent.width
          height: parent.height
          TextEdit {
            //TODO: Make this scrollable
            width: parent.width - 20
            height: parent.height
            anchors.margins: 5
            //focus: true
          }
          Rectangle {
            color: 'green'
            width: 20
            height: parent.height
            Text {
              text: 'Expand'
              anchors.centerIn: parent
              rotation: 90
            }
          }
        }
      }
      }
      Row {
        id: summary_leftWindow_restrictions
        spacing: 10
        leftPadding: 5
        rightPadding: 5
        topPadding: 20
        anchors.top: summary_leftWindow_purpose.bottom
        width: parent.width - summary_leftWindow_vbar.width
        height: parent.height / 3  
        y: (-summary_leftWindow_vbar.position * height)
        Rectangle {
          width: parent.width / 5 - 10
          height: 25
          color: 'orange'
          Text {
            text: 'Restrictions:'
            anchors.centerIn: parent
          }
        }
        Row {
          width: parent.width * (4 / 5) - 10
          height: parent.height
          Rectangle {
            color: 'pink'
            width: parent.width - 20
            height: parent.height
            Row {
              id: restrictions_buttons
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
                    restrictions_model.insert(1,{restriction : restriction_text.text})
                    restriction_text.text = 'String Field'
                  }
                }
              }
              Rectangle {
                width: parent.width / 2 - 20
                height: parent.height
                anchors.right: parent.right
                color: 'plum'
                Text {
                  text: 'Remove'
                  anchors.centerIn: parent
                }
                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    restrictions_model.remove(1)
                  }
              }
            }
          }
          Rectangle {
            color: 'lightgreen'
            width: parent.width
            height: 25
            anchors.top: restrictions_buttons.bottom
            TextInput {
              id: restriction_text
              text: 'String Field (64 Characters)'
              maximumLength: 64
              cursorVisible: false
              anchors.centerIn: parent
            }
          }
          ListModel {
            id: restrictions_model
            ListElement { restriction: '' }
          }
          Component {
            id: restriction_list_entry
            Item {
              width: parent.width
              height: 25
              Label {
                text: restriction
              } 
            }
          }
          ListView {
            id: restrictions_list
            height: parent.height - restrictions_buttons.height
            model: restrictions_model
            delegate: restriction_list_entry
            anchors.top: restrictions_buttons.bottom
          }
        }
        Rectangle {
          color: 'green'
          width: 20
          height: parent.height
          Text {
            text: 'Expand'
            anchors.centerIn: parent
            rotation: 90
          }
        }
      }        
    }
    ScrollBar {
      id: summary_leftWindow_vbar
      hoverEnabled: true
      active: hovered || pressed
      orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
      size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.bottom: parent.bottom
    }
  }
  //TAB:SUMMARY_RIGHTWINDOW
  Rectangle {
    id: summary_rightWindow
    clip: true
    Layout.fillWidth: true
    Layout.fillHeight: true
    Row {
      id: summary_rightWindow_author
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Author:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: author
          text: 'String Field (128 Characters)'
          maximumLength: 128
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_org
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      anchors.top: summary_rightWindow_author.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Org:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: org
          text: 'String Field (64 Characters)'
          maximumLength: 64
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_phoneNumber
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      anchors.top: summary_rightWindow_org.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Phone#:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: phoneNumber
          text: 'String Field (12 Characters)'
          maximumLength: 12
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_email
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      anchors.top: summary_rightWindow_phoneNumber.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Email:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: email
          text: 'String Field (64 Characters)'
          maximumLength: 64
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_keyword
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      anchors.top: summary_rightWindow_email.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Keyword:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: keyword
          text: 'String Field (64 Characters)'
          maximumLength: 64
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_applicationDomain
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      topPadding: 5
      width: parent.width  - summary_rightWindow_vbar.width
      height: parent.height / 10
      y: (-summary_rightWindow_vbar.position * height)
      anchors.top: summary_rightWindow_keyword.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Application Domain:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        width: parent.width * (4 / 5) - 10
        height: 25
        color: 'pink'
        TextInput {
          id: applicationDomain
          text: 'String Field (64 Characters)'
          maximumLength: 64
          cursorVisible: false
          anchors.centerIn: parent
        }
      }
    }
    Row {
      id: summary_rightWindow_useLimitations
      spacing: 10
      leftPadding: 5
      rightPadding: 5
      width: parent.width - summary_leftWindow_vbar.width
      height: parent.height / 3
      y: (-summary_leftWindow_vbar.position * height)
      anchors.top: summary_rightWindow_applicationDomain.bottom
      Rectangle {
        width: parent.width / 5 - 10
        height: 25
        color: 'orange'
        Text {
          text: 'Use Limitations:'
          anchors.centerIn: parent
        }
      }
      Rectangle {
        color: 'pink'
        width: parent.width * (4 / 5) - 10
        height: parent.height
        Row {
          width: parent.width
          height: parent.height
          TextEdit { //TODO: Make this scrollable
            width: parent.width - 20
            height: parent.height
            anchors.margins: 5
            //focus: true
          }
          Rectangle {
            color: 'green'
            width: 20
            height: parent.height
            Text {
              text: 'Expand'
              anchors.centerIn: parent
              rotation: 90
            }
          }
        }
      }
    }
    ScrollBar {
      id: summary_rightWindow_vbar
      hoverEnabled: true
      active: hovered || pressed
      orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
      size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.bottom: parent.bottom
    }
  }
}

