import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12


  import "../common"
  import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
    id: root
    property SQLBackend backend

  ColumnLayout {
    id: summary_leftWindow
    anchors.fill : parent

    Author {
      id: main_author
      author_id : 1
    }

    AuthorEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      author : main_author
      backend : root.backend
    }

    Rectangle {
        height : 25
        color : "Red"
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_keywords"
      label : "Keywords"
      default_value : 'Comma Delimited List of Keywords'
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_domain"
      label : "Domain"
      default_value : 'Medical Prolonged Field Care'
      required : true
    }

    PropertyAreaEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      backend : root.backend
      name : "scenario_limitations"
      label : "Use Limitations"
      default_value : 'Enter any Usage Limitations'
      required : false 
    }
    
    Rectangle {
        Layout.fillHeight: true
        color : "Red"
    }
  }
}