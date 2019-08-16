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

    FourButtonRow {
      id: controls
      anchors.top : listRectangle.top
      anchors.left : listRectangle.left
      anchors.right : listRectangle.right
      anchors.topMargin : 2
      anchors.rightMargin  : 5
      anchors.leftMargin  : 5

      property int next : 1

      firstButtonText : "Add"
      fourthButtonText : "Remove"

      secondButtonText : "Move Up"
      thirdButtonText : "Move Down"

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
        } 

        root.backend.update_scene(self)
        root.model.insert(root.model.count,
          {
           "id" : self.scene_id,
           "name": "%1".arg(self.name)}
        );
        location.fk_scene = self.scene_id
        location.location_id = -1
        location.name = "New Scene %1 Location".arg(next)
        location.scene_name = "New Scene %1".arg(next)
        root.backend.update_location(location)

        ++next;
      }
      onSecondButtonClicked :{
        console.log("Reordering Currently Unsupported!")
      }
      onThirdButtonClicked : {
        console.log("Reordering Currently Unsupported!")
      }
      onFourthButtonClicked : {
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

      highlight: Rectangle {
          color: '#1111110F'
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.margins: 5
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
         TextField {
           id : scene_title_text
           anchors.left : scene.left
           anchors.leftMargin : 5
           text :  model.name 
           width : 150
           font.weight: Font.Bold
           font.pointSize: 10
           readOnly : true
           activeFocusOnPress: false
           hoverEnabled : false
           enabled : false
           color: enabled ? Material.primaryTextColor : Material.primaryTextColor
           onEditingFinished : {
             self.scene_id = model.id
             self.name = text
             root.backend.update_scene(self)
             model.id = self.scene_id
          }
        }

        states: State {
          name : "Selected"
                PropertyChanges{ target : scene_title_text; readOnly : false}
                PropertyChanges{ target : scene_title_text; activeFocusOnPress : true}
                PropertyChanges{ target : scene_title_text; hoverEnabled : true}
                PropertyChanges{ target : scene_title_text; enabled : true}
                PropertyChanges{ target : scene_title_text; mouseSelectionMode  : TextInput.SelectCharacters }
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
            });
        }
      }
    }
  }
}