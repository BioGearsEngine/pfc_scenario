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
  property int index // = roles.index

  Role {
    id : self
  }
  Scene {
    id : self_scene
  }


  function update_role(values) {
    obj.role_id = values.id
    obj.name = values.name
    obj.description = values.description
    root.backend.update_role(obj)
  }
  StackLayout {
    id: role_stack
    currentIndex: 1
    Rectangle {
      id: full_listRectangle
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 5

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
          self.role_id = -1
          self.name = full_listArea.model.get(full_listArea.currentIndex).name
          self.description = full_listArea.model.get(full_listArea.currentIndex).description
          console.log(JSON.stringify(root.model.get(root.index)))
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          root.backend.update_role_in_scene(self_scene,self)
          role_stack.currentIndex = 1
          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            console.log(JSON.stringify(self_scene))
            root.backend.roles_in_scene(self_scene)
            while ( root.backend.next_role(self) ) {
              console.log(JSON.stringify(self))
              listArea.model.insert(listArea.model.count,
              {
                "role_id" : self.role_id,
                "name" : "%1".arg(self.name),
                "description" : "%1".arg(self.description)
              });
            }
          }
        }
        onSecondButtonClicked :{
          if ( next < full_listArea.count )
          { next = full_listArea.model.count + 1 }
          self.role_id = -1
          self.name = "New Role %1".arg(next)
          self.description = "Description of Role %1".arg(next)
          console.log(JSON.stringify(root.model.get(root.index)))
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          while( root.backend.select_role(self) )
          {
            next = next +1
            self.role_id = -1
            self.name = "New Role %1".arg(next)
            self.description = "Description of Role %1".arg(next)
          }
          root.backend.update_role(self)
          full_listArea.model.insert(listArea.model.count,
            {
            "id" : self.role_id,
            "name" : "%1".arg(self.name),
            "description" : "%1".arg(self.description)}
          );
          ++next;
        }
        onThirdButtonClicked : {
          self.role_id = -1
          self.name = full_listArea.model.get(full_listArea.currentIndex).name
          root.backend.remove_role(self)
          full_listArea.model.remove(full_listArea.currentIndex)
          full_listArea.currentIndex = Math.max(0, full_listArea.currentIndex-1)
        }
        onFourthButtonClicked : {
          role_stack.currentIndex = 1
          var values = model.get(index)
          if (values) {
            listArea.model.clear()
            self_scene.scene_id = root.model.get(root.index).id
            self_scene.name = root.model.get(root.index).name
            console.log(JSON.stringify(self_scene))
            root.backend.roles_in_scene(self_scene)
            while ( root.backend.next_role(self) ) {
              console.log(JSON.stringify(self))
              listArea.model.insert(listArea.model.count,
              {
                "role_id" : self.role_id,
                "name" : "%1".arg(self.name),
                "description" : "%1".arg(self.description)
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
          id : full_role
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
            id : full_role_title_text
            anchors.left : full_role.left
            anchors.leftMargin : 5
            text :  model.name
            width : 150
            font.weight: Font.Bold
            font.pointSize: 10
            enabled : false
            color: enabled ? Material.primaryTextColor : Material.primaryTextColor
          }  

          Text {
            id : full_role_value_text
            anchors.left : full_role_title_text.right
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
            PropertyChanges{ target : full_role_title_text; enabled : true}
            PropertyChanges{ target : full_role_value_text; enabled  : true}
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

        firstButtonText : "Add"
        fourthButtonText : "Remove"  

        secondButtonText : "New"
        thirdButtonText : "Move Down"  

        onFirstButtonClicked :{
          role_stack.currentIndex = 0
          full_listArea.model.clear()
          root.backend.roles()
          while ( root.backend.next_role(self) ) {
            console.log(JSON.stringify(self))
            full_listArea.model.insert(full_listArea.model.count,
            {
              "role_id" : self.role_id,
              "name" : "%1".arg(self.name),
              "description" : "%1".arg(self.description)
              });
          }
        }
        onSecondButtonClicked :{
          if( next < listArea.model.count ) 
          { next = listArea.model.count +1}
          self.role_id = -1
          self.name = "New Role %1".arg(next)
          self.description = "Description of Role %1".arg(next)
          console.log(JSON.stringify(root.model.get(root.index)))
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          while( root.backend.select_role(self) )
          { 
           next = next +1
           self.role_id = -1; 
           self.name = "New Role %1".arg(next);
           self.description = "Description of Role %1".arg(next)
          }   

          root.backend.update_role_in_scene(self_scene, self)
          listArea.model.insert(listArea.model.count,
            {
             "id" : self.role_id,
             "name": "%1".arg(self.name), 
             "description": "%1".arg(self.description)}
          );
          ++next;
        }
        onThirdButtonClicked : {
          console.log("Reordering Currently Unsupported!")
        }
        onFourthButtonClicked : {
          console.log(JSON.stringify(listArea.model.get(listArea.currentIndex)))
          self.role_id = listArea.model.get(listArea.currentIndex).role_id
          self.name = listArea.model.get(listArea.currentIndex).name
          self_scene.scene_id = root.model.get(root.index).id
          self_scene.name = root.model.get(root.index).name
          root.backend.remove_role_from_scene(self,self_scene)
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
          id : role
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
            id : role_title_text
            anchors.left : role.left
            anchors.leftMargin : 5
            text :  model.name
            width : 150
            font.weight: Font.Bold
            font.pointSize: 10
            enabled : false
            color: enabled ? Material.primaryTextColor : Material.primaryTextColor
          }  

          Text {
            id : role_value_text
            anchors.left : role_title_text.right
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
            PropertyChanges{ target : role_title_text; enabled : true}
            PropertyChanges{ target : role_value_text; enabled  : true}
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
          var r_count = backend.role_count();
          root.backend.roles()
          while ( root.backend.next_role(self) ){  

            listArea.model.insert(listArea.model.count,
              {
               id  : self.role_id,
               name: "%1".arg(self.name), 
               description: "%1".arg(self.description)
              });
          }
        }
      }
    }
  }
    onIndexChanged : {
      console.log("index changed----------")
      role_stack.currentIndex = 1
      var values = model.get(index)
      if (values) {
        listArea.model.clear()
        self_scene.scene_id = root.model.get(root.index).id
        self_scene.name = root.model.get(root.index).name
        console.log(JSON.stringify(self_scene))
        root.backend.roles_in_scene(self_scene)
        while ( root.backend.next_role(self) ) {
          console.log(JSON.stringify(self))
          listArea.model.insert(listArea.model.count,
          {
            "role_id" : self.role_id,
            "name" : "%1".arg(self.name),
            "description" : "%1".arg(self.description)
          });
        }
      }
    }
}