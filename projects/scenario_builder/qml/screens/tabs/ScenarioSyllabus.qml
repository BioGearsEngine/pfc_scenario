import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "syllabus"
import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

GridLayout {
  id : root
  property SQLBackend backend
  property int topIndex


  width : parent.width
  height : parent.height
  columns : 2

  LeftPane {
    id : syllabus_leftPane

    Layout.leftMargin : 5
    Layout.bottomMargin : 5

    backend : root.backend
    topIndex : root.topIndex
    Layout.preferredWidth : parent.width / 2
    Layout.fillHeight : true
  }

  RightPane {
    id : syllabus_rightPane

    Layout.rightMargin : 5
    Layout.bottomMargin : 5

    backend : root.backend
    topIndex : root.topIndex
    currentIndex : syllabus_leftPane.currentIndex
    currentAssessment : syllabus_leftPane.currentAssessment
    currentEquipment : syllabus_leftPane.currentEquipment
    currentObjective : syllabus_leftPane.currentObjective
    currentTreatment : syllabus_leftPane.currentTreatment
    currentTrauma : syllabus_leftPane.currentTrauma
    currentProfile : syllabus_leftPane.currentProfile

    Component.onCompleted : {}
  }
}
