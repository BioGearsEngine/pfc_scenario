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
  Scene {
    id : self_scene
  }

  function update_event(values) {
    obj.event_id = values.id
    obj.name = values.name
    obj.location = values.location
    obj.actor = values.actor
    obj.equipment = values.equipment
    obj.description = values.description
    root.backend.update_event(obj)
  }
  StackLayout {
    id: event_stack
    currentIndex: 1
    Rectangle {
      id: full_listRectangle
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 5
      color: "transparent"
      border.color : "black"

      FourButtonRow {
        id: full_controls
        anchors.top : full_listRectangle.top
        anchors.left : full_listRectangle.left
        anchors.right : full_listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5  

        property int next : 1  

        firstButtonText : "Add"
        secondButtonText : "New"
        thirdButtonText : "Remove"  
        fourthButtonText : "Exit"  

        onFirstButtonClicked :{
          if ( next < full_listArea.count )
          { next = full_listArea.model.count + 1 }
          self.event_id = -1
          self.name = full_listArea.model.get(full_listArea.currentIndex).name
          self.location = full_listArea.model.get(full_listArea.currentIndex).location
          self.actor = full_listArea.model.get(full_listArea.currentIndex).actor
          self.equipment = full_listArea.model.get(full_listArea.currentIndex).equipment
          self.description = full_listArea.model.get(full_listArea.currentIndex).description
          console.log(JSON.stringify(root.model.get(root.index)))
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          root.backend.update_event_in_scene(self_scene,self)
          event_stack.currentIndex = 1
          console.log("index changed----------")
          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            console.log(JSON.stringify(self_scene))
            root.backend.events_in_scene(self_scene)
            while ( root.backend.next_event(self) ) {
              console.log(JSON.stringify(self))
              listArea.model.insert(listArea.model.count,
              {
             "id" : self.event_id,
             "name": "%1".arg(self.name), 
             "location" : self.location,
             "actor" : self.actor,
             "equipment" : "%1".arg(self.equipment),
             "description": "%1".arg(self.description)
              });
            }
          }
        }
        onSecondButtonClicked :{
          if ( next < full_listArea.count )
          { next = full_listArea.model.count + 1 }
          self.event_id = -1
          self.name = "New Event %1".arg(next)
          self.description = "Description of Event %1".arg(next)
          self.location = -1
          self.actor = -1
          self.equipment = "New Equipment %1".arg(next)          

          console.log(JSON.stringify(root.model.get(root.index)))
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          while( root.backend.select_event(self) )
          {
            next = next +1
            self.event_id = -1
            self.name = "New Event %1".arg(next)
            self.description = "Description of Event %1".arg(next)
          }
          root.backend.update_event(self)
          full_listArea.model.insert(listArea.model.count,
            {
             "id" : self.event_id,
             "name": "%1".arg(self.name), 
             "location" : self.location,
             "actor" : self.actor,
             "equipment" : "%1".arg(self.equipment),
             "description": "%1".arg(self.description)}
          );
          ++next;
        }
        onThirdButtonClicked : {
          self.event_id = -1
          self.name = full_listArea.model.get(full_listArea.currentIndex).name
          root.backend.remove_event(self)
          full_listArea.model.remove(full_listArea.currentIndex)
          full_listArea.currentIndex = Math.max(0, full_listArea.currentIndex-1)
        }
        onFourthButtonClicked : {
          event_stack.currentIndex = 1
          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            console.log(JSON.stringify(self_scene))
            root.backend.events_in_scene(self_scene)
            while ( root.backend.next_event(self) ) {
              console.log(JSON.stringify(self))
              listArea.model.insert(listArea.model.count,
              {
             "id" : self.event_id,
             "name": "%1".arg(self.name), 
             "location" : self.location,
             "actor" : self.actor,
             "equipment" : "%1".arg(self.equipment),
             "description": "%1".arg(self.description)
              });
            }
          }
        }
      } 

      ListView {
        id : full_listArea
        anchors { top : full_controls.bottom ; bottom : parent.bottom; 
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
          id : full_event
          color : 'transparent'
          border.color: "steelblue"
          height : 30
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              full_listArea.currentIndex = index  

            }
          }  

          Text {
            id : full_event_title_text
            anchors.left : full_event.left
            anchors.leftMargin : 5
            text :  model.name
            width : 150
            font.weight: Font.Bold
            font.pointSize: 10
            enabled : false
            color: enabled ? Material.primaryTextColor : Material.primaryTextColor
          }  

          Text {
            id : full_event_value_text
            anchors.left : full_event_title_text.right
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
            PropertyChanges{ target : full_event_title_text; enabled : true}
            PropertyChanges{ target : full_event_value_text; enabled  : true}
          }  

          onFocusChanged: {
            if(full_listArea.currentIndex == index){
              state = 'Selected';
            }
            else{
              state = '';
            }
          }
        }
        ScrollBar.vertical: ScrollBar { }  
      }      
    }
    StackLayout {
      id: contentStack
      Layout.fillWidth: true
      Layout.fillHeight: true
      currentIndex: 0
    Rectangle {
      id : listRectangle
      Layout.fillWidth : true
      Layout.fillHeight: true
      Layout.margins : 5  

      border.color : "black"  

      ThreeButtonRow {
        id: controls
        anchors.top : listRectangle.top
        anchors.left : listRectangle.left
        anchors.right : listRectangle.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5  

        property int next : 1  

        firstButtonText : "Add"
        secondButtonText : "New"
        thirdButtonText : "Remove"  

        onFirstButtonClicked :{
          event_stack.currentIndex = 0
          full_listArea.model.clear()
          root.backend.events()
          while ( root.backend.next_event(self) ) {
            console.log(JSON.stringify(self))
            full_listArea.model.insert(full_listArea.model.count,
            {
             "id" : self.event_id,
             "name": "%1".arg(self.name), 
             "location" : self.location,
             "actor" : self.actor,
             "equipment" : "%1".arg(self.equipment),
             "description": "%1".arg(self.description)
              });
          }
        }
        onSecondButtonClicked :{
          if( next < listArea.model.count ) 
          { next = listArea.model.count +1}
          self.event_id = -1
          self.name = "New Event %1".arg(next)
          self.location = -1
          self.actor = -1
          self.equipment = "New Equipment %1".arg(next)
          self.description = "Description of Event %1".arg(next)
          console.log(JSON.stringify(root.model.get(root.index)))

          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          while( root.backend.select_event(self) )
          { 
           next = next +1
           self.event_id = -1; 
           self.name = "New Event %1".arg(next);
           self.location = -1
           self.actor = -1
           self.equipment = "New Equipment %1".arg(next)
           self.description = "Description of Event %1".arg(next)
          }   

          root.backend.update_event_in_scene(self_scene, self)
          listArea.model.insert(listArea.model.count,
            {
             "id" : self.event_id,
             "name": "%1".arg(self.name), 
             "location" : self.location,
             "actor" : self.actor,
             "equipment" : "%1".arg(self.equipment),
             "description": "%1".arg(self.description)}
          );
          ++next;
        }
        onThirdButtonClicked : {
          self.event_id = listArea.model.get(listArea.currentIndex).event_id
          self.name = listArea.model.get(listArea.currentIndex).name
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name          
          root.backend.remove_event_from_scene(self,self_scene)
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
            onDoubleClicked: {
              console.log("DOUBLECLICK")
              console.log(JSON.stringify(listArea.model.get(index)))
              eventEdit.name = listArea.model.get(index).name
              eventEdit.description = listArea.model.get(index).description
              eventEdit.eventID = listArea.model.get(index).event_id
              contentStack.currentIndex = 1
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
              });
          }
        }
      }
    }
    EventEditPane {
      id: eventEdit
      Layout.fillWidth : true
      Layout.fillHeight: true
      Layout.margins : 5  
      backend: root.backend
      onExit : {
        contentStack.currentIndex = 0
        event_stack.currentIndex = 1
        var values = model.get(index)
        if (values) {
          listArea.model.clear()
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          console.log(JSON.stringify(self_scene))
          root.backend.events_in_scene(self_scene)
          while ( root.backend.next_event(self) ) {
            console.log(JSON.stringify(self))
            listArea.model.insert(listArea.model.count,
            {
              "event_id" : self.event_id,
              "name" : "%1".arg(self.name),
              "location" : self.location,
              "actor" : self.actor,
              "equipment" : "%1".arg(self.equipment),
              "description" : "%1".arg(self.description)
            });
          }
        }        
      }
      //border.color : "black"  
    }
    }
  }
    onIndexChanged : {
      console.log("index changed----------")
      event_stack.currentIndex = 1
      var values = model.get(index)
      if (values) {
        listArea.model.clear()
        self_scene.scene_id = root.model.get(root.index).id
        self_scene.name = root.model.get(root.index).name
        console.log(JSON.stringify(self_scene))
        root.backend.events_in_scene(self_scene)
        while ( root.backend.next_event(self) ) {
          console.log(JSON.stringify(self))
          listArea.model.insert(listArea.model.count,
          {
            "event_id" : self.event_id,
            "name" : "%1".arg(self.name),
            "location" : self.location,
            "actor" : self.actor,
            "equipment" : "%1".arg(self.equipment),
            "description" : "%1".arg(self.description)
          });
        }
      }
    }
}