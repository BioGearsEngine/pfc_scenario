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
  property ListModel model
  property int index // = events.index

  Event {
    id : self
  }

  function update_event(values) {
    obj.event_id = values.id
    obj.name = values.name
    obj.location = values.location
    obj.actor = values.actor
    obj.equipment = values.equipment
    obj.description = values.description
    obj.fk_scene = values.fk_scene
    root.backend.update_event(obj)
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
      anchors.rightMargin : 5
      anchors.leftMargin : 5

      property int next : 1

      firstButtonText : "New"
      fourthButtonText : "Remove"

      secondButtonText : "Move Up"
      thirdButtonText : "Move Down"

      onFirstButtonClicked :{
        if( next < listArea.model.count ) 
        { next = listArea.model.count +1}
        self.event_id = -1
        self.name = "New Event %1".arg(next)
        self.location = -1
        self.actor = -1
        self.equipment = "New Equipment %1".arg(next)
        self.description = "Description of Event %1".arg(next)
        console.log(JSON.stringify(root.model.get(root.index)))
        self.fk_scene = root.model.get(root.index).id
        while( root.backend.select_event(self) )
        { 
         next = next +1
         self.event_id = -1; 
         self.name = "New Event %1".arg(next);
         self.location = -1
         self.actor = -1
         self.equipment = "New Equipment %1".arg(next)
         self.description = "Description of Event %1".arg(next)
         self.fk_scene = root.model.get(root.index).id
        } 

        root.backend.update_event(self)
        listArea.model.insert(listArea.model.count,
          {
           "id" : self.event_id,
           "name": "%1".arg(self.name), 
           "location" : self.location,
           "actor" : self.actor,
           "equipment" : "%1".arg(self.equipment),
           "description": "%1".arg(self.description),
           "fk_scene": self.fk_scene}
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
        self.name = listArea.model.get(listArea.currentIndex).name
        root.backend.remove_event(self)
        listArea.model.remove(listArea.currentIndex)
        listArea.currentIndex = Math.max(0,listArea.currentIndex-1)
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
             fk_scene : self.fk_scene
            });
        }
      }
    }
  }
  onIndexChanged : {
    var values = model.get(index)
    if(values) {
      listArea.model.clear()
      root.backend.events() 
      while ( root.backend.next_event(self) ) {
        if ( self.fk_scene == root.model.get(root.index).id ) {
          listArea.model.insert(listArea.model.count,
          {
            "event_id" : self.event_id,
            "name" : "%1".arg(self.name),
            "location" : self.location,
            "actor" : self.actor,
            "equipment" : "%1".arg(self.equipment),
            "description" : "%1".arg(self.description),
            "fk_scene" : self.fk_scene
            });
        }
      }
    }
  }
}