import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Flickable {
  id: root  
  property alias currentIndex    : stack.currentIndex

  property alias objectiveModel  : objectives.model
  property alias assessmentModel : assessments.model
  property alias injuryModel     : injuries.model
  property alias injurySetModel  : injurySets.model
  property alias treatmentModel  : treatments.model
  property alias equipmentModel  : equipment.model

  property alias objectiveIndex  : objectives.index
  property alias assessmentIndex : assessments.index
  property alias injuryIndex     : injuries.index
  property alias injurySetIndex  : injurySets.index
  property alias treatmentIndex  : treatments.index
  property alias equipmentIndex  : equipment.index

  property SQLBackend backend
  ScrollBar.vertical: ScrollBar { }
  
  StackLayout {
      id : stack
      anchors.fill : parent
      anchors.bottomMargin : 5

      RP_Objectives{
        id : objectives
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Assessments{
        id : assessments
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Injuries {
        id : injuries
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_InjurySets {
        id : injurySets
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Treatments {
        id : treatments
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Equipment {
        id : equipment
        backend : root.backend
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
  }
}
