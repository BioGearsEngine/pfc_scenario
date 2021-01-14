import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
  id : root
  property SQLBackend backend
  property int index
  width : parent.width
  height : parent.height
  border.color : "black"
  Property {
    id : property_g
  }
  Author {
    id : author_g
  }
  function generate_title() {
    var title = root.backend.getProperty("scenario_title");
    var ver = root.backend.getProperty("scenario_version");
    var classification = root.backend.getProperty("scenario_security");


    var result = " <p>";
    result += "<h1 align=\"center\" style='color:#003366;'>%1 ver:%2</h1>".arg(title).arg(ver);
    result += "<br><center style='color:#003366;'>%1</center>".arg(classification);
    result += "</p>";
    return result;
  }
  function generate_authorship() {
    author_g.clear();
    author_g.author_id = 1;
    root.backend.select_author(author_g);
    var description = root.backend.getProperty("scenario_description");

    var result = "<p>";
    result += "<b style='color:#003366;'>Author:</b>%1 %2<br>".arg(author_g.first).arg(author_g.last);
    result += "<b style='color:#003366;'>Org:</b>%1<br>".arg(author_g.organization);
    result += "<b style='color:#003366;'>Email:</b>%1<br>".arg(author_g.email);
    result += "<b style='color:#003366;'>Summary:</b>%1<br>".arg(description);
    result += "</p>";
    return result
  }

  function generate_works_cited() {
    var citations = root.backend.citations;
    var result = "<h2 style='color:#003366;'>Works Cited</h2>";
    result += "<p>";
	result += "<ol>";
    for ( var ii = 0; ii < citations.length; ++ii){
	   result += "<li>";
       result += "%1".arg(citations[ii].title);
       var authors = citations[ii].authors;
       if ( authors.length > 50){
          var author_array = authors.split(',');
          authors = author_array[0] + ", et. al.";
       }
       result += ":%1".arg(authors);
       result += ";%1".arg(citations[ii].publisher);
       result += " [%1]".arg(citations[ii].year);
       result += "<br>";
	   result += "</li>";
    }
	result += "</ol>";
    result += "</p>";
    return result
  }
  
  function generate_objectives() {
	var objs = root.backend.objectives;
	var result = "<h2 style='color:#003366;'>Objectives</h2>";
	result += "<p>";
	for (var ii = 0; ii < objs.length; ++ii) {
		result += "<i>%1</i>".arg(objs[ii].name);
		result += "<br>";
		result += "<blockquote>";
		result += "	%1".arg(objs[ii].description);
		result += "</blockquote>";
		result += "<br>";
	}
	result += "</p>";
	return result
  }
  
  function generate_roles() {
	var roles = root.backend.roles;
	var result = "<h2 style='color:#003366;'>Roles</h2>";
	result += "<p>";
	for (var ii = 0; ii < roles.length; ++ii) {
		result += "<i>%1</i>".arg(roles[ii].name);
		result += "<br>";
		result += "<blockquote>";
		result += "	%1".arg(roles[ii].description);
		result += "</blockquote>";
		result += "<br>";
	}
	result += "</p>";
	return result
  }
  
  function generate_events() {
	var events = root.backend.events;
	var result = "<h2 style='color:#003366;'>Events</h2>";
	result += "<p>";
	for (var ii = 0; ii < events.length; ++ii) {
		result += "<i>%1</i>".arg(events[ii].name);
		result += "<br>";
		result += "<blockquote>";
		result += "	%1".arg(events[ii].description);
		result += "</blockquote>";
		result += "<br>";
	}
	result += "</p>";
	return result
  }
  
  function generate_trauma_profiles() {
	var trauma_profiles = root.backend.traumaProfiles;
	var result = "<h2 style='color:#003366;'>Trauma Profiles</h2>";
	result += "<p>";
	for (var ii = 0; ii < trauma_profiles.length; ++ii) {
		result += "<i>%1</i>".arg(trauma_profiles[ii].name);
		result += " (%1)".arg(trauma_profiles[ii].traumas.length);
		result += "<br>";
		result += "<ul>";
		for (var jj = 0; jj < trauma_profiles[ii].traumas.length; ++jj) {
			result += "<li>";
			result += "%1".arg(trauma_profiles[ii].traumas[jj].trauma.medicalName);
			result += "(%1, ".arg(trauma_profiles[ii].traumas[jj].location);
			result += "%1)".arg(trauma_profiles[ii].traumas[jj].severity);
			result += "</li>";
		}
		result += "</ul>";
		result += "<br>";
	}
	result += "</p>";
	return result
  }
  
  function generate_scene() {
	var scene = root.backend.scenes;
	var result = "<h2 style='color:#003366;'>Scene</h2>";
	result += "<p>";
	for (var ii = 0; ii < scene.length; ++ii) {
		result += "<b style='color:#003366;'>Name :</b> %1".arg(scene[ii].name);
		result += "<br>";
		result += "<b style='color:#003366;'>Description:</b> %1".arg(scene[ii].description);
		result += "<br>";
		result += "<b style='color:#003366;'>Equipment:</b> Scene %1".arg(ii+1);
		var equipmentMaps = root.backend.equipmentMaps;
		result += "<ul>";
		for (var jj = 0; jj < equipmentMaps.length; ++jj) {
		  if (equipmentMaps[jj].scene.id == ii.id) {
			result += "<li>%1</li>".arg(equipmentMaps[jj].equipment.name);
		  }
		}
		result += "</ul>";
	}
	result += "</p>";
	return result
  }
  
  function refresh_summary() {
    var summary = "BEGIN SUMMARY<br>";
    summary += generate_title();
    summary += generate_authorship();
	summary += generate_scene();
	summary += generate_roles();
	summary += generate_events();
	summary += generate_objectives();
	summary += generate_trauma_profiles();
	summary += generate_works_cited();
    summary += "<br>END SUMMARY";

    summaryText.text = summary;
  }

  ScrollView {
    anchors.fill : parent
    Layout.margins : 5

    TextArea {
      id : summaryText
      Layout.maximumWidth : 595
	  //Layout.fillWidth : true
      textFormat : TextEdit.RichText
      text : "Generating Summary"
	  wrapMode : TextEdit.WordWrap
    }
  }

  onIndexChanged : {
    refresh_summary()
  }
}
