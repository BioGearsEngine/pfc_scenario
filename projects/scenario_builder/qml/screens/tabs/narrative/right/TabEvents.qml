import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Scene currentScene
  property Location currentLocation
  Event {
    id : event_g
  }
  Event {
    id : self
  }
  Scene {
    id : self_scene
  }

  function refresh_event_list() {
    eventsInSceneList.events = []
    var eventMaps = root.backend.eventMaps;
    for (var ii = 0; ii < eventMaps.length; ++ ii) {
      if (eventMaps[ii].scene.id == currentScene.id) {
        eventsInSceneList.events.push(event_g.make());
        var index = eventsInSceneList.events.length - 1;
        eventsInSceneList.events[index].assign(eventMaps[ii].event);
      }
    }
    eventsInSceneList.model = eventsInSceneList.events
  }

  function refresh_full_event_list() {
    full_eventsInSceneList.events = []
    var events = root.backend.events;
    for (var ii = 0; ii < events.length; ++ ii) {
      var index = full_eventsInSceneList.events.length;
      full_eventsInSceneList.events.push(event_g.make());
      full_eventsInSceneList.events[index].assign(events[ii]);
    }
    full_eventsInSceneList.model = full_eventsInSceneList.events
  }

  function update_event(event) {
    root.backend.update_event(event)
  }

  StackLayout {
    id : event_stack
    currentIndex : 1
    Rectangle {
      id : full_listRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      color : "transparent"
      border.color : "black"

      FourButtonRow {
        id : full_controls
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


        onFirstButtonClicked : {
          root.backend.update_event_in_scene(currentScene, full_eventsInSceneList.events[full_eventsInSceneList.currentIndex]);
          event_stack.currentIndex = 1
        }
        onSecondButtonClicked : {
          var likely_id = root.backend.nextID(SQLBackend.EVENTS);
          event_g.clear(likely_id);
          root.backend.update_event(event_g);
          full_eventsInSceneList.events.push(event_g.make());
          full_eventsInSceneList.events[full_eventsInSceneList.events.length - 1].assign(event_g);
          full_eventsInSceneList.model = full_eventsInSceneList.events;
          full_eventsInSceneList.currentIndex = full_eventsInSceneList.events.length - 1
        }
        onThirdButtonClicked : {
          root.backend.remove_event(full_eventsInSceneList.events[full_eventsInSceneList.currentIndex])
          refresh_full_event_list();
        }
        onFourthButtonClicked : {
          event_stack.currentIndex = 1
        }
      }
      ListView {
        id : full_eventsInSceneList
         property var events: []
        anchors {
          top : full_controls.bottom;
          bottom : parent.bottom;
          left : parent.left;
          right : parent.right
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }
        model : ListModel {}
        delegate : Rectangle {
          id : full_event
          color : Material.color(Material.BlueGrey)
          border.color : "steelblue"
          height : full_event_title_text.height + full_event_value_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }
          MouseArea {
            anchors.fill : parent
            onClicked : {
              full_eventsInSceneList.currentIndex = index
            }
            onDoubleClicked : {
              eventEdit.returnTo = 0;
              eventEdit.currentEvent = full_eventsInSceneList.events[full_eventsInSceneList.currentIndex];
              event_stack.currentIndex = 2;
            }
          }
          Label {
            id : full_event_title_label
            anchors.left : parent.left
            anchors.leftMargin : 5
            text : "Name: "
            width : 50
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.color(Material.Grey) : Material.secondaryTextColor
          }
          Text {
            id : full_event_title_text
            anchors.left : full_event_title_label.right
            anchors.leftMargin : 5
            text : (full_eventsInSceneList.events[index]) ? full_eventsInSceneList.events[index].name : "Undefined"

            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : full_event_value_label
            anchors.top : full_event_title_text.bottom
            anchors.left : parent.left
            anchors.leftMargin : 5
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Description: "
            enabled : false
            color : enabled ? Material.color(Material.Grey) : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          Text {
            id : full_event_value_text
            anchors.top : full_event_title_text.bottom
            anchors.left : full_event_value_label.right
            anchors.right : parent.right

            anchors.leftMargin : 2
            font.pointSize : 10
            text : (full_eventsInSceneList.events[index]) ? full_eventsInSceneList.events[index].description : "Undefined"
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : full_event_title_text;
              enabled : true
            }
            PropertyChanges {
              target : full_event_value_text;
              enabled : true
            }
            PropertyChanges {
              target : full_event_title_label;
              enabled : true
            }
            PropertyChanges {
              target : full_event_value_label;
              enabled : true
            }
          }
          onFocusChanged : {
            if (full_eventsInSceneList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }
        ScrollBar.vertical : ScrollBar {}
      }
    }

    Rectangle {
      id : listRectangle
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5

      border.color : "black"

      ThreeButtonRow {
        id : controls
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

        onFirstButtonClicked : {
          refresh_full_event_list();
          event_stack.currentIndex = 0
        }
        onSecondButtonClicked : {
          var likely_id = root.backend.nextID(SQLBackend.eventS);
          event_g.clear(likely_id);
          root.backend.update_event(event_g);
          eventsInSceneList.events.push(event_g.make());
          eventsInSceneList.events[eventsInSceneList.events.length - 1].assign(event_g);
          eventsInSceneList.model = eventsInSceneList.events;
          eventsInSceneList.currentIndex = eventsInSceneList.events.length - 1;
          root.backend.update_event_in_scene(currentScene, event_g)
        }
        onThirdButtonClicked : {
          if (!eventsInSceneList.events || eventsInSceneList.events.length < 2) {
            return
          }
          event_g.clear();
          event_g.assign(eventsInSceneList.events[eventsInSceneList.currentIndex]);
          root.backend.remove_event_from_scene(event_g, currentScene);
          var prevIndex = eventsInSceneList.currentIndex;
          refresh_event_list();
          eventsInSceneList.currentIndex = Math.max(0, prevIndex);
        }
      }
      ListView {
        id : eventsInSceneList
         property var events: []

        anchors {
          top : controls.bottom;
          bottom : parent.bottom;
          left : parent.left;
          right : parent.right
        }
        spacing : 5
        clip : true
        highlightFollowsCurrentItem : true
        highlightMoveDuration : 1
        highlight : Rectangle {
          color : '#1111110F'
          Layout.alignment : Qt.AlignTop
          Layout.fillWidth : true
          Layout.margins : 5
        }

        model : ListModel {}

        delegate : Rectangle {
          id : event
          color : 'transparent'
          border.color : "steelblue"
          height : event_title_text.height + event_value_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              eventsInSceneList.currentIndex = index

            }
            onDoubleClicked : {
              eventEdit.returnTo = 1;
              eventEdit.currentEvent = eventsInSceneList.events[eventsInSceneList.currentIndex];
              event_stack.currentIndex = 2;
            }
          }
          Label {
            id : event_title_label
            anchors.left : parent.left
            anchors.leftMargin : 5
            text : "Name: "
            width : 50
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.accentColor : Material.secondaryTextColor
          }
          Text {
            id : event_title_text
            anchors.left : event_title_label.right
            anchors.leftMargin : 5
            text : (eventsInSceneList.events[index]) ? eventsInSceneList.events[index].name : "Undefined"

            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }
          Label {
            id : event_value_label
            anchors.top : event_title_text.bottom
            anchors.left : parent.left
            anchors.leftMargin : 5
            font.weight : Font.Bold
            font.pointSize : 10
            width : 100
            text : "Description: "
            enabled : false
            color : enabled ? Material.accentColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          Text {
            id : event_value_text
            anchors.top : event_title_text.bottom
            anchors.left : event_value_label.right
            anchors.right : parent.right

            anchors.leftMargin : 2
            font.pointSize : 10
            text : (eventsInSceneList.events[index]) ? eventsInSceneList.events[index].description : "Undefined"
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : event_title_text;
              enabled : true
            }
            PropertyChanges {
              target : event_value_text;
              enabled : true
            }
            PropertyChanges {
              target : event_title_label;
              enabled : true
            }
            PropertyChanges {
              target : event_value_label;
              enabled : true
            }
          }

          onFocusChanged : {
            if (eventsInSceneList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }

        ScrollBar.vertical : ScrollBar {}

        Component.onCompleted : {}
      }
    }
    Rectangle {
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      border.color : "black"
      border.width : 1
      EventEditPane {
        id : eventEdit
        anchors.fill : parent
        anchors.margins : 2
        backend : root.backend
        onExit : {
          event_stack.currentIndex = eventEdit.returnTo
        }

      }
    }

  }
  onCurrentSceneChanged : {
    refresh_event_list()
  }

  onBackendChanged : {
    if (backend) {
      backend.eventsChanged.connect(refresh_event_list)
      backend.eventMapsChanged.connect(refresh_event_list)
    }
  }
  //   onBackendChanged : {
  //   if (backend) {
  //     backend.citationsChanged.connect(refresh_citations);
  //     backend.equipmentChanged.connect(refresh_equipment);
  //   }
  // }
}
