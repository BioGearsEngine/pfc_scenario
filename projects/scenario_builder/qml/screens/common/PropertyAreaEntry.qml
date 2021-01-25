import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

TextAreaEntry {
  id : root

  property SQLBackend backend
  property alias name  : self.name
  property alias label : root.label
  property alias default_value : root.placeholderText
  property alias text : root.text

  Property {
    id : self
    name : "scenario_purpose"
  }
  onEditingFinished : {
    if ( text != self.value){
      self.value = text
      root.backend.update_property(self)
    }
  }

 onActiveFocusChanged : {
   if ( text != self.value){
      self.value = text
      root.backend.update_property(self)
    }
 }

 onCompleted : {
    root.backend.select_property(self);
    if(self.value === ""){
      self.value = root.placeholderText;
      if(required){
        root.backend.update_property(self)
        text = self.value;
      }
    } else {
      text = self.value;
    }
  }
}