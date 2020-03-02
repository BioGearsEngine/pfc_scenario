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


  Treatment {
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
        self.treatment_id = -1
        self.medical_name = "New Treatment %1".arg(next)
        self.common_name = "New Treatment %1".arg(next)
        self.description = "Description of Treatment %1".arg(next)
        self.equipment = ""
        self.citations = ""

        while( root.backend.select_treatment(self) )
        { 
         next = next +1
         self.treatment_id = -1; 
         self.medical_name = "New Treatment %1".arg(next);
         self.common_name = "New Treatment %1".arg(next);
         self.description = "Description of Treatment %1".arg(next)
        } 

        root.backend.update_treatment(self)
        root.model.insert(root.model.count,
          {
           "id" : self.treatment_id,
           "medical_name": "%1".arg(self.medical_name), 
           "common_name": "%1".arg(self.common_name), 
           "description": "%1".arg(self.description) , 
           "citations": self.citaitons,
           "equipment": self.equipment,

          }
        );
        ++next;
      }
      onSecondButtonClicked : {
        if (root.model.count == 0) {
          return
        }
        self.treatment_id = -1
        self.medical_name = root.model.get(root.index).medical_name

        root.backend.remove_treatment(self)
        root.model.remove(root.index)
        if (listArea.currentIndex == 0) { // If the index was 0 this wasn't registering as an index change and the right pane wasn't reloading
          listArea.currentIndex = 1
        } 
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
          Layout.alignment: Qt.AlignTop
          Layout.fillWidth: true
          Layout.margins : 5
      }

      model : ListModel {}

      delegate : Rectangle {
        id : treatment
        color : 'transparent'
        border.color: "steelblue"
        height : treatment_title_text.height + treatment_value_text.height
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            listArea.currentIndex = index

          }
        }

        Text {
          id : treatment_title_text
          anchors.left : treatment.left
          anchors.leftMargin : 5
          text :  model.medical_name
          width : 150
          font.weight: Font.Bold
          font.pointSize: 10
          enabled : false
          color: enabled ? Material.primaryTextColor : Material.primaryTextColor
        }

        Text {
          id : treatment_value_text
          anchors.top : treatment_title_text.bottom
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
          PropertyChanges{ target : treatment_title_text; enabled : true}
          PropertyChanges{ target : treatment_value_text; enabled  : true}
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
        var r_count = backend.treatment_count();
        root.backend.treatments()
        while ( root.backend.next_treatment(self) ){
          listArea.model.insert(listArea.model.count,
            {
             id : self.treatment_id,
             medical_name: "%1".arg(self.medical_name), 
             common_name: "%1".arg(self.common_name), 
             description: "%1".arg(self.description) , 
             equipment: self.equipment,
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
