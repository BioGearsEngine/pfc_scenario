import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "syllabus"
import "common"

import com.ara.pfc.ScenarioModel.SQL 1.0

GridLayout {
  id: root
  property SQLBackend backend
  property int topIndex

  width: parent.width
  height: parent.height
  columns: 2
  
  LeftPane{
    id: syllabus_leftPane
    backend : root.backend
    topIndex : root.topIndex
    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
  }

  RightPane{
    id: syllabus_rightPane
    backend : root.backend

    currentIndex : syllabus_leftPane.currentIndex
    
    objectiveModel  : syllabus_leftPane.objectiveModel
    assessmentModel : syllabus_leftPane.assessmentModel
    injuryModel     : syllabus_leftPane.injuryModel
    injurySetModel  : syllabus_leftPane.injurySetModel
    treatmentModel  : syllabus_leftPane.treatmentModel
    equipmentModel  : syllabus_leftPane.equipmentModel

    objectiveIndex  : syllabus_leftPane.objectiveIndex
    assessmentIndex : syllabus_leftPane.assessmentIndex
    injuryIndex     : syllabus_leftPane.injuryIndex
    injurySetIndex  : syllabus_leftPane.injurySetIndex
    treatmentIndex  : syllabus_leftPane.treatmentIndex
    equipmentIndex  : syllabus_leftPane.equipmentIndex

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.topMargin : 58
    Layout.rightMargin: 5


    Component.onCompleted: {
      
    }
  }
}
