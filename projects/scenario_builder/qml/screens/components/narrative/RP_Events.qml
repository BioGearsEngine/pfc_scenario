import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id: root
  property SQLBackend backend
  readonly property alias model : listArea.model 
  readonly property alias index : listArea.currentIndex

  Event {
    id : self
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
        self.event_id = -1
        self.name = "New Event %1".arg(next)
        self.description = "Description of Event %1".arg(next)
        self.citations = new Array()

        while( root.backend.select_event(self) )
        { 
         next = next +1
         self.event_id = -1; 
         self.name = "New Event %1".arg(next);
         self.description = "Description of Event %1".arg(next)
        } 

        root.backend.update_event(self)
        root.model.insert(root.model.count,
          {
           "id" : self.event_id,
           "name": "%1".arg(self.name), 
           "name": "%1".arg(self.name), 
           "description": "%1".arg(self.description) , 
           "citations": self.citations}
        );
        ++next;
      }
      onSecondButtonClicked :{
        console.log("Reordering Currently Unsupported!")
      }
      onThirdButtonClicked : {
        console.log("Reordering Currently Unsupported!")
      }
      onFourthButtonClicked : {
        self.event_id = -1
        self.name = root.model.get(root.index).name

        root.backend.remove_event(self)
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
        id : event
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

        Text {
          id : event_title_text
          anchors.left : event.left
          anchors.leftMargin : 5
          text :  model.name
          width : 150
          font.weight: Font.Bold
          font.pointSize: 10
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
        }

        Text {
          id : event_value_text
          anchors.left : event_title_text.right
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize: 10
          text :  description
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
          elide: Text.ElideRight
        }

        states: State {
          name : "Selected"
          PropertyChanges{ target : event_title_text; enabled : true}
          PropertyChanges{ target : event_value_text; enabled  : true}
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
        var r_count = backend.event_count();
        root.backend.events()
        while ( root.backend.next_event(self) ){

          listArea.model.insert(listArea.model.count,
            {
             id  : self.event_id,
             name: "%1".arg(self.name), 
             description: "%1".arg(self.description),
             citations : self.citations 
            });
        }
      }
    }
  }
}