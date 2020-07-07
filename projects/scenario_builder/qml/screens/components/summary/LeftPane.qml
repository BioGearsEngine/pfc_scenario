import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
    Scene {
      id : sce
    }
    Role {
      id : rol
    }
    Objective {
      id : obj
    }
    id: root
    property SQLBackend backend
    property int index
    clip : true
    anchors.fill : parent
    Label {
      property int pointSize : 10
      Text {
        text: ' Scenes'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : sceList
      //anchors { top : parent.top ; bottom : equList.top ; left : parent.left ; right : parent.right }
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      //height : parent.height / 2
      border.color : "black"
      ListView {
        id : sceListArea
        anchors { top : parent.top ; bottom : parent.bottom; 
                     left : parent.left ; right : parent.right } 
        height : parent.height
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
            sceListArea.currentIndex = index

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
        }
        onFocusChanged: {
          if(sceListArea.currentIndex == index){
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
          while ( root.backend.next_scene(sce) ){ 

            sceListArea.model.insert(sceListArea.model.count,
              {
               id  : sce.scene_id,
               name: "%1".arg(sce.name), 
               description : sce.description,
               details : sce.details
              });
          }
        }
      }
    }
    Label {
      property int pointSize : 10
      Text {
        text: ' Roles'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : rolList
      //anchors { top : refList.bottom ; bottom : parent.bottom ; left : parent.left ; right : parent.right }
      //height : parent.height / 2
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      border.color : "black"
      ListView {
        id : rolListArea
        anchors { top : parent.top ; bottom : parent.bottom; 
                     left : parent.left ; right : parent.right } 
        height : parent.height
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
          id : role
          color : 'transparent'
          border.color: "steelblue"
          height : role_title_text.height + role_value_text.height
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              rolListArea.currentIndex = index  

            }
            onDoubleClicked: {
              roleEdit.name = rolListArea.model.get(index).name
              roleEdit.description = rolListArea.model.get(index).description
              roleEdit.roleID = rolListArea.model.get(index).id
              contentStack.currentIndex = 1
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
          }  

          Text {
            id : role_value_text
            anchors.top : role_title_text.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.leftMargin : 10
            font.pointSize: 10
            text :  description
            enabled : false
            elide: Text.ElideRight
          }  

          states: State {
            name : "Selected"
            PropertyChanges{ target : role_title_text; enabled : true}
            PropertyChanges{ target : role_value_text; enabled  : true}
          }  

          onFocusChanged: {
            if(rolListArea.currentIndex == index){
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
          while ( root.backend.next_role(rol) ){  
            rolListArea.model.insert(rolListArea.model.count,
              {
               id  : rol.role_id,
               name: "%1".arg(rol.name), 
               description: "%1".arg(rol.description)
              });
          }
        }
      }
    }

    Label {
      property int pointSize : 10
      Text {
        text: ' Objectives'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : objList
      //anchors { top : refList.bottom ; bottom : parent.bottom ; left : parent.left ; right : parent.right }
      //height : parent.height / 2
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      border.color : "black"
      ListView {
        id : objListArea
        anchors { top : parent.top ; bottom : parent.bottom; 
                     left : parent.left ; right : parent.right } 
        height : parent.height
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
          id : objective
          color : 'transparent'
          border.color: "steelblue"
          height : objective_title_text.height + objective_value_text.height
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              objListArea.currentIndex = index  

            }
          }  

          Text {
            id : objective_title_text
            anchors.left : objective.left
            anchors.leftMargin : 5
            text :  model.name
            width : 150
            font.weight: Font.Bold
            font.pointSize: 10
            enabled : false
          }  

          Text {
            id : objective_value_text
            anchors.top : objective_title_text.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.leftMargin : 10
            font.pointSize: 10
            text :  description
            enabled : false
            elide: Text.ElideRight
          }  

          states: State {
            name : "Selected"
            PropertyChanges{ target : objective_title_text; enabled : true}
            PropertyChanges{ target : objective_value_text; enabled  : true}
          }  

          onFocusChanged: {
            if(objListArea.currentIndex == index){
              state = 'Selected';
            }
            else{
              state = '';
            }
          }
        }

        ScrollBar.vertical: ScrollBar { }  

        Component.onCompleted : {
          var r_count = backend.objective_count();
          root.backend.objectives()
          while ( root.backend.next_objective(obj) ){
            objListArea.model.insert(objListArea.model.count,
              {
               id  : obj.objective_id,
               name: "%1".arg(obj.name), 
               description: "%1".arg(obj.description),
               citations : obj.citations
              });
          }
        }
      }
    }
    onIndexChanged : {
      if (index == 3) {
        rolListArea.model.clear()
        var r_count = backend.role_count();
        root.backend.roles()
        while ( root.backend.next_role(rol) ){  
          rolListArea.model.insert(rolListArea.model.count,
            {
             id  : rol.role_id,
             name: "%1".arg(rol.name), 
             description: "%1".arg(rol.description)
            });
        }
        objListArea.model.clear()
        var o_count = backend.objective_count();
        root.backend.objectives()
        while ( root.backend.next_objective(obj) ){
          objListArea.model.insert(objListArea.model.count,
            {
             id  : obj.objective_id,
             name: "%1".arg(obj.name), 
             description: "%1".arg(obj.description),
             citations : obj.citations
            });
        }
        sceListArea.model.clear()
        var s_count = backend.scene_count();
        root.backend.scenes()
        while ( root.backend.next_scene(sce) ){ 
          sceListArea.model.insert(sceListArea.model.count,
            {
             id  : sce.scene_id,
             name: "%1".arg(sce.name), 
             description : sce.description,
             details : sce.details
            });
        }
      }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
