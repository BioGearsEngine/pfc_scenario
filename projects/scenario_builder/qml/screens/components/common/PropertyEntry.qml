import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0


Rectangle {
  id: root

  property SQLBackend backend
  property alias label : label.text
  property alias name: self.name
  property alias value: entryField.text
  property alias default_value: entryField.placeholderText
  property bool required : false

  signal editingFinished()

  Layout.preferredHeight : entryField.height
  Layout.rightMargin : 20

  Property {
    id : self
    name  : "common_property"
    value : ""
  }

  Label {
  id: label
   text: '(32 Characters):'
   font.pointSize : 10
   color: "steelblue"
   width: 100
  }

  TextField {
    id : entryField
    maximumLength: 128
    placeholderText: qsTr( 'String Field (128 Characters)')
    font.pointSize : 10
    anchors { left : label.right ; right : parent.right}
    leftPadding : 5
    rightPadding: 5
    selectByMouse : true

    onEditingFinished : {
      if ( entryField.text != self.value){
        self.value = entryField.text
        root.backend.update_property(self)
      }
      root.editingFinished();
    }

    Component.onCompleted : {
        root.backend.select_property(self);
        if(self.value === ""){
          self.value = root.default_value;
          if(root.required){
            root.backend.update_property(self)
            entryField.text = self.value;
          }
        } else {
          entryField.text = self.value;
        }
      }
  }

}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
