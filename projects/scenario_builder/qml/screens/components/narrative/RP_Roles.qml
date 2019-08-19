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

  function update_role(values) {
    obj.role_id = values.id
    obj.name = values.name
    obj.description = values.description
    obj.fk_scene = values.fk_scene
    root.backend.update_role(obj)
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

      secondButtonText : "Move Up"
      thirdButtonText : "Move Down"

      onFirstButtonClicked :{
        if( next < listArea.model.count ) 
        { next = listArea.model.count +1}
        self.role_id = -1
        self.name = "New Role %1".arg(next)
        self.description = "Description of Role %1".arg(next)
        console.log(JSON.stringify(root.model.get(root.index)))
        self.fk_scene = root.model.get(root.index).id
        while( root.backend.select_role(self) )
        { 
         next = next +1
         self.role_id = -1; 
         self.name = "New Role %1".arg(next);
         self.description = "Description of Role %1".arg(next)
         self.fk_scene = root.model.get(root.index).id
        } 

        root.backend.update_role(self)
        listArea.model.insert(listArea.model.count,
          {
           "id" : self.role_id,
           "name": "%1".arg(self.name), 
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
        self.role_id = -1
        self.name = root.model.get(root.index).name
        root.backend.remove_role(self)
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
      root.backend.roles() 
      while ( root.backend.next_role(self) ) {
        if ( self.fk_scene == root.model.get(root.index).id ) {
          listArea.model.insert(listArea.model.count,
          {
            "role_id" : self.role_id,
            "name" : "%1".arg(self.name),
            "description" : "%1".arg(self.description),
            "fk_scene" : self.fk_scene
            });
        }
      }
    }
  }
}