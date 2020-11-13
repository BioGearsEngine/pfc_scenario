import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import "../../../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root
  property SQLBackend backend
  property TraumaProfile currentProfile
  property int topIndex // topIndex is the index of the top set of 4 tabs

  Layout.fillWidth : true
  Layout.fillHeight : true

  TraumaProfile {
    id : obj
  }
  Trauma {
    id : trauma
  }
  function refresh_traumas() {
    var values = model.get(index)
    if (values) {
      nameEntry.text = values.name

      var injuries = (values.injuries) ? values.injuries.split(";").filter(x => x) : "";
      var locations = (values.locations) ? values.locations.split(";").filter(x => x) : "";
      var severities = (values.severities) ? values.severities.split(";").filter(x => x) : "";
      traumaList.model.clear()
      for (var i = 0; i < injuries.length; ++ i) {
        trauma.trauma_id = injuries[i];
        trauma.medical_name = "";
        trauma.common_name = "";
        root.backend.select_trauma(trauma);
        if (trauma.medical_name == "" && trauma.common_name == "") {
          continue
        }
        traumaList.model.insert(traumaList.model.count, {
          trauma_id: trauma.trauma_id,
          medical_name: "%1".arg(trauma.medical_name),
          common_name: "%1".arg(trauma.common_name),
          description: "%1".arg(trauma.description),
          citations: "%1".arg(trauma.citations),
          min: "%1".arg(trauma.min),
          max: "%1".arg(trauma.max),
          location: (locations.length) ? locations[i] : "",
          severity: (severities.length) ? severities[i] : 0.5
        });
      }
    }
  }

  function update_trauma_set(values) {
    obj.trauma_set_id = values.id;
    obj.name = values.name;
    obj.description = values.description
    obj.injuries = values.injuries;
    obj.locations = values.locations;
    obj.severities = values.severities;

    root.backend.update_trauma_set(obj)
  }

  TextEntry {
    Layout.fillWidth : true
    Layout.leftMargin : 5
    id : nameEntry
    label : "Name"

    onEditingFinished : {
      var entry = model.get(index);
      if (text != entry.name) {
        entry.name = text;
        update_trauma_set(entry);
      }
    }
  }

  StackLayout {
    id : listStack
    Layout.fillWidth : true
    Layout.fillHeight : true
    Layout.leftMargin : 5
    currentIndex : 0
    InjuryListEntry {
      id : traumaList
      Layout.fillWidth : true
      Layout.leftMargin : 5
      backend : root.backend

      onList : {
        var values = root.model.get(root.index);
        if (values) {
          fullInjuryList.model.clear();
          root.backend.injuries();
          while (root.backend.next_trauma(trauma)) {
            fullInjuryList.model.insert(fullInjuryList.model.count, {
              trauma_id: "%1".arg(trauma.trauma_id),
              medical_name: "%1".arg(trauma.medical_name),
              common_name: "%1".arg(trauma.common_name),
              description: "%1".arg(trauma.description),
              citations: "%1".arg(trauma.citations),
              min: "%1".arg(trauma.min),
              max: "%1".arg(trauma.max)
            });
          };
        }
        listStack.currentIndex = 1
      }

      onSeverityChanged : {
        var entry = root.model.get(root.index);
        var severities = entry.severities.split(";").filter(item => item);
        severities.splice(index, 1, severity);
        entry.severities = severities.join(";");
        update_trauma_set(entry)
      }

      onLocationChanged : {
        var entry = root.model.get(root.index)
        var locations = entry.locations.split(";").filter(item => item);
        locations.splice(index, 1, location);
        entry.locations = locations.join(";");
        update_trauma_set(entry)
      }

      onInjuryAdded : {
        var entry = root.model.get(root.index);
        entry.injuries = (entry.injuries) ? entry.injuries.concat(";" + trauma_id) : entry.injuries.concat(trauma_id);
        entry.severities = (entry.severities) ? entry.severities.concat(";" + severity) : entry.severities.concat(severity);
        entry.locations = (entry.locations) ? entry.locations.concat(";" + location) : entry.locations.concat(location);
        entry.description = (entry.description) ? entry.description.concat(";" + description) : entry.description.concat(description);
        update_trauma_set(entry);
      }

      onInjuryRemoved : {
        var entry = root.model.get(root.index);
        var injuries = entry.injuries.split(";").filter(item => item);
        var severities = entry.severities.split(";").filter(item => item);
        var locations = entry.locations.split(";").filter(item => item);
        var description = entry.description.split(";").filter(item => item);
        injuries.splice(index, 1);
        severities.splice(index, 1);
        locations.splice(index, 1);
        entry.injuries = injuries.join(";");
        entry.severities = severities.join(";");
        entry.locations = locations.join(";");
        entry.description = description.join(";");
        update_trauma_set(entry)
      }
    }
    FullInjuryListEntry {
      id : fullInjuryList
      Layout.fillWidth : true
      Layout.fillHeight : true
      backend : root.backend

      onFullAdded : {
        trauma.trauma_id = fullInjuryList.model.get(current).trauma_id;
        root.backend.select_trauma(trauma);
        var severity = trauma.severity;
        var location = trauma.location;
        var injuries = trauma.injuries;
        var description = trauma.description;
        var entry = root.model.get(root.index);
        entry.severities = (entry.severities) ? entry.severities.concat(";" + severity) : entry.severities.concat(severity);
        entry.locations = (entry.locations) ? entry.locations.concat(";" + location) : entry.locations.concat(location);
        entry.injuries = (entry.injuries) ? entry.injuries.concat(";" + trauma.trauma_id) : entry.injuries.concat(trauma.trauma_id);
        entry.description = (entry.description) ? entry.description.concat(";" + description) : entry.description.concat(description);
        update_trauma_set(entry);
        fullExit();
      }

      onFullExit : {
        var values = root.model.get(root.index);
        if (values) {
          nameEntry.text = values.name;
          traumaList.model.clear();
          var injuries = values.injuries.split(";").filter(x => x)
          var locations = (values.locations) ? values.locations.split(";").filter(x => x) : "";
          var severities = (values.severities) ? values.severities.split(";").filter(x => x) : "";
          for (var i = 0; i < injuries.length; ++ i) {
            trauma.trauma_id = injuries[i];
            trauma.medical_name = "";
            trauma.common_name = "";
            if (root.backend.select_trauma(trauma)) {
              traumaList.model.insert(traumaList.model.count, {
                trauma_id: trauma.trauma_id,
                medical_name: "%1".arg(trauma.medical_name),
                common_name: "%1".arg(trauma.common_name),
                description: "%1".arg(trauma.description),
                citations: "%1".arg(trauma.citations),
                min: "%1".arg(trauma.min),
                max: "%1".arg(trauma.max),
                location: (locations.length) ? locations[i] : "",
                severity: (severities.length) ? severities[i] : 0.5
              });
            }
          };
        }
        listStack.currentIndex = 0
      }
    }
  }
  onTopIndexChanged : {
    if (topIndex == 1) {
      refresh_traumas()
    }
  }
}
