import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property Assessment currentAssessment
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true
  Layout.fillHeight : true

  Assessment {
    id : currentAssessment
  }

  Connections {
    target : backend

  }

  function update_assessment(assessment) {
    if (assessment) {
      root.backend.update_assessment(assessment)
    }
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : nameEntry
    label : "Name"
    placeholderText : "String Field (128 Characters)"
    text : (currentAssessment) ? currentAssessment.name : ""
    onEditingFinished : {
      if (text != currentAssessment.name) {
        currentAssessment.name = text;
        update_assessment(currentAssessment);
      }
    }
  }

  TextAreaEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5

    id : descriptionEntry
    label : "Description"
    required : true
    placeholderText : "Text Area (5-15 Lines)"
    text : (currentAssessment) ? currentAssessment.description : ""
    onEditingFinished : {
      if (text != currentAssessment.description) {
        currentAssessment.description = text;
        update_assessment(currentAssessment);
      }
    }
  }
  RowLayout {
    Layout.leftMargin : 5
    Label {
      text : "Assessment Type :"
    }
    ComboBox {
      id : typeSelect
      width : 200
      model : ["Pass/Fail", "Partial"]

      contentItem : Text {
        text : typeSelect.displayText
        font.pointSize : 8
        verticalAlignment : Text.AlignVCenter
        elide : Text.ElideRight
      }
      onActivated : {
        optionalArea.currentIndex = index;
        if (root.model.count == 0) {
          return
        }
        var entry = root.model.get(root.index);
        entry.type = (index) ? "Scalar" : "Binary";
        update_assessment(entry);
      }
    }
  }

  StackLayout {
    id : optionalArea
    Layout.fillWidth : true
    Layout.fillHeight : false
    Layout.margins : 5
    Layout.rightMargin : 20
    Layout.preferredHeight : partialInput.height
    currentIndex : 0

    Rectangle {
      id : passFailInput
      Layout.fillWidth : true
      height : childrenRect.height + 10
      border.color : "steelblue"

      TextAreaEntry {
        anchors.top : parent.top
        anchors.left : parent.left
        anchors.right : parent.right
        anchors.margins : 5
        id : binaryCriteriaEntry
        label : "Criteria"
        placeholderText : "String Field (128 Characters)"
        text : (currentAssessment) ? currentAssessment.criteria : ""
        onEditingFinished : {
          partialCriteriaEntry.text = text;
          if (text != currentAssessment.criteria) {
            currentAssessment.criteria = text;
            update_assessment(currentAssessment);
          }
        }
      }
    }

    Rectangle {
      id : partialInput
      Layout.fillWidth : true
      height : childrenRect.height + 10
      border.color : "red"

      TextEntry {
        anchors.top : parent.top
        anchors.left : parent.left
        anchors.right : parent.right
        anchors.margins : 5

        id : partialPointsEntry
        label : "Total Points"
        placeholderText : "String Field (128 Characters)"
        text : (currentAssessment) ? currentAssessment.avaliablePoints : ""
        onEditingFinished : {
          if (text != currentAssessment.avaliablePoints) {
            currentAssessment.avaliablePoints = parseInt(text);
            update_assessment(currentAssessment);
          }
        }
      }

      TextAreaEntry {
        anchors.top : partialPointsEntry.bottom
        anchors.left : parent.left
        anchors.right : parent.right
        anchors.margins : 5
        id : partialCriteriaEntry
        label : "Criteria"
        placeholderText : "String Field (128 Characters)"
        text : (currentAssessment) ? currentAssessment.criteria : ""
        onEditingFinished : {
          binaryCriteriaEntry.text = text;
          if (text != currentAssessment.criteria) {
            currentAssessment.criteria = text;
            update_assessment(currentAssessment);
          }
        }
      }
    }
  }
}
