import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel.SQL 1.0

ColumnLayout {
  id : root

  property SQLBackend backend
  property Author author

  signal editingFinished()

  Row {
    height : first_name.height
    spacing : 5
    Layout.leftMargin : 5
    Label {
      id : name_label
      text : 'Name'
      font.pointSize : 10
      color : "steelblue"
      width : 100
    }

    TextField {
      id : first_name
      maximumLength : 16
      placeholderText : qsTr('First')
      font.pointSize : 10
      text : author.first
      leftPadding : 5
      rightPadding : 5
      selectByMouse : true
      onEditingFinished : {
        if (text != author.first) {
          author.first = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }

    TextField {
      id : middle_name
      maximumLength : 16
      placeholderText : qsTr('Middle')
      font.pointSize : 10
      text : author.middle
      leftPadding : 5
      rightPadding : 5
      selectByMouse : true
      onEditingFinished : {
        if (text != author.first) {
          author.middle = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }

    TextField {
      id : last_name
      maximumLength : 16
      placeholderText : qsTr('Last')
      font.pointSize : 10
      text : author.last
      leftPadding : 5
      rightPadding : 5
      selectByMouse : true
      onEditingFinished : {
        if (text != author.last) {
          author.last = text
          root.backend.update_first_author(author)
        }
        root.editingFinished();
      }
    }
  }

  TextEntry {
    id : organization
    label : "Org"
    placeholderText : "Your Organization"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.organization
    onEditingFinished : {
      if (text != author.organization) {
        author.organization = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry {
    id : phone
    label : "Phone#"
    placeholderText : "(555) 555-5555"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.phone
    onEditingFinished : {
      if (text != author.phone) {
        author.phone = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry {
    id : email
    label : "Email"
    placeholderText : "name@organization"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.email
    onEditingFinished : {
      if (text != author.email) {
        author.email = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry {
    id : zipcode
    label : "Zipcode"
    placeholderText : "55555"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.zip
    onEditingFinished : {
      if (text != author.zip) {
        author.zip = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry {
    id : state
    label : "State"
    placeholderText : "Virginia"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.state
    onEditingFinished : {
      if (text != author.state) {
        author.state = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  TextEntry {
    id : country
    label : "Country"
    placeholderText : "United States"
    Layout.fillWidth : true
    Layout.alignment : Qt.AlignTop
    Layout.leftMargin : 5
    text : author.country
    onEditingFinished : {
      if (text != author.country) {
        author.country = text
        root.backend.update_first_author(author)
      }
      root.editingFinished();
    }
  }

  Component.onCompleted : {
    refresh_author()
  }
  Connections {
    target : backend
    onAuthorsChanged : {
      refresh_author()
    }
  }

  function refresh_author() {
    backend.select_author(author)
  }
}
