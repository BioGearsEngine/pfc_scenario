import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
    id: root
    property SQLBackend backend
    property string scenarioTitle
    property ListModel model
    property int index

  ColumnLayout {
      id: summary_leftWindow
      anchors.fill : parent

    PropertyEntry {
        id: propertyEntry
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      Layout.leftMargin: 5
      Layout.topMargin: 1
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
      Layout.alignment: Qt.AlignTop
      Layout.leftMargin: 5
      backend : root.backend
      name : "scenario_version"
      label : "Version"
      default_value : '0.0.1'
    }

    PropertyEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      Layout.leftMargin: 5
      backend : root.backend
      name : "scenario_security"
      label : "Security"
      default_value : 'UnClassified'
    }

    PropertyAreaEntry {
      Layout.fillWidth : true
      Layout.alignment: Qt.AlignTop
      Layout.leftMargin: 5
      backend : root.backend
      name : "scenario_description"
      label : "Description"
      default_value : 'Enter the scenarios description.'
      required : false
    }

    StackLayout {
      id : listStack
      Layout.fillWidth : true
      Layout.fillHeight : true
      Layout.leftMargin: 5
      currentIndex : 0
      RestrictionListEntry {
        id : restrictionList
        Layout.fillWidth : true
        Layout.fillHeight : true
        backend : root.backend  
        onList : {
          var values = root.model.get(root.index)
          if(values) {
            fullRestrictionList.model.clear()
            var restrictions = values.restrictions.split(";").filter(x=>x);
            root.backend.restrictions()
            while(root.backend.next_restriction(restriction)){
              fullRestrictionList.model.insert(fullRestrictionList.model.count,
                  {
                    restriction_id : "%1".arg(restriction.restriction_id),
                    name : "%1".arg(restriction.name),
                    value : "%1".arg(restriction.value)
                 }
               );
            };
          }
          listStack.currentIndex = 1
        }

        onRestrictionAdded : {
          var entry = root.model.get(root.index)
          entry.restrictions = (entry.restrictions) ? entry.restrictions.concat(";"+restriction_id) : entry.restrictions.concat(restriction_id)
          update_objective(entry)
        } 

        onRestrictionRemoved : {
          var entry = root.model.get(root.index)
          var restrictions = entry.restrictions.split(";").filter(item => item).filter(item => item != restriction_id);
          entry.restrictions = restrictions.join(";")
          update_objective(entry)
        }
      }
      FullRestrictionListEntry {
        id : fullRestrictionList
        Layout.fillWidth : true
        Layout.fillHeight : true
        backend : root.backend   

        onFullAdded : {
          restriction.id = fullRestrictionList.model.get(currentIndex)
          root.backend.select_restriction(restriction)
          var entry = root.model.get(root.index)
          entry.restrictions = (entry.restrictions) ? entry.restrictions.concat(";"+restriction_id) : entry.restrictions.concat(restriction_id)
          update_objective(entry)
        }

        onFullExit : {
          var values = root.model.get(root.index)
          if(values) {
            nameEntry.text = values.name
            restrictionList.model.clear()   

            var restrictions = values.restrictions.split(";").filter(x => x);  
            for(var i = 0; i < restrictions.length; ++i){
              restriction.restriction_id = restrictions[i]
              restriction.name = ""
              restriction.value = ""
              if(root.backend.select_restriction(restriction)){
                restrictionList.model.insert(restrictionList.model.count,
                    {
                       restriction_id : "%1".arg(restriction.restriction_id),
                       name : "%1".arg(restriction.name),
                       value : "%1".arg(restriction.value)
                   }
                 );
              }
            };
          }
          listStack.currentIndex = 0
        }    
      }
    }

    Rectangle {
        Layout.fillHeight: true
        color : "Red"
        border.color:"Red"
    }
  }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
