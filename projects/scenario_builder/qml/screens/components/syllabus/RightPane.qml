import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Flickable {
  id: root  
  property alias currentIndex : stack.currentIndex
  property alias objectiveModel  : objectives.model
  property alias currentAssessment : assessments.model
  property alias currentInjury     : injuries.model
  property alias currentInjurySet  : injurySets.model
  property alias currentTreatment  : treatments.model
  property alias currentEquipment  : equipment.model

  property alias objectiveIndex : objectives.index
  
  property SQLBackend backend
  ScrollBar.vertical: ScrollBar { }
  
  StackLayout {
      id : stack
      anchors.fill : parent
      anchors.bottomMargin : 5

      RP_Objectives{
        id : objectives
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Assessments{
        id : assessments
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Injuries {
        id : injuries
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Injuries {
        id : injurySets
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Treatments {
        id : treatments
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
      RP_Equipment {
        id : equipment
        Layout.fillHeight: true
        Layout.fillWidth: true
      }
  }
}
