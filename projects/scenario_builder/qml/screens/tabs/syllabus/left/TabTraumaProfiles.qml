import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property int topIndex
  property TraumaProfile currentProfile : (profileList.traumaProfiles[profileList.currentIndex]) ? profileList.traumaProfiles[profileList.currentIndex] : currentTraumaProfile

  TraumaProfile {
    id : currentTraumaProfile
  }

  function update_truamaProfiles() {
    profileList.traumaProfiles = []
    let traumaProfiles = root.backend.traumaProfiles;
    for (var ii = 0; ii < traumaProfiles.length; ++ ii) {
      profileList.traumaProfiles.push(currentTraumaProfile.make());
      profileList.traumaProfiles[profileList.traumaProfiles.length - 1].assign(traumaProfiles[ii]);
    }
    profileList.model = profileList.traumaProfiles;
  }

  Rectangle {
    id : listRectangle
    Layout.fillWidth : true
    Layout.fillHeight : true

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
        var likely_id = root.backend.nextID(SQLBackend.TRAUMA_PROFILES);
        currentTraumaProfile.clear(likely_id);
        root.backend.update_trauma_profile(currentTraumaProfile);
        profileList.traumaProfiles.push(currentTraumaProfile.make());
        profileList.traumaProfiles[profileList.traumaProfiles.length - 1].assign(currentTraumaProfile);
        profileList.model = profileList.traumaProfiles;
        profileList.currentIndex = profileList.traumaProfiles.length - 1
      }
      onSecondButtonClicked : {
        if (!profileList.traumaProfiles || profileList.traumaProfiles.length < 2) {
          return
        }
        currentTraumaProfile.clear();
        currentTraumaProfile.assign(profileList.traumaProfiles[profileList.currentIndex]);
        root.backend.remove_trauma_profile(currentTraumaProfile);
        update_truamaProfiles();
        profileList.currentIndex = Math.max(0, root.index - 1)
      }
    }

    ListView {
      id : profileList
      property var traumaProfiles

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
        id : traumaProfile
        color : 'transparent'
        border.color : "steelblue"
        height : traumaProfile_title_text.height + traumaProfile_value_text.height
        anchors {
          left : parent.left;
          right : parent.right;
          margins : 5
        }

        MouseArea {
          anchors.fill : parent
          onClicked : {
            profileList.currentIndex = index

          }
        }

        Text {
          id : traumaProfile_title_text
          anchors.left : traumaProfile.left
          anchors.leftMargin : 5
          text : (profileList.traumaProfiles[index]) ? profileList.traumaProfiles[index].name : ""
          width : 150
          font.weight : Font.Bold
          font.pointSize : 10
          wrapMode : Text.Wrap
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
        }
        // This won't get deleted just yet because we haven't decided on how this bar should look
        Text {
          id : traumaProfile_value_text
          anchors.left : traumaProfile_title_text.right
          anchors.right : parent.right
          anchors.leftMargin : 10
          font.pointSize : 10
          text : (profileList.traumaProfiles[index]) ? profileList.traumaProfiles[index].description : ""
          enabled : false
          color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          elide : Text.ElideRight
        }

        states : State {
          name : "Selected"
          PropertyChanges {
            target : traumaProfile_title_text;
            enabled : true
          }
          // PropertyChanges{ target : traumaProfile_value_text; enabled  : true}
        }

        onFocusChanged : {
          if (profileList.currentIndex == index) {
            state = 'Selected';
          } else {
            state = '';
          }
        }
      }

      ScrollBar.vertical : ScrollBar {}
    }
  }

  Component.onCompleted : {
    update_truamaProfiles()
  }

  onBackendChanged : {
    if (backend) {
      backend.traumaProfilesChanged.connect(update_truamaProfiles)
    }
  }
}
