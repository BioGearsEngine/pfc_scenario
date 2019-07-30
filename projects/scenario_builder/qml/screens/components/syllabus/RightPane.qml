import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Flickable {
  id: root  
  property alias currentIndex : stack.currentIndex
  property SQLBackend backend

  ScrollBar.vertical: ScrollBar { }
  
  StackLayout {
      id : stack
      anchors.fill : parent
      anchors.bottomMargin : 5

      RP_Objectives{
        Layout.fillHeight: true
        Layout.fillWidth: true

      }

      RP_Assessments{
        Layout.fillHeight: true
        Layout.fillWidth: true
       }

      RP_Injuries {
        Layout.fillHeight: true
        Layout.fillWidth: true
       }

      RP_Injuries {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }

      RP_Treatments {
        Layout.fillHeight: true
        Layout.fillWidth: true
       }

      RP_Equipment {
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
  }
}
