import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout  {
    id: root
    property SQLBackend backend
    property ListModel model
    property int index

    Layout.fillWidth: true
    Layout.fillHeight: true

    Assessment {
      id : obj
    }

    Connections {
      target : backend

    }
    

    function update_assessment(values) 
    {
      obj.assessment_id = values.id
      obj.name         =  values.name
      obj.description  =  values.description
      obj.type         =  values.type
      obj.available_points = values.available_points
      obj.criteria         = values.criteria
      root.backend.update_assessment(obj)
    }

    TextEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5
      id: nameEntry
      label : "Name"
      placeholderText: "String Field (128 Characters)"

       onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.name){
          entry.name = text
          update_assessment(entry)
        }
      }
    }

    TextAreaEntry {
      Layout.fillWidth: true
      Layout.leftMargin: 5

      id: descriptionEntry
      label : "Description"
      required: true
      placeholderText: "Text Area (5-15 Lines)"

      onEditingFinished : {
        var entry = model.get(root.index)
        if ( text != entry.description){
          entry.description = text
          update_assessment(entry)
        }
      }
    }
    RowLayout {
      Layout.leftMargin: 5
      Label {
        text : "Assessment Type :"
      }
      ComboBox {
          id : typeSelect
          width: 200
          model: [ "Pass/Fail", "Partial"]

          contentItem: Text {

              text: typeSelect.displayText
              font.pointSize: 8
              verticalAlignment: Text.AlignVCenter
              elide: Text.ElideRight
          }
          onActivated: {
              optionalArea.currentIndex = index
              if (root.model.count == 0) {
                return
              }
              var entry = root.model.get(root.index)
              entry.type = (index) ? "Scalar" : "Binary"
              update_assessment(entry)
          }
      }
    }

    StackLayout {
        id : optionalArea
        Layout.fillWidth: true
        Layout.fillHeight: false

        Layout.margins: 5
        Layout.preferredHeight : partialInput.height
        currentIndex : 0

        Rectangle {
          id: passFailInput
          Layout.fillWidth: true
          height : childrenRect.height + 10
          border.color  : "steelblue"

          TextAreaEntry {
            anchors.top: parent.top
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.margins : 5
            id: binaryCriteriaEntry
            label : "Criteria"
            placeholderText: "String Field (128 Characters)"

             onEditingFinished : {
              partialCriteriaEntry.text = text
              var entry = model.get(root.index)
              if ( text != entry.criteria){
                entry.criteria = text
                update_assessment(entry)
              }
            }
          }
        }

        Rectangle {
          id: partialInput
          Layout.fillWidth: true
          height : childrenRect.height + 10
          border.color  : "red"

          TextEntry {
            anchors.top : parent.top
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.margins : 5

            id: partialPointsEntry
            label : "Total Points"
            placeholderText: "String Field (128 Characters)"

             onEditingFinished : {
              var entry = model.get(root.index)
              if ( text != entry.available_points){
                entry.available_points = parseInt(text)
                update_assessment(entry)
              }
            }
          }

          TextAreaEntry {
            anchors.top: partialPointsEntry.bottom
            anchors.left : parent.left
            anchors.right : parent.right
            anchors.margins : 5
            id: partialCriteriaEntry
            label : "Criteria"
            placeholderText: "String Field (128 Characters)"

             onEditingFinished : {
              binaryCriteriaEntry.text = text
              var entry = model.get(root.index)
              if ( text != entry.criteria){
                entry.criteria = text
                update_assessment(entry)
              }
            }
          }
        }
    }

    onIndexChanged : {
      var values = model.get(index)
      if(values) {
        nameEntry.text = values.name
        descriptionEntry.text = values.description
        if (values.type === "Binary") {
          optionalArea.currentIndex =  0
          typeSelect.currentIndex =  0

        } else {
          optionalArea.currentIndex =  1
          typeSelect.currentIndex =  1
          partialPointsEntry.text   = "%1".arg(values.available_points)
        }
          partialCriteriaEntry.text = values.criteria
          binaryCriteriaEntry.text = values.criteria
      }
    }
}
