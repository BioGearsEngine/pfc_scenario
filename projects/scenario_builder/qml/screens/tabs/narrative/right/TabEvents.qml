import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id: root
  property SQLBackend backend
  property ListModel model
  property int index // = events.index
  property int count

  Event {
    id : self
  }
  Scene {
    id : self_scene
  }

  function update_event(values) {
    obj.event_id = values.id
    obj.name = values.name
    obj.description = values.description
    obj.category = values.category
    obj.fidelity = values.fidelity
    obj.actor_1 = values.actor_1
    obj.actor_2 = values.actor_2
    obj.equipment = values.equipment
    obj.details = values.details
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
          if (full_listArea.model.count == 0) {
            return
          }
          if ( next < full_listArea.count )
          { next = full_listArea.model.count + 1 }
          self.event_id = -1
          self.name = full_listArea.model.get(full_listArea.currentIndex).name
          self.description = full_listArea.model.get(full_listArea.currentIndex).description

          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          root.backend.update_event_in_scene(self_scene,self)
          event_stack.currentIndex = 1

          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name

            root.backend.events_in_scene(self_scene)
            while ( root.backend.next_event(self) ) {

              listArea.model.insert(listArea.model.count,
              {
               "id" : self.event_id,
               "name": "%1".arg(self.name), 
               "description": "%1".arg(self.description),
               "actor_1": "%1".arg(self.actor_1),
               "actor_2": "%1".arg(self.actor_2),
               "fidelity": "%1".arg(self.fidelity),
               "category": "%1".arg(self.type),
               "details": "%1".arg(self.details)
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
             "description": "%1".arg(self.description),
             "actor_1": "%1".arg(self.actor_1),
             "actor_2": "%1".arg(self.actor_2),
             "fidelity": "%1".arg(self.fidelity),
             "category": "%1".arg(self.type),
             "details": "%1".arg(self.details)}
          );
          ++next;
        }
        onThirdButtonClicked : {
          if (full_listArea.model.count == 0) {
            return
          }
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
            root.backend.events_in_scene(self_scene)
            while ( root.backend.next_event(self) ) {
              listArea.model.insert(listArea.model.count,
              {
              "id" : self.event_id,
              "name": "%1".arg(self.name), 
              "description": "%1".arg(self.description),
              "actor_1": "%1".arg(self.actor_1),
              "actor_2": "%1".arg(self.actor_2),
              "fidelity": "%1".arg(self.fidelity),
              "category": "%1".arg(self.type),
              "details": "%1".arg(self.details)
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
        highlightMoveDuration : 1
        highlight: Rectangle {
            color: '#1111110F'
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.margins : 5
        }  

        model : ListModel {}  

        delegate : Rectangle {
          id : full_event
          color : 'transparent'
          border.color: "steelblue"
          height : full_event_title_text.height + full_event_value_text.height
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
            anchors.top : full_event_title_text.bottom
            anchors.left : parent.left
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
            if (count == 0) {
              return
            }
            event_stack.currentIndex = 0
            full_listArea.model.clear()
            root.backend.events()
            while ( root.backend.next_event(self) ) {
              full_listArea.model.insert(full_listArea.model.count,
              {
               "id" : self.event_id,
               "name": "%1".arg(self.name), 
               "description": "%1".arg(self.description),
               "actor_1": "%1".arg(self.actor_1),
               "actor_2": "%1".arg(self.actor_2),
               "fidelity": "%1".arg(self.fidelity),
               "category": "%1".arg(self.type),
               "details": "%1".arg(self.details)
                });
            }
          }
          onSecondButtonClicked :{
            if (count == 0) {
              return
            }
            if( next < listArea.model.count ) 
            { next = listArea.model.count +1}
            self.event_id = -1
            self.name = "New Event %1".arg(next)
            self.description = "Description of Event %1".arg(next)  
  

            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            while( root.backend.select_event(self) )
            { 
             next = next +1
             self.event_id = -1; 
             self.name = "New Event %1".arg(next);
             self.description = "Description of Event %1".arg(next)
            }     

            root.backend.update_event_in_scene(self_scene, self)
            listArea.model.insert(listArea.model.count,
              {
               "id" : self.event_id,
               "name": "%1".arg(self.name), 
               "description": "%1".arg(self.description),
               "actor_1": "%1".arg(self.actor_1),
               "actor_2": "%1".arg(self.actor_2),
               "fidelity": "%1".arg(self.fidelity),
               "category": "%1".arg(self.type),
               "details": "%1".arg(self.details)}
            );
            ++next;
          }
          onThirdButtonClicked : {
            if (count == 0) {
              return
            }
            self.event_id = listArea.model.get(listArea.currentIndex).id
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
        highlightMoveDuration : 1
        highlight: Rectangle {
            color: '#1111110F'
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.margins : 5
        }  

        model : ListModel {}  

        delegate : Rectangle {
          id : event
          color : 'transparent'
          border.color: "steelblue"
          height : event_title_text.height + event_value_text.height
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              listArea.currentIndex = index  

            }
            onDoubleClicked: {

              eventEdit.name = listArea.model.get(index).name
              eventEdit.description = listArea.model.get(index).description
              eventEdit.eventID = listArea.model.get(index).id
              eventEdit.actor_1 = listArea.model.get(index).actor_1
              eventEdit.actor_2 = listArea.model.get(index).actor_2
              eventEdit.details = listArea.model.get(index).details
              var fid = listArea.model.get(index).fidelity
              if (fid == "LOW") {
                eventEdit.fidelityIndex = 0
              } else if (fid == "MEDIUM") {
                eventEdit.fidelityIndex = 1
              } else if (fid == "HIGH") {
                eventEdit.fidelityIndex = 2
              } else {
                eventEdit.fidelityIndex = 0
              }
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
            anchors.top : event_title_text.bottom
            anchors.left : parent.left
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
               "id"  : self.event_id,
               "name": "%1".arg(self.name), 
               "description": "%1".arg(self.description),
               "actor_1": "%1".arg(self.actor_1),
               "actor_2": "%1".arg(self.actor_2),
               "fidelity": "%1".arg(self.fidelity),
               "category": "%1".arg(self.type),
               "details": "%1".arg(self.details)
              });
          }
        }
      }
    }
    Rectangle { 
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      border.color : "black"
      border.width : 1
      EventEditPane {
        id: eventEdit
        anchors.fill : parent
        anchors.margins : 2
        backend: root.backend
        onExit : {
          contentStack.currentIndex = 0
          event_stack.currentIndex = 1
          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            root.backend.events_in_scene(self_scene)
            while ( root.backend.next_event(self) ) {
              listArea.model.insert(listArea.model.count,
              {
                "id" : self.event_id,
                "name" : "%1".arg(self.name),
                "description" : "%1".arg(self.description),
                "actor_1": "%1".arg(self.actor_1),
                "actor_2": "%1".arg(self.actor_2),
                "fidelity": "%1".arg(self.fidelity),
                "category": "%1".arg(self.type),
                "details": "%1".arg(self.details)
              });
            }
          }        
        }
        //border.color : "black"  
      }
    }
    }
  }
    onIndexChanged : {
      event_stack.currentIndex = 1
      var values = model.get(index)
      if (values) {
        listArea.model.clear()
        self_scene.scene_id = root.model.get(root.index).id
        self_scene.name = root.model.get(root.index).name
        root.backend.events_in_scene(self_scene)
        while ( root.backend.next_event(self) ) {
          listArea.model.insert(listArea.model.count,
          {
            "id" : self.event_id,
            "name" : "%1".arg(self.name),
            "description" : "%1".arg(self.description),
            "actor_1": "%1".arg(self.actor_1),
            "actor_2": "%1".arg(self.actor_2),
            "fidelity": "%1".arg(self.fidelity),
            "category": "%1".arg(self.type),
            "details": "%1".arg(self.details)
          });
        }
      }
    }
    onCountChanged : {
      if(count == 0) {
          contentStack.currentIndex = 0
          while (listArea.count > 0) {
            listArea.model.remove(0)
          }
        } else {
          indexChanged()
        }
    }
}