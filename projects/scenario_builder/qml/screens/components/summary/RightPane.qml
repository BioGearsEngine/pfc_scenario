import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12


  import "../common"
  import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
    Citation {
      id : cit
    }
    Equipment {
      id : equ
    }
    Event {
      id : eve
    }
    id: root
    property SQLBackend backend
    property int index
    clip : true
    anchors.fill : parent
    Label {
      property int pointSize : 10
      Text {
        text: ' Events'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : eveList
      //anchors { top : refList.bottom ; bottom : parent.bottom ; left : parent.left ; right : parent.right }
      //height : parent.height / 2
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      border.color : "black"
      ListView {
        id : eveListArea
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
          id : event
          color : 'transparent'
          border.color: "steelblue"
          height : event_title_text.height + event_value_text.height
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              eveListArea.currentIndex = index  

            }
            onDoubleClicked: {

              eventEdit.name = eveListArea.model.get(index).name
              eventEdit.description = eveListArea.model.get(index).description
              eventEdit.eventID = eveListArea.model.get(index).id
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
            elide: Text.ElideRight
          }  

          states: State {
            name : "Selected"
            PropertyChanges{ target : event_title_text; enabled : true}
            PropertyChanges{ target : event_value_text; enabled  : true}
          }  

          onFocusChanged: {
            if(eveListArea.currentIndex == index){
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
          while ( root.backend.next_event(eve) ){  

            eveListArea.model.insert(eveListArea.model.count,
              {
               id  : eve.event_id,
               name: "%1".arg(eve.name), 
               description: "%1".arg(eve.description),
              });
          }
        }
      }
    }
    Label {
      property int pointSize : 10
      Text {
        text: ' Equipment'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : equList
      //anchors { top : refList.bottom ; bottom : parent.bottom ; left : parent.left ; right : parent.right }
      //height : parent.height / 2
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      border.color : "black"
      ListView {
        id : equListArea
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
        id : equipment
        color : 'transparent'
        border.color: "steelblue"
        height : equipment_title_text.height + equipment_value_text.height
        anchors { left : parent.left; right: parent.right ; margins : 5 }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            equListArea.currentIndex = index

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
          elide: Text.ElideRight
        }

        states: State {
          name : "Selected"
          PropertyChanges{ target : equipment_title_text; enabled : true}
          PropertyChanges{ target : equipment_value_text; enabled  : true}
        }

        onFocusChanged: {
          if(equListArea.currentIndex == index){
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
          while ( root.backend.next_equipment(equ) ){
            equListArea.model.insert(equListArea.model.count,
              {
               id : equ.equipment_id,
               type : "%1".arg(equ.type),
               name: "%1".arg(equ.name), 
               description: "%1".arg(equ.description), 
               citations: equ.citations,
               image: "%1".arg(equ.image) 
              });
          }
        }
      }
    }
    Label {
      property int pointSize : 10
      Text {
        text: ' References'
        font.pointSize : 20
        color: "steelblue"
      }
      font.pointSize : 20
      Layout.fillWidth : true
      Layout.bottomMargin : 5
    }
    Rectangle {
      id : refList
      //anchors { top : parent.top ; bottom : equList.top ; left : parent.left ; right : parent.right }
      Layout.fillHeight : true
      Layout.fillWidth : true
      Layout.bottomMargin : 5
      //height : parent.height / 2
      border.color : "black"
      ListView {
        id : citListArea
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
          id : citation
          color : 'transparent'
          border.color: "steelblue"
          height : citation_title_text.height + citation_value_text.height
          anchors { left : parent.left; right: parent.right ; margins : 5 }  

          MouseArea {
            anchors.fill: parent
            onClicked: {
              citListArea.currentIndex = index  

            }
            onDoubleClicked: {
            }
          }  

          Text {
            id : citation_title_text
            anchors.left : citation.left
            anchors.leftMargin : 5
            text :  model.title
            width : 150
            font.weight: Font.Bold
            font.pointSize: 10
            enabled : false
          }  

          Text {
            id : citation_value_text
            anchors.top : citation_title_text.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.leftMargin : 10
            font.pointSize: 10
            text :  model.key
            enabled : false
            elide: Text.ElideRight
          }  

          states: State {
            name : "Selected"
            PropertyChanges{ target : citation_title_text; enabled : true}
            PropertyChanges{ target : citation_value_text; enabled  : true}
          }  

          onFocusChanged: {
            if(citListArea.currentIndex == index){
              state = 'Selected';
            }
            else{
              state = '';
            }
          }
        }  

        ScrollBar.vertical: ScrollBar { }  

        Component.onCompleted : {
          var r_count = backend.citation_count();
          root.backend.citations()
          while ( root.backend.next_citation(cit) ){  
            citListArea.model.insert(citListArea.model.count,
              {
               id  : cit.citation_id,
               key: "%1".arg(cit.key), 
               title: "%1".arg(cit.title),
               authors: "%1".arg(cit.authors),
               year: "%1".arg(cit.year),
               publisher: "%1".arg(cit.publisher)
              });
          }
        }
      }
    }
    onIndexChanged : {
      if (index == 3) {
        citListArea.model.clear()
        var c_count = backend.citation_count();
        root.backend.citations()
        while ( root.backend.next_citation(cit) ){  
          citListArea.model.insert(citListArea.model.count,
            {
             id  : cit.citation_id,
             key: "%1".arg(cit.key), 
             title: "%1".arg(cit.title),
             authors: "%1".arg(cit.authors),
             year: "%1".arg(cit.year),
             publisher: "%1".arg(cit.publisher)
            });
        }        
        equListArea.model.clear()
        var eq_count = backend.equipment_count()
        root.backend.equipments()
        while ( root.backend.next_equipment(equ) ){  //equ
          equListArea.model.insert(equListArea.model.count,
            {
              id : equ.equipment_id,
              type : "%1".arg(equ.type),
              name: "%1".arg(equ.name), 
              description: "%1".arg(equ.description), 
              citations: equ.citations,
              image: "%1".arg(equ.image) 
            });
        }  
        eveListArea.model.clear()
        var ev_count = backend.event_count()
        root.backend.events()
        while ( root.backend.next_event(eve) ){  //eve
          eveListArea.model.insert(eveListArea.model.count,
            {
              id: eve.event_id,
              name: "%1".arg(eve.name), 
              description: "%1".arg(eve.description),
              actor_1: "%1".arg(eve.actor_1),
              actor_2: "%1".arg(eve.actor_2),
              fidelity: "%1".arg(eve.fidelity),
              category: "%1".arg(eve.type),
              details: "%1".arg(eve.details)
            });
        }  
      }
    }
}