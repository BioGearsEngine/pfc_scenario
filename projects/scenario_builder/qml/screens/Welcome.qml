import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import com.ara.pfc.ScenarioModel.SQL 1.0 as PFC
import com.ara.pfc.ScenarioModel.XML 1.0

Page {
  id : root
  property PFC.SQLBackend backend
  signal loadClicked()
  signal createClicked()
  signal listUpdated()
  signal recentClicked(string path)

  onListUpdated : {
    recentFileList.clear();
    var recent_files = backend.get_recent_scenario_files();
    var i = 0;
    for (i = 0; i < 10; i++) {
      if (recent_files[i] != "") {
        recentFileList.append({'path': recent_files[i], 'name': 'NAME', 'last': 'LAST'})
      }
    }
  }

  Rectangle {
    id : mainArea
    color : "transparent"
    border.color : "#34495e"
    width : parent.width * .75
    height : parent.height * .90
    anchors.centerIn : parent

    ColumnLayout {
      id : mainColumnLayout
      anchors.fill : parent
      anchors.centerIn : parent
      anchors.margins : 10
      spacing : 15

	  Image {
		  id : logoBackground
          Layout.alignment : Qt.AlignTop | Qt.AlignHCenter
		  opacity : 0.85
		  //anchors.verticalCenter : parent.verticalCenter
          //anchors.right : parent.left
          width : 500
          fillMode : Image.PreserveAspectFit 
          clip : false
          source : 'qrc:/img/logo.png'
        
		Text {
			id : sustainText
			anchors.horizontalCenter : logoBackground.horizontalCenter
			anchors.bottom : scenarioBuilderText.top
			color : "#34495e"
			text : "Sustain"
			font.italic : true
			font.bold : true
			font.underline : false
			font.pointSize : 24

			
		  }

		  Text {
			id : scenarioBuilderText
			anchors.centerIn : logoBackground
			//anchors.top : sustainText.bottom
			color : "#34495e"
			text : "Scenario Builder"
			font.italic : true
			font.bold : true
			font.underline : true
			font.pointSize : 24
		  }
	  }

      Rectangle {
        id : getting_started_rect
        Layout.alignment : Qt.AlignTop | Qt.AlignHCenter
        Layout.preferredHeight : childrenRect.height
        Layout.preferredWidth : 600
        color : "transparent"

        Label {
          id : getting_started_text
          text : "Getting Started:"
          font.pointSize : 12
          font.bold : true
          color : 'steelblue'

        }

        Button {
          id : openButton
          text : "Open"
          anchors.right : newButton.left
          anchors.rightMargin : 6

          onClicked : {
            root.loadClicked()
          }
        }
        Button {
          id : newButton
          text : "New Scenario"
          anchors.right : parent.right

          onClicked : {
            root.createClicked()
          }
        }
        Label {
          text : "Recent:"
          anchors.left : getting_started_text.left
          anchors.top : newButton.bottom
          anchors.topMargin : 10
          font.pointSize : 12
          font.bold : true
          color : 'steelblue'
        }
      }


      ListView {
        id : listArea
        Layout.alignment : Qt.AlignTop | Qt.AlignHCenter
        Layout.minimumHeight : 100
        Layout.minimumWidth : 600
        Layout.preferredHeight : childrenRect.height
        Layout.leftMargin : 20
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true


        model : ListModel {
          id : recentFileList
          Component.onCompleted : {
            root.listUpdated()
          }
        }

        delegate : MouseArea {
          anchors.left : parent.left
          anchors.right : parent.right
          RowLayout {
            spacing : 5
            Label {
              text : model.index + 1
            }
            Text {
              text :  model.path.replace(/file:\/\/(\/(\w:)){0,1}/i, '$2');
              color : (index == listArea.currentIndex) ? Material.accentColor : Material.secondaryTextColor
            }
          }
          width : childrenRect.width
          height : childrenRect.height
          onClicked : {
            listArea.currentIndex = index
          }
          onDoubleClicked : {
            listArea.currentIndex = index;
            recentClicked(recentFileList.get(listArea.currentIndex).path)
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }

      Rectangle {
        Layout.alignment : Qt.AlignHCenter
        Layout.minimumWidth : 600
        Layout.minimumHeight : childrenRect.height
        color : 'transparent'
        Button {
          anchors.right : parent.right
          text : "Load"
          onClicked : {
            root.loadClicked()
          }
        }
      }
    }
  }
}
