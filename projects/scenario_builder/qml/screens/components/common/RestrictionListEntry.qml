import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

ListEntry {
  id: root
  property SQLBackend backend

  Restirction {
    id: self
    restriction_id : -1
  }

  model : ListModel{}

  delegate : Rectangle {
     id : delegateArea
     anchors { left : parent.left; right: parent.right ; margins : 5 }

     color : 'transparent'
     border.color: "steelblue"
     height : 30

     Text {
       id : restriction_title_text
       anchors.left : delegateArea.left
       anchors.leftMargin : 5
       text :  model.name + " " +  model.value
     }

     MouseArea {
       anchors.fill: parent
       onClicked: {
        root.current = index
       }
     }
  }

  onAdded : {
  console.log("Added Export Control " + index)
  self.restriction_id = -1
  self.name = "New Restriction %1".arg(root.model.count)
  self.value = "%1".arg(index)
  root.backend.update_restriction(self)
  root.model.insert(root.model.count,
    {"name": "%1".arg(self.name), "value": "%1".arg(self.value)});
  }
  onRemoved : {
    console.log("Removed Export Control " + index)
    self.restriction_id = -1
    self.name = root.model.get(index).name

    root.backend.remove_restriction(self)
    root.model.remove(index)
    current = math.Max(0,index-1)
  }

  onBackendChanged : {
    console.log("Backend Changed")
    var r_count = backend.restriction_count();
    root.backend.restrictions()
    while ( root.backend.next_restriction(self) ){
      console.log(self.name + " " + self.value)
      root.model.insert(root.model.count,
        {"name": "%1".arg(self.name), "value": "%1".arg(self.value)});
    }
  }
}
