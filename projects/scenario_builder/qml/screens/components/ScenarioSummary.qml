import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "summary"
import "common"

import com.ara.pfc.ScenarioModel.SQL 1.0

GridLayout {
  id: root
  property SQLBackend backend
  property string scenarioTitle
  columns : 2

  //TAB:SUMMARY_LEFTWINDOW
  LeftPane{
    id : summary_left
    backend : root.backend
    scenarioTitle : root.scenarioTitle
    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
    Layout.leftMargin: 5
    border.color: 'black'
    border.width: 1
    onScenarioTitleChanged : {
      root.scenarioTitle = scenarioTitle
    }
  }

  //TAB:SUMMARY_RIGHTWINDOW
  RightPane{
    id : summary_right
    backend : root.backend
    border.color: 'black'
    border.width: 1
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.rightMargin: 5
  }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
