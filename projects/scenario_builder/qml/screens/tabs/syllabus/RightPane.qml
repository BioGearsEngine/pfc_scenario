import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.4
import QtQuick.Controls 2.12

import "../../common"
import "right"

import com.ara.pfc.ScenarioModel.SQL 1.0

Flickable {
  id: root  
  property alias currentIndex    : stack.currentIndex

  property alias objectiveModel  : objectives.model
  property alias assessmentModel : assessments.model
  property alias injuryModel     : injuries.model
  property alias injurySetModel  : injurySets.model
  property alias treatmentModel  : treatments.model

  property alias objectiveIndex  : objectives.index
  property alias assessmentIndex : assessments.index
  property alias injuryIndex     : injuries.index
  property alias injurySetIndex  : injurySets.index
  property alias treatmentIndex  : treatments.index

  property alias currentEquipment  : equipment.currentEquipment

  Layout.fillHeight : true
  Layout.fillWidth : true
  property SQLBackend backend
  property int topIndex
  property int syllabusIndex
  ScrollBar.vertical: ScrollBar { }
  
  StackLayout {
      id : stack
      anchors.fill : parent
      anchors.bottomMargin : 5
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1
        TabObjectives{
          id : objectives
          backend : root.backend
          width: parent.width
          height: parent.height
          topIndex : root.topIndex
        }
      }
      Rectangle {
        id: injuriesPane
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1
        ScrollView {
          anchors.fill : parent
          clip : true
          TabTraumas {

            width : Math.max(injuriesPane.width, implicitWidth)
            height :Math.max(injuriesPane.height, implicitHeight)
            id : injuries
            backend : root.backend

          }
        }
      }
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1
        TabTraumaSets {
          id : injurySets
          backend : root.backend
          width: parent.width
          height: parent.height
          syllabusIndex : root.syllabusIndex
          topIndex: root.topIndex
        }
      }
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1
        TabTreatments {
          id : treatments
          backend : root.backend
          width: parent.width
          height: parent.height
          syllabusIndex : root.syllabusIndex
          topIndex: root.topIndex
        }
      }
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1      
        TabEquipment {
          id : equipment
          backend : root.backend
          width: parent.width
          height: parent.height
          topIndex : root.topIndex
        }
      }
      Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.rightMargin: 5
        border.color: 'black'
        border.width: 1
        TabAssessments{
          id : assessments
          backend : root.backend
          width: parent.width
          //height: parent.height
        }
      }
  }
}
