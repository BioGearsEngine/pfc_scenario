import QtQuick 2.4
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

  signal reloadEquipmentList();

  Equipment {
    id : self
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
        self.equipment_id = -1
        self.type = 1
        self.name = "New Equipment %1".arg(next)
        self.description = "Description of Equipment %1".arg(next)
        self.image = ""
        self.citations = ""
        self.properties = ""

        while( root.backend.select_equipment(self) )
        { 
         next = next +1
         self.equipment_id = -1; 
         self.name = "New Equipment %1".arg(next);
         self.description = "Description of Equipment %1".arg(next)
        } 
        root.backend.update_equipment(self)
        root.model.insert(root.model.count,
          {
           "id" : self.equipment_id,
           "type" : "1", //Change this to not be default later
           "name": "%1".arg(self.name), 
           "description": "%1".arg(self.description) , 
           "citations": self.citations,
           "image": self.image,

          }
        );
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        self.equipment_id = -1
        self.name = root.model.get(root.index).name

        root.backend.remove_equipment(self)
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
        id : equipment
        color : 'transparent'
        border.color: "steelblue"
        height : equipment_title_text.height + equipment_value_text.height
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            listArea.currentIndex = index

          }
        }

        Text {
          id : equipment_title_text
          anchors.left : equipment.left
          anchors.leftMargin : 5
          text :  model.name
          width : 150
          font.weight: Font.Bold
          font.pointSize: 10
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
        }

        Text {
          id : equipment_value_text
          anchors.top : equipment_title_text.bottom
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
          PropertyChanges{ target : equipment_title_text; enabled : true}
          PropertyChanges{ target : equipment_value_text; enabled  : true}
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
        var r_count = backend.equipment_count();
        root.backend.equipments()
        while ( root.backend.next_equipment(self) ){
          listArea.model.insert(listArea.model.count,
            {
             id : self.equipment_id,
             type : "%1".arg(self.type),
             name: "%1".arg(self.name), 
             description: "%1".arg(self.description), 
             citations: self.citations,
             image: "%1".arg(self.image) 
            });
        }
      }
    }
  }
  onReloadEquipmentList : {
    listArea.model.clear()
    root.backend.equipments()
    while (root.backend.next_equipment(self) ){
      listArea.model.insert(listArea.model.count, 
      {
        id : self.equipment_id,
        type : "%1".arg(self.type),
        name: "%1".arg(self.name), 
        description: "%1".arg(self.description), 
        citations: self.citations,
        image: "%1".arg(self.image) 
      });
    }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
