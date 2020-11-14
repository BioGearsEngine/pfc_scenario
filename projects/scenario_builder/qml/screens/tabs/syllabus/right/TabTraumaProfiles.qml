import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property TraumaProfile currentProfile
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true
  Layout.fillHeight : true


  Trauma {
    id : trauma
  }
  function refresh_traumas() {}

  function update_traumaProfile(traumaProfile) {
    if (values.physiologyTree.length == 0) {
      values.physiologyTree.push("StandardMale@0s.xml");
    }
    root.backend.update_traumaProfile(traumaProfile)
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : nameEntry
    label : "Name"
    placeholderText : "String Field (128 Characters)"
    text : (currentProfile) ? currentProfile.name : ""
    onEditingFinished : {
      if (text != currentProfile.name) {
        currentProfile.name = text;
        update_traumaProfile(currentProfile);
      }
    }
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : descriptionEntry
    label : "Description"
    placeholderText : "String Field (128 Characters)"
    text : (currentProfile) ? currentProfile.description : ""
    onEditingFinished : {
      if (text != currentProfile.description) {
        currentProfile.description = text;
        update_traumaProfile(currentProfile);
      }
    }
  }

  StackLayout {
    id : listStack
    Layout.fillWidth : true
    Layout.fillHeight : false
    Layout.leftMargin : 5
    Layout.rightMargin : 20
    currentIndex : 0
    InjuryListEntry {
      id : traumaList
      Layout.leftMargin : 5
      backend : root.backend

      onList : {
        listStack.currentIndex = 1
      }

      onSeverityChanged : {}
      onLocationChanged : {}
      onInjuryAdded : {}
      onInjuryRemoved : {}
    }
    FullInjuryListEntry {
      id : fullInjuryList
      backend : root.backend

      onFullAdded : {}

      onFullExit : {
        listStack.currentIndex = 0
      }
    }
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : physiologyEntry
    label : "Physiology File"
    placeholderText : "String Field (128 Characters)"
    text : (currentProfile && currentProfile.physiologyTree.length > 1) ? currentProfile.physiologyTree[0] : ""
    onEditingFinished : {
      if (currentProfile.physiologyTree.length == 0) {
        currentProfile.physiologyTree.push("")
      }
      if (text != currentProfile.physiologyTree[0]) {
        currentProfile.physiologyTree[0] = text;
        update_traumaProfile(currentProfile);
      }
    }
  }
}
