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


  Injury {
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
        self.injury_id = -1
        self.medical_name = "New Injury %1".arg(next)
        self.common_name = "New Injury %1".arg(next)
        self.description = "Description of Injury %1".arg(next)
        
        self.citations = ""
        self.min = 0.0
        self.max = 1.0

        while( root.backend.select_injury(self) )
        { 
         next = next +1
         self.injury_id = -1; 
         self.medical_name = "New Injury %1".arg(next);
         self.common_name = "New Injury %1".arg(next);
         self.description = "Description of Injury %1".arg(next)
        } 

        root.backend.update_injury(self)
        root.model.insert(root.model.count,
          {
           "id" : self.injury_id,
           "medical_name": "%1".arg(self.medical_name),
           "common_name": "%1".arg(self.common_name),
           "description": "%1".arg(self.description),
           "citations": self.citations,
           "min": self.min,
           "max": self.max

          }
        );
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        self.injury_id = -1
        self.medical_name = root.model.get(root.index).medical_name

        root.backend.remove_injury(self)
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
//          anchors.left : parent.left
//          anchors.right : parent.right
//          anchors.margins: 5
          Layout.alignment: Qt.AlignTop
          Layout.fillWidth: true
          Layout.margins : 5
      }

      model : ListModel {}

      delegate : Rectangle {
        id : injury
        color : 'transparent'
        border.color: "steelblue"
        height : injury_title_text.height + injury_value_text.height
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            listArea.currentIndex = index

          }
        }

        Text {
          id : injury_title_text
          anchors.left : injury.left
          anchors.leftMargin : 5
          text :  model.medical_name
          width : 150
          font.weight: Font.Bold
          font.pointSize: 10
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
        }

        Text {
          id : injury_value_text
          width : parent.width - 5
          height : injury_title_text.height
          anchors.top : injury_title_text.bottom
          anchors.left : parent.left
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize: 10
          text :  description
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
          elide: Text.ElideRight
          wrapMode : Text.WordWrap
        }

        states: State {
          name : "Selected"
          PropertyChanges{ target : injury_title_text; enabled : true}
          PropertyChanges{ target : injury_value_text; enabled  : true}
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
        var r_count = backend.injury_count();
        root.backend.injuries()
        while ( root.backend.next_injury(self) ){
          listArea.model.insert(listArea.model.count,
            {
             id : self.injury_id,
             medical_name: "%1".arg(self.medical_name), 
             common_name:  "%1".arg(self.common_name), 
             description:  "%1".arg(self.description) , 
             min: self.min,
             max: self.max,
             citations: self.citations,

            });
        }
      }
    }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
