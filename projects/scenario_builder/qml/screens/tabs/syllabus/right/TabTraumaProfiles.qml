import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.1
import Qt.labs.platform 1.1 as Labs
import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0 as PFC

ColumnLayout {
  id : root
  property PFC.SQLBackend backend
  property PFC.TraumaProfile currentProfile
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true
  Layout.fillHeight : true

  PFC.Trauma {
    id : trauma_g
  }
  PFC.TraumaOccurence {
    id : occurence_g
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

  TextAreaEntry {
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
  RowLayout {
    id : physiologyEntryRow
    Layout.fillWidth : true
    Layout.leftMargin : 5

    TextEntry {
      Layout.preferredWidth : 4 * root.width / 5.0
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
    Button {
      id : physiologyButton
      text : "Browse"

      FileDialog {
        id : browseDialog
        title : "Please Choose a File:"
        visible : false
        selectMultiple : false
        selectExisting : true
        nameFilters : ["Images (*.jpg *.png *.bmp)", "All files (*)"]
        folder : Labs.StandardPaths.writableLocation(Labs.StandardPaths.PicturesLocation)
        onAccepted : {
          console.log("Selected %1".arg(browseDialog.fileUrls.toString()));
          currentProfile.image.clear();
          currentProfile.image.uri = browseDialog.fileUrls.toString();
          currentProfile.image.cache(currentProfile.uuid);
          update_trauma_profile(currentProfile);
        }
        onRejected : {
          console.log("Canceled")
        }
      }

      onClicked : {
        browseDialog.open()
      }
    }
  }

  onCurrentProfileChanged : {
    refresh_traumas()
  }

  Connections {
    target : backend
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
}
