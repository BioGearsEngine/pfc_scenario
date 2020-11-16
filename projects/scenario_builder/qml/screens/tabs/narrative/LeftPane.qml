import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Scene currentScene : sceneList.scenes[sceneList.currentIndex]
  property Location currentLocation :  sceneList.locations[sceneList.currentIndex]
  Scene {
    id : scene_g
  }
  Location {
    id : location_g
    name: "location_g"
  }

  function update_scenes() {
    sceneList.scenes = []
    sceneList.locations = []
    let scenes = root.backend.scenes;
    for (var ii = 0; ii < scenes.length; ++ ii) {
      sceneList.scenes.push(scene_g.make());
      sceneList.locations.push(location_g.make());
      sceneList.scenes[sceneList.scenes.length - 1].assign(scenes[ii]);
      sceneList.locations[sceneList.locations.length - 1].assign(root.backend.getLocationOfScene(scenes[ii]));
    }
     sceneList.model = sceneList.scenes;
  }


  function seconds_to_clock_time(time_s) {

    var v_seconds = time_s % 60;
    var v_minutes = Math.floor(time_s / 60) % 60;
    var v_hours = Math.floor(time_s / 3600);
    v_seconds = (v_seconds < 10) ? "0%1".arg(v_seconds) : "%1".arg(v_seconds);
    v_minutes = (v_minutes < 10) ? "0%1".arg(v_minutes) : "%1".arg(v_minutes);
    v_hours = (v_hours < 10) ? "0%1".arg(v_hours) : "%1".arg(v_hours);
    return "%1:%2:%3".arg(v_hours).arg(v_minutes).arg(v_seconds);
  }
  
  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.margins : 5

    border.color : "black"

    TwoButtonRow {
      id : controls
      anchors.top : listRectangle.top
      anchors.left : listRectangle.left
      anchors.right : listRectangle.right
      anchors.topMargin : 2
      anchors.rightMargin : 5
      anchors.leftMargin : 5

      property int next : 1

      firstButtonText : "Add"
      secondButtonText : "Remove"

      onFirstButtonClicked : {
        var likely_id = root.backend.nextID(SQLBackend.SCENES);
        scene_g.clear(likely_id);
        root.backend.update_scene(scene_g);
      }
      onSecondButtonClicked : {
        if (!sceneList.scenes || sceneList.scenes.length < 2) {
          return
        }
        scene_g.clear();
        scene_g.assign(sceneList.scenes[sceneList.currentIndex]);
        root.backend.remove_scene(scene_g);
        sceneList.currentIndex = Math.max(0, sceneList.currentIndex - 1)
      }
    }

    ListView {
      id : sceneList
      property var scenes: []
      property var locations: []
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
        id : scene
        property var currentDef: (sceneList.scenes[index]) ? sceneList.scenes[index] : undefined
        property var currentLocationDef: (sceneList.locations[index]) ? sceneList.locations[index] : undefined
        color : 'transparent'
        border.color : "steelblue"
        height : scene_name_text.height + scene_location_label.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }
        MouseArea {
          anchors.fill : parent
          onClicked : {
            sceneList.currentIndex = index
          }
        }
        Label {
          id : scene_name_text
          text : (currentDef) ? currentDef.name : ""
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.accentColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }
        Label {
          id : scene_location_label
          text : "Location:"
          anchors.top : scene_name_text.bottom
          anchors.left : parent.left
          font.weight : Font.Bold
          anchors.leftMargin : 5
          font.pointSize : 10
          enabled : false
          color : enabled ?  Material.accentColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }
        Text {
          id : scene_location_text
          text : (currentLocationDef) ? currentLocationDef.name : "unknown"
          anchors.top : scene_name_text.bottom
          anchors.left : scene_location_label.right
          anchors.leftMargin : 5
          width : Math.max(contentWidth, 100)
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }
        Label {
          id : scene_time_label
          text : "Time:"
          anchors.top : scene_name_text.bottom
          anchors.left : scene_location_text.right
          anchors.leftMargin : 5
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ?  Material.accentColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }
        Text {
          id : scene_time_text
          text : (currentDef) ?  seconds_to_clock_time(currentDef.timeOfDay) : "unknown"
          anchors.top : scene_name_text.bottom
          anchors.left : scene_time_label.right
          anchors.right : parent.right
          anchors.leftMargin : 5
          width : Math.max(contentWidth, 100)
          font.weight : Font.Bold
          font.pointSize : 10
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges { target : scene_name_text; enabled : true }
          PropertyChanges { target : scene_location_label; enabled : true }
          PropertyChanges { target : scene_location_text; enabled : true }
          PropertyChanges { target : scene_time_label; enabled : true }
          PropertyChanges { target : scene_time_text; enabled : true }
        }

        onFocusChanged : {
          if (sceneList.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}

      onCurrentIndexChanged : {
      }
    }
  }
  Component.onCompleted : {
    update_scenes()
  }
  onBackendChanged : {
    if (backend) {
      backend.scenesChanged.connect(update_scenes)
      backend.locationsChanged.connect(update_scenes)
    }
  }
}
