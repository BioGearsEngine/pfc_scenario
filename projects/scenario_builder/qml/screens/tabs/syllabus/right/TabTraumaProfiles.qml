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
    id : trauma_g
  }
  TraumaOccurence {
    id : occurence_g
  }

  function update_trauma_profile(traumaProfile) {
    if (traumaProfile.physiologyTree.length == 0) {
      traumaProfile.physiologyTree.push("StandardMale@0s.xml");
    }
    root.backend.update_trauma_profile(traumaProfile)
  }

  function refresh_traumas() {
    occurenceStack.occurences = []
    let traumas = currentProfile.traumas;
    for (var ii = 0; ii < traumas.length; ++ ii) {
      occurenceStack.occurences.push(occurence_g.make());
      occurenceStack.occurences[occurenceStack.occurences.length - 1].assign(traumas[ii]);
    }
    occurenceList.model = occurenceStack.occurences;
  }
  function refresh_all_traumas() {
    occurenceStack.allTraumas = [];
    let traumas = root.backend.traumas;
    for (var ii = 0; ii < traumas.length; ++ ii) {
      occurenceStack.allTraumas.push(trauma_g.make());
      occurenceStack.allTraumas[occurenceStack.allTraumas.length - 1].assign(traumas[ii]);
    }
    fullTraumaList.model = occurenceStack.allTraumas;
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
        update_trauma_profile(currentProfile);
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
        update_trauma_profile(currentProfile);
      }
    }
  }

  StackLayout {
    id : occurenceStack
    Layout.fillWidth : true
    Layout.fillHeight : false
    Layout.leftMargin : 5
    Layout.rightMargin : 20

    property var occurences: []
    property var allTraumas: []

    currentIndex : 0
    CrossReferenceTraumaOccurences {
      id : occurenceList
      Layout.leftMargin : 5
      backend : root.backend

      onList : {
        occurenceStack.currentIndex = 1;
        refresh_all_traumas()
      }
      onTraumaAdded : {
        currentProfile.traumas.push(occurence_g.make());
        currentProfile.traumas[currentProfile.traumas.length - 1].trauma.assign(traumaAdded);
        update_trauma_profile(currentProfile);
        refresh_traumas()
      }
      onTraumaRemoved : {
        currentProfile.RemoveTrauma(index);
        update_trauma_profile(currentProfile);
        refresh_traumas()
      }
      onTraumaModified : {
        currentProfile.traumas[index].assign(occurence);
        update_trauma_profile(currentProfile);
        refresh_traumas();
      }
    }
    ListOfAllTraumas {
      id : fullTraumaList
      backend : root.backend

      onTraumaCreated : {
        refresh_all_traumas()
      }
      onTraumaAdded : {
        currentProfile.traumas.push(occurence_g.make());
        currentProfile.traumas[currentProfile.traumas.length - 1].trauma = trauma;
        update_trauma_profile(currentProfile);
        refresh_traumas();
        occurenceStack.currentIndex = 0;
      }
      onFullExit : {
        refresh_traumas();
        occurenceStack.currentIndex = 0;
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
        update_trauma_profile(currentProfile);
      }
    }
  }
  onBackendChanged : {
    if (backend) {
      backend.traumaProfilesChanged.connect(refresh_traumas);
      backend.traumasChanged.connect(refresh_traumas);
      backend.equipmentChanged.connect(refresh_traumas);
    }
  }
}
