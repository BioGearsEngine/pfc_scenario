import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import Qt.labs.settings 1.1
import Qt.labs.platform 1.1 as Labs
import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0 as PFC

ScrollView {
  id : root
  property PFC.SQLBackend backend
  property PFC.Trauma currentTrauma
  property int topIndex // topIndex is the index of the top set of 4 tabs
  Layout.fillWidth : true
  Layout.fillHeight : true

  contentWidth : column.width
  contentHeight : column.height
  clip : true

  PFC.Citation {
    id : citation_g
  }

  ColumnLayout {
    id : column
    width : root.width
    spacing : 5
    GridLayout {
      id : grid
      Layout.fillWidth : true
      columns : 2
      TextEntry {
        Layout.minimumWidth : 2 * root.width / 3
        Layout.fillWidth : true
        Layout.leftMargin : 5
        Layout.column : 0
        Layout.row : 0

        id : medicalNameEntry
        label : "Medical Name"
        placeholderText : "String Field (128 Characters)"
        text : (currentTrauma) ? currentTrauma.medicalName : ""
        onEditingFinished : {
          if (text != currentTrauma.medicalName) {
            currentTrauma.medicalName = text
            update_trauma(currentTrauma)
          }
        }
      }

      TextEntry {
        Layout.fillWidth : true
        Layout.leftMargin : 5
        Layout.column : 0
        Layout.row : 1
        id : commonNameEntry
        label : "Common Name"
        placeholderText : "String Field (128 Characters)"
        text : (currentTrauma) ? currentTrauma.commonName : ""
        onEditingFinished : {
          if (text != currentTrauma.commonName) {
            currentTrauma.commonName = text
            update_trauma(currentTrauma)
          }
        }
        onLabelWidthChanged : {
          medicalNameEntry.nameWidth = commonNameEntry.nameWidth
        }
      }

      Image {
        id : imageView
        Layout.alignment : Qt.AlignHCenter | Qt.AlignTop
        Layout.fillWidth : true
        Layout.fillHeight : false
        Layout.preferredHeight : 150
        Layout.leftMargin : 5
        Layout.column : 1
        Layout.row : 0
        Layout.rowSpan : 2
        fillMode : Image.PreserveAspectFit
        source : "qrc:/img/trauma_placeholder.png"
      }
      Button {
        Layout.alignment : Qt.AlignHCenter | Qt.AlignTop
        text : "Browse"
        Layout.column : 1
        Layout.row : 2
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
            currentTrauma.image.clear();
            currentTrauma.image.uri = browseDialog.fileUrls.toString();
            currentTrauma.image.cache(currentTrauma.uuid);
            update_trauma(currentTrauma);
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
    StackLayout {
      id : citationStack
      Layout.fillWidth : true
      Layout.fillHeight : false
      Layout.leftMargin : 5
      Layout.rightMargin : 20
      currentIndex : 0
      property var treatmentCitations: []
      property var allCitations: []

      CrossReferenceCitations {
        id : referenceList

        Layout.fillWidth : true
        backend : root.backend
        onList : {
          citationStack.currentIndex = 1;
          refresh_all_citations()
        }
        onCitationAdded : {
          currentTrauma.citations.push(citation_g.make());
          currentTrauma.citations[currentTrauma.citations.length - 1].assign(citation);
          update_trauma(currentTrauma);
          refresh_citations()
        }
        onCitationRemoved : {
          currentTrauma.removeCitation(index);
          refresh_citations()
        }
      }
      ListOfAllCitations {
        id : fullReferenceList
        Layout.fillWidth : true
        backend : root.backend
        onCitationCreated : {
          refresh_all_citations()
        }
        onCitationAdded : {
          currentTrauma.citations.push(citation_g.make());
          currentTrauma.citations[currentTrauma.citations.length - 1].assign(citation);
          update_trauma(currentTrauma);
          refresh_citations();
          citationStack.currentIndex = 0;
        }
        onFullExit : {
          refresh_citations();
          citationStack.currentIndex = 0;
        }
      }
    }

    TextAreaEntry {
      Layout.leftMargin : 5

      id : descriptionEntry
      label : "Description"
      required : true
      placeholderText : "Text Area (5-15 Lines)"
      text : (currentTrauma) ? currentTrauma.description : ""
      onEditingFinished : {
        if (text != currentTrauma.description) {
          currentTrauma.description = text;
          update_trauma(currentTrauma);
        }
      }
    }

    RangeEntry {
      Layout.fillWidth : true
      Layout.leftMargin : 5

      id : severityEntry
      label : "Severity Range"
      min : (currentTrauma) ? currentTrauma.min : 0
      max : (currentTrauma) ? currentTrauma.max : 1
      onRangeModified : {
        currentTrauma.min = min;
        currentTrauma.max = max;
        update_trauma(currentTrauma)
      }
    }
  }
  onCurrentTraumaChanged : {
    refresh_citations()
  }

  Connections {
    target : backend
  }

  function update_trauma(citation) {
    if (citation) {
      root.backend.update_trauma(citation)
    }
  }
  function refresh_citations() {
    citationStack.treatmentCitations = []
    let citations = currentTrauma.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.treatmentCitations.push(citation_g.make());
      citationStack.treatmentCitations[citationStack.treatmentCitations.length - 1].assign(citations[ii]);
    }
    referenceList.model = citationStack.treatmentCitations;
  }
  function refresh_all_citations() {
    citationStack.allCitations = [];
    let citations = root.backend.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationStack.allCitations.push(citation_g.make());
      citationStack.allCitations[citationStack.allCitations.length - 1].assign(citations[ii]);
    }
    fullReferenceList.model = citationStack.allCitations;
  }
}
