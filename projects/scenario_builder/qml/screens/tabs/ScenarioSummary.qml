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
    result += "<h1 align=\"center\">%1 ver:%2</h1>".arg(title).arg(ver);
    result += "<br><center>%3</center>".arg(classification);
    result += "</p>";
    return result;
  }
  function generate_authorship() {
    author_g.clear();
    author_g.author_id = 1;
    root.backend.select_author(author_g);
    var description = root.backend.getProperty("scenario_description");

    var result = "<p>";
    result += "<b>Author:</b>%1 %2<br>".arg(author_g.firstName).arg(author_g.secondName);
    result += "<b>Org:</b>%1 %2<br>".arg(author_g.organization);
    result += "<b>email:</b>%1<br>".arg(author_g.email);
    result += "<b>summary:</b>%1<br>".arg(description);
    result += "</p>";
    return result
  }

  function geneate_works_cited() {
    var citations = root.backend.citations;
    var result = "<h2>WorksCited</h2>";
    result += "<p>";
    for ( var ii = 0; ii < citations.length; ++ii){
       
       result += "%1".arg(citations[ii].title)
       result += ":%2".arg(citations[ii].authors)
       result += ";%3".arg(citations[ii].publisher)
       result += " - [%4]".arg(citations[ii].year)
       result += "<br>"
    }
    result += "</p>";
    return result
  }
  function refresh_summary() {
    var summary = "BEGIN SUMMARY<br>";
    summary += generate_title();
    summary += generate_authorship();
    summary += geneate_works_cited();
    summary += "<br>END SUMMARY";

    summaryText.text = summary;
  }

  ScrollView {
    anchors.fill : parent
    Layout.margins : 5

    TextArea {
      id : summaryText
      Layout.fillWidth : true
      textFormat : TextEdit.RichText
      text : "Generating Summary"
    }
  }

  onIndexChanged : {
    refresh_summary()
  }
}
