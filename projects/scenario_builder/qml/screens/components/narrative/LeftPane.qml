import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id: root
  property SQLBackend backend
  readonly property alias model : listArea.model 
  readonly property alias index : listArea.currentIndex
  readonly property alias count : listArea.count
  Scene {
    id : self
  }
  Location {
    id : location
  }
  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight: true
    Layout.margins : 5

    border.color : "black"

    TwoButtonRow {
      id: controls
      anchors.top : listRectangle.top
      anchors.left : listRectangle.left
      anchors.right : listRectangle.right
      anchors.topMargin : 2
      anchors.rightMargin  : 5
      anchors.leftMargin  : 5

      property int next : 1

      firstButtonText : "Add"
      secondButtonText : "Remove"

      onFirstButtonClicked :{
        if( next < root.model.count ) 
        { next = root.model.count +1}
        self.scene_id = -1
        self.name = "New Scene %1".arg(next)

        while( root.backend.select_scene(self) )
        { 
         next = next +1
         self.scene_id = -1; 
         self.name = "New Scene %1".arg(next);
         self.description = "Scene Description";
         self.details = "Scene Details";
        } 
        root.backend.update_scene(self)
        root.model.insert(root.model.count,
          {
           "id" : self.scene_id,
           "name": "%1".arg(self.name),
           "description" : self.description,
           "details" : self.details}
        );
//        location.location_id = -1
//        location.name = "New Scene %1 Location".arg(next)
//        location.scene_name = "New Scene %1".arg(next)
//        location.time_of_day = "";
//        location.environment = "";
//        root.backend.update_location(location)
        ++next;
      }
      onSecondButtonClicked : {
        if (listArea.model.count == 0) {
          return
        }
        self.scene_id = -1
        self.name = root.model.get(root.index).name

        root.backend.remove_scene(self)
        root.model.remove(root.index)
        listArea.currentIndex = Math.max(0,root.index-1)
      }
    }

    ListView {
      id : listArea
      anchors { top : controls.bottom ; bottom : parent.bottom; 
                   left : parent.left ; right : parent.right }  
      spacing : 5
      clip: true
      highlightFollowsCurrentItem : true
      highlightMoveDuration : 1
      highlight: Rectangle {
          color: '#1111110F'
          Layout.alignment: Qt.AlignTop
          Layout.fillWidth: true
          Layout.margins : 5
      }

      model : ListModel {}

      delegate : Rectangle {
        id : scene
        color : 'transparent'
        border.color: "steelblue"
        height : 30
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            listArea.currentIndex = index

          }
        }
         Rectangle {
           id : scene_title_text
           anchors.left : scene.left
           anchors.leftMargin : 5
           width : 150
           Text  {
            text :  model.name 
            font.weight: Font.Bold
            font.pointSize: 10
           }
           enabled : false
           color: enabled ? Material.primaryTextColor : Material.primaryTextColor
        }
        onFocusChanged: {
          if(listArea.currentIndex == index){
            state = 'Selected';
          }
          else{
            state = '';
          }
        }
      }

      ScrollBar.vertical: ScrollBar { }

      Component.onCompleted : {
        var r_count = backend.scene_count();
        root.backend.scenes()
        while ( root.backend.next_scene(self) ){

          listArea.model.insert(listArea.model.count,
            {
             id  : self.scene_id,
             name: "%1".arg(self.name), 
             description : self.description,
             details : self.details
            });
        }
      }
    }
  }
}