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
  columns : 2

  //TAB:SUMMARY_LEFTWINDOW
  LeftPane{
    id : summary_left
    backend : root.backend

    Layout.preferredWidth: parent.width / 2
    Layout.fillHeight: true
  }

  //TAB:SUMMARY_RIGHTWINDOW
  RightPane{
    id : summary_right
    backend : root.backend

    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.topMargin : 58
  }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
