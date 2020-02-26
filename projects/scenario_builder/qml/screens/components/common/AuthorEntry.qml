import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id: root

  property SQLBackend backend
  property Author  author

  signal editingFinished()
  
  Row{
    height: first_name.height
    spacing : 5
    Layout.leftMargin: 5
    Label{
      id: name_label
      text: 'Name'
      font.pointSize : 10
      color: "steelblue"
      width: 100
    }

    TextField {
      id : first_name
      maximumLength: 16
      placeholderText: qsTr( 'First')
      font.pointSize : 10
      
      leftPadding : 5
      rightPadding: 5
      onEditingFinished : {
        if ( text != author.first){
          author.first = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }

    TextField {
      id : middle_name
      maximumLength: 16
      placeholderText: qsTr( 'Middle')
      font.pointSize : 10
      
      leftPadding : 5
      rightPadding: 5
      onEditingFinished : {
        if ( text != author.first){
          author.middle = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }

    TextField {
      id : last_name
      maximumLength: 16
      placeholderText: qsTr( 'Last')
      font.pointSize : 10
      
      leftPadding : 5
      rightPadding: 5
      onEditingFinished : {
        if ( text != author.last){
          author.last = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }
  }

  TextEntry{
    id: organization
    label: "Org"
    placeholderText: "Your Organization"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
        onEditingFinished : {
      if ( text != author.organization){
        author.organization = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry{
    id: phone
    label: "Phone#"
    placeholderText: "(555) 555-5555"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
    onEditingFinished : {
      if ( text != author.phone){
        author.phone = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry{
    id: email
    label: "Email"
    placeholderText: "name@organization"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
    onEditingFinished : {
      if ( text != author.email){
        author.email = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
    Component.onCompleted : {
      root.backend.select_author(author);
      var update = false 

      if( author.name_first !== ""){
        first_name.text = author.first
      }
      if( author.name_last !== ""){
        last_name.text = author.last
      }
      if( author.email !== ""){
        email.text = author.email
      }
      if( author.phone !== ""){
        phone.text = author.phone
      }
      if( author.organiztion !== ""){
        organization.text = author.organization
      }
      if( author.zip !== ""){
        zipcode.text = author.zip
      }
      if( author.state !== ""){
        state.text = author.state
      }
      if( author.country !== ""){
        country.text = author.country
      }
    }
  }

  TextEntry{
    id: zipcode
    label: "Zipcode"
    placeholderText: "55555"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
    onEditingFinished : {
      if ( text != author.zip){
        author.zip = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry{
    id: state
    label: "State"
    placeholderText: "Virginia"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
    onEditingFinished : {
      if ( text != author.state){
        author.state = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry{
    id: country
    label: "Country"
    placeholderText: "United States"
    Layout.fillWidth : true
    Layout.alignment: Qt.AlignTop
    Layout.leftMargin: 5
    onEditingFinished : {
      if ( text != author.country){
        author.country = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

}