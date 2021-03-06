import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

import "../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property SQLBackend backend
  property string scenarioTitle
  property int index

  readonly property alias model : citationList.model

  Citation {
    id : citation
  }
  ColumnLayout {
    id : general_leftWindow
    anchors.fill : parent
    anchors.bottom : parent.bottom
    PropertyEntry {
      id : propertyEntry
      Layout.fillWidth : true
      Layout.alignment : Qt.AlignTop
      Layout.leftMargin : 5
      Layout.topMargin : 1
      backend : root.backend
      name : "scenario_title"
      label : "Title"
      default_value : 'New Scenario'
      onTextChanged : {
        root.scenarioTitle = text
      }
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment : Qt.AlignTop
      Layout.leftMargin : 5
      backend : root.backend
      name : "scenario_version"
      label : "Version"
      default_value : '0.0.1'
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment : Qt.AlignTop
      Layout.leftMargin : 5
      backend : root.backend
      name : "scenario_security"
      label : "Security"
      default_value : 'UnClassified'
    }

    PropertyAreaEntry {
      Layout.fillWidth : true
      Layout.alignment : Qt.AlignTop
      Layout.leftMargin : 5
      backend : root.backend
      name : "scenario_description"
      label : "Description"
      default_value : 'Enter the scenarios description.'
      required : false
    }
    Rectangle {
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.margins : 5
      // Layout.alignment : Qt.AlignTop
      border.color : "black"
      TwoButtonRow {
        id : controls
        anchors.top : parent.top
        anchors.left : parent.left
        anchors.right : parent.right
        anchors.topMargin : 2
        anchors.rightMargin : 5
        anchors.leftMargin : 5
        property int next : 1
        label : "Reference"

        onFirstButtonClicked : {
          if (next < citationList.citations.length) {
            next = citationList.citations.length + 1
          }
          
          var likely_id = root.backend.nextID(SQLBackend.CITATIONS) + 1;
          self.clear(likely_id);

          citation.uuid = "";
          root.backend.update_citation(citation);
          citationList.citations.push(citation.make());
          citationList.citations[citationList.citations.length - 1].assign(citation);
          citationList.model = citationList.citations;
          ++next;
        }
        onSecondButtonClicked : {
          if (citationList.model.count == 0) {
            return
          }
          citation.citation_id = -1;
          citation.key = citationList.citations[citationList.currentIndex].key;
          root.backend.remove_citation(citation);

          citationList.currentIndex = Math.max(0, citationList.currentIndex - 1);

        }
      }
      ListView {
        id : citationList
        property var citations : []
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


        delegate : Rectangle {
          id : citation
          color : 'transparent'
          border.color : "steelblue"
          height : citation_title_text.height + citation_value_text.height
          anchors {
            left : parent.left;
            right : parent.right;
            margins : 5
          }

          MouseArea {
            anchors.fill : parent
            onClicked : {
              citationList.currentIndex = index

            }
            onDoubleClicked : {}
          }

          Text {
            id : citation_title_text
            anchors.left : citation.left
            anchors.leftMargin : 5
            text : (citationList.citations[index]) ? citationList.citations[index].title : ""
            width : 150
            font.weight : Font.Bold
            font.pointSize : 10
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
          }

          Text {
            id : citation_value_text
            anchors.top : citation_title_text.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.leftMargin : 10
            font.pointSize : 10
            text : (citationList.citations[index]) ? "%1".arg(citationList.citations[index].key) : ""
            enabled : false
            color : enabled ? Material.primaryTextColor : Material.secondaryTextColor
            elide : Text.ElideRight
          }

          states : State {
            name : "Selected"
            PropertyChanges {
              target : citation_title_text;
              enabled : true
            }
            PropertyChanges {
              target : citation_value_text;
              enabled : true
            }
          }

          onFocusChanged : {
            if (citationList.currentIndex == index) {
              state = 'Selected';
            } else {
              state = '';
            }
          }
        }

        ScrollBar.vertical : ScrollBar {}

        Component.onCompleted : {
          citationList.citations = []
          let citations = root.backend.citations;
          for (var ii = 0; ii < citations.length; ++ ii) {
            citationList.citations.push(citation.make());
            citationList.citations[citationList.citations.length - 1].assign(citations[ii]);
          }
          citationList.model = citationList.citations;
        }
      }
    }
    Rectangle {
      Layout.fillHeight : true
      color : "Red"
      border.color : "Red"
    }
  }
  function rebuild_citations() {
    citationList.citations = []
    let citations = root.backend.citations;
    for (var ii = 0; ii < citations.length; ++ ii) {
      citationList.citations.push(citation.make());
      citationList.citations[citationList.citations.length - 1].assign(citations[ii]);
    }
    citationList.model = citationList.citations;
  }

  Component.onCompleted : {
    if (backend){
      rebuild_citations()
    }
  }
  onIndexChanged : {
    if (backend) {
      rebuild_citations()
    }
  }


  Connections {
    target : backend
    onCitationsChanged : {
      rebuild_citations()
    }
  }
}