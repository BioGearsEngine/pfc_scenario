import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../../common"
import "right"

import com.ara.pfc.ScenarioModel.SQL 1.0

Flickable {
  id : root
  property alias currentIndex : stack.currentIndex

  property alias currentTreatment : treatments.currentTreatment
  property alias currentTrauma : traumas.currentTrauma
  property alias currentProfile : traumaProfiles.currentProfile
  property alias currentObjective : objectives.currentObjective
  property Equipment currentEquipment 
  property alias currentAssessment : assessments.currentAssessment

  Layout.fillHeight : true
  Layout.fillWidth : true
  property SQLBackend backend
  property int topIndex
  property int syllabusIndex
  ScrollBar.vertical : ScrollBar {}

  StackLayout {
    id : stack
    anchors.fill : parent

    Rectangle {

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1
      TabObjectives {
        id : objectives
        backend : root.backend
        width : parent.width
        height : parent.height
        topIndex : root.topIndex
      }
    }
    Rectangle {
      id : traumaPane

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1
      TabTraumas {
        id : traumas
        backend : root.backend
        width : parent.width
        height : parent.height
        topIndex : root.topIndex
      }
    }
    Rectangle {

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1
      TabTraumaProfiles {
        id : traumaProfiles
        backend : root.backend
        width : parent.width
        height : parent.height

        topIndex : root.topIndex
      }
    }
    Rectangle {

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1
      TabTreatments {
        id : treatments
        backend : root.backend
        width : parent.width
        height : parent.height

        topIndex : root.topIndex
      }
    }
    Rectangle {

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1


      TabEquipment {
        id : equipment
        backend : root.backend
        width : parent.width
        height : parent.height
        topIndex : root.topIndex
        currentEquipment : root.currentEquipment
      }
    }
    Rectangle {

      Layout.fillWidth : true
      Layout.rightMargin : 5
      border.color : 'black'
      border.width : 1
      TabAssessments {
        id : assessments
        backend : root.backend
        width : parent.width
        // height: parent.height
      }
    }
  }
}
