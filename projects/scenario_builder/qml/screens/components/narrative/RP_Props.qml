import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import "../common"

import com.ara.pfc.ScenarioModel.SQL 1.0

Rectangle {
	id: root
	property SQLBackend backend
	focus: true
	Layout.fillWidth: true
	Layout.fillHeight: true
	Layout.margins: 5
	border.color: 'black'
}