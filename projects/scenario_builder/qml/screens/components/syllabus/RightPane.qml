import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

StackLayout {
    id: root
    property alias currentIndex : root.currentIndex
    property SQLBackend backend

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

    RP_Treatments {
      Layout.fillHeight: true
      Layout.fillWidth: true
     }

    RP_Equipment {
      Layout.fillHeight: true
      Layout.fillWidth: true
    }
}
