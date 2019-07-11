import QtQuick 2.10
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import com.ara.pfc.ScenarioModel 1.0
import com.ara.pfc.ScenarioModel.Locations 1.0

Page {
	id: root
	signal closed
	property var stack

	ColumnLayout {
		id: columns
		anchors.horizontalCenter: parent.horizontalCenter
		Rectangle {
			id: titleCard
			color: 'blue'
			radius: 10
			Layout.alignment: Qt.AlignHCenter
			Layout.fillWidth: true
			Layout.minimumHeight: 50
			Layout.minimumWidth: Font.contentWidth + 10
			Text {
				anchors.centerIn: parent
				text: 'SCENARIO TITLE'
			}
		}
		TabBar {
			id: tabs
			width: (parent.width / 2) - 10
			Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
			TabButton {
				text: "Summary"
				width: implicitWidth
			}
			TabButton {
				text: "Syllabus"
				width: implicitWidth
			}
			TabButton {
				text: "Narrative"
				width: implicitWidth
			}
		}
		Rectangle {
			Layout.alignment: Qt.AlignHCenter
			Layout.fillHeight: true
			Layout.fillWidth: true
			width: root.width - 20
			height: root.height - tabs.height - titleCard.height - menuBar.height - 40
			radius: 5
			color: 'grey'
			StackLayout {
				currentIndex: tabs.currentIndex
				width: parent.width
				height: parent.height - 10
				Row {
					id: summary
					Layout.alignment: Qt.AlignHCenter
					Layout.fillHeight: true
					Layout.fillWidth: true
					spacing: 10
					leftPadding: 5
					rightPadding: 5
					topPadding: 5
					width: parent.width
					height: parent.height
					Rectangle {
						id: summary_leftWindow
						width: (parent.width / 2) - 10
						height: parent.height
						clip: true
						color: 'red'
					    Row {
        					id: summary_leftWindow_title
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_leftWindow_vbar.width
							height: parent.height / 10
							y: (-summary_leftWindow_vbar.position * height)
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Title:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: title
					    	    	text: 'String Field (128 Characters)'
						        	maximumLength: 128
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
    					}
					    Row {
        					id: summary_leftWindow_version
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							anchors.top: summary_leftWindow_title.bottom
							width: parent.width - summary_leftWindow_vbar.width
							height: parent.height / 10
							y: (-summary_leftWindow_vbar.position * height)
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Version:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					     			id: version
					    	    	text: 'String Field (128 Characters)'
						        	maximumLength: 128
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
    					}
					    Row {
        					id: summary_leftWindow_security
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							anchors.top: summary_leftWindow_version.bottom
							width: parent.width - summary_leftWindow_vbar.width
							height: parent.height / 10
							y: (-summary_leftWindow_vbar.position * height)
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Security:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	color: 'pink'
						       	width: parent.width * (4 / 5) - 10
					        	height: 25					        	
						    	ComboBox {	
						    		id: control
						    		width: parent.width
						    		height: 25
					 				currentIndex: 0
					 				model: ["Unclassified","Classified","Secret","Top Secret"]
						 			background: Rectangle {
						 				color: 'pink'
						 				width: parent.width
						 				height: 25
						 				border.width: 0
						 				border.color: 'transparent'
						 				radius: 0
					 				}
								    contentItem: Text {
								        leftPadding: 0
								        rightPadding: control.indicator.width + control.spacing
								        text: control.displayText
								        font: titleCard.font
								        verticalAlignment: Text.AlignVCenter
								        horizontalAlignment: Text.AlignHCenter
								    }
				    			}
				    		}					    	
    					}	
    					Row {
    						id: summary_leftWindow_purpose
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							anchors.top: summary_leftWindow_security.bottom
							width: parent.width - summary_leftWindow_vbar.width
							height: parent.height / 3
							y: (-summary_leftWindow_vbar.position * height)
							Rectangle {
								width: parent.width / 5 - 10
								height: 25
								color: 'orange'
								Text {
									text: 'Purpose:'
									anchors.centerIn: parent
								}
							}
							Rectangle {
								color: 'pink'
								width: parent.width * (4 / 5) - 10
								height: parent.height
								Row {
									width: parent.width
									height: parent.height
									TextEdit { //TODO: Make this scrollable
										width: parent.width - 20
										height: parent.height
										anchors.margins: 5
										//focus: true
									}
									Rectangle {
										color: 'green'
										width: 20
										height: parent.height
										Text {
											text: 'Expand'
											anchors.centerIn: parent
											rotation: 90
										}
									}
								}
							}
    					}
    					Row {
    						id: summary_leftWindow_restrictions
   							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 20
							anchors.top: summary_leftWindow_purpose.bottom
							width: parent.width - summary_leftWindow_vbar.width
							height: parent.height / 3	
							y: (-summary_leftWindow_vbar.position * height)
							Rectangle {
								width: parent.width / 5 - 10
								height: 25
								color: 'orange'
								Text {
									text: 'Restrictions:'
									anchors.centerIn: parent
								}
							}
							Row {
								width: parent.width * (4 / 5) - 10
								height: parent.height
								Rectangle {
									color: 'pink'
									width: parent.width - 20
									height: parent.height
									Row {
										id: restrictions_buttons
										width: parent.width
										height: 25
										Rectangle {
											width: parent.width / 2 - 20
											height: parent.height
											anchors.left: parent.left
											color: 'lightblue'
											Text {
												text: 'Add'
												anchors.centerIn: parent
											}
											MouseArea {
												anchors.fill: parent
												onClicked: {
													restrictions_model.insert(1,{restriction : restriction_text.text})
													restriction_text.text = 'String Field'
												}
											}
										}
										Rectangle {
											width: parent.width / 2 - 20
											height: parent.height
											anchors.right: parent.right
											color: 'plum'
											Text {
												text: 'Remove'
												anchors.centerIn: parent
											}
											MouseArea {
												anchors.fill: parent
												onClicked: {
													restrictions_model.remove(1)
												}
											}
										}
									}
									Rectangle {
										color: 'lightgreen'
										width: parent.width
										height: 25
										anchors.top: restrictions_buttons.bottom
										TextInput {
											id: restriction_text
											text: 'String Field (64 Characters)'
											maximumLength: 64
											cursorVisible: false
											anchors.centerIn: parent
										}
									}
									ListModel {
										id: restrictions_model
										ListElement { restriction: '' }
									}
									Component {
										id: restriction_list_entry
										Item {
											width: parent.width
											height: 25
											Label {
												text: restriction
											} 
										}
									}
									ListView {
										id: restrictions_list
										height: parent.height - restrictions_buttons.height
										model: restrictions_model
										delegate: restriction_list_entry
										anchors.top: restrictions_buttons.bottom
									}
								}
								Rectangle {
									color: 'green'
									width: 20
									height: parent.height
									Text {
										text: 'Expand'
										anchors.centerIn: parent
										rotation: 90
									}
								}
							}				
    					}
						ScrollBar {
							id: summary_leftWindow_vbar
							hoverEnabled: true
							active: hovered || pressed
							orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
							size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
							anchors.top: parent.top
							anchors.right: parent.right
							anchors.bottom: parent.bottom
						}
					}
					Rectangle {
						id: summary_rightWindow
						clip: true
						width: (parent.width / 2) - 10
						height: parent.height
						color: 'blue'
						Row {
        					id: summary_rightWindow_author
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Author:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: author
					    	    	text: 'String Field (128 Characters)'
						        	maximumLength: 128
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
    					}
						Row {
							id: summary_rightWindow_org
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
							anchors.top: summary_rightWindow_author.bottom
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Org:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: org
					    	    	text: 'String Field (64 Characters)'
						        	maximumLength: 64
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
						}
						Row {
							id: summary_rightWindow_phoneNumber
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
							anchors.top: summary_rightWindow_org.bottom
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Phone#:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: phoneNumber
					    	    	text: 'String Field (12 Characters)'
						        	maximumLength: 12
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
						}
						Row {
							id: summary_rightWindow_email
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
							anchors.top: summary_rightWindow_phoneNumber.bottom
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Email:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: email
					    	    	text: 'String Field (64 Characters)'
						        	maximumLength: 64
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
						}
						Row {
							id: summary_rightWindow_keyword
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
							anchors.top: summary_rightWindow_email.bottom
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Keyword:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: keyword
					    	    	text: 'String Field (64 Characters)'
						        	maximumLength: 64
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
						}
						Row {
							id: summary_rightWindow_applicationDomain
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							topPadding: 5
							width: parent.width  - summary_rightWindow_vbar.width
							height: parent.height / 10
							y: (-summary_rightWindow_vbar.position * height)
							anchors.top: summary_rightWindow_keyword.bottom
					        Rectangle {
					        	width: parent.width / 5 - 10
					        	height: 25
					        	color: 'orange'
					        	Text {
					        		text: 'Application Domain:'
					        		anchors.centerIn: parent
					        	}
					        }
					        Rectangle {
					        	width: parent.width * (4 / 5) - 10
					        	height: 25
					        	color: 'pink'
					        	TextInput {
					        		id: applicationDomain
					    	    	text: 'String Field (64 Characters)'
						        	maximumLength: 64
						        	cursorVisible: false
						        	anchors.centerIn: parent
						        }
					    	}
						}
						Row {
							id: summary_rightWindow_useLimitations
							spacing: 10
							leftPadding: 5
							rightPadding: 5
							width: parent.width - summary_leftWindow_vbar.width
							height: parent.height / 3
							y: (-summary_leftWindow_vbar.position * height)
							anchors.top: summary_rightWindow_applicationDomain.bottom
							Rectangle {
								width: parent.width / 5 - 10
								height: 25
								color: 'orange'
								Text {
									text: 'Use Limitations:'
									anchors.centerIn: parent
								}
							}
							Rectangle {
								color: 'pink'
								width: parent.width * (4 / 5) - 10
								height: parent.height
								Row {
									width: parent.width
									height: parent.height
									TextEdit { //TODO: Make this scrollable
										width: parent.width - 20
										height: parent.height
										anchors.margins: 5
										//focus: true
									}
									Rectangle {
										color: 'green'
										width: 20
										height: parent.height
										Text {
											text: 'Expand'
											anchors.centerIn: parent
											rotation: 90
										}
									}
								}
							}
						}
						ScrollBar {
							id: summary_rightWindow_vbar
							hoverEnabled: true
							active: hovered || pressed
							orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
							size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
							anchors.top: parent.top
							anchors.right: parent.right
							anchors.bottom: parent.bottom
						}
					}
				}
				Row {
					id: syllabus
					Layout.alignment: Qt.AlignHCenter
					Layout.fillHeight: true
					Layout.fillWidth: true
					spacing: 10
					leftPadding: 5
					rightPadding: 5
					topPadding: 5
					width: parent.width
					height: parent.height
					Rectangle {
						width: parent.width / 2 - 10
						height: parent.height
						id: syllabus_leftWindow
						TabBar {
							id: syllabus_tabs
							width: parent.width
							Layout.alignment: Qt.AlignHCenter | Qt.AlignLeft
							TabButton {
								text: "Objectives"
								width: implicitWidth
							}
							TabButton {
								text: "Assessments"
								width: implicitWidth
							}
							TabButton {
								text: "Injuries"
								width: implicitWidth
							}
							TabButton {
								text: "Treatments"
								width: implicitWidth
							}
							TabButton {
								text: "Equipment"
								width: implicitWidth
							}
						}

						Row {
							id: syllabus_leftWindow_buttons
							width: parent.width
							height: 25
							anchors.top: syllabus_tabs.bottom
							Rectangle {
								color: 'green'
								width: parent.width / 4
								height: 25
								Text {
									text: 'New'
									anchors.centerIn: parent
								}
								MouseArea {
									anchors.fill: parent
									onClicked: {

									}
								}
							}
							Rectangle {
								color: 'orange'
								width: parent.width / 4
								height: 25
								Text {
									text: 'Move Up'
									anchors.centerIn: parent
								}
							}
							Rectangle {
								color: 'blue'
								width: parent.width / 4
								height: 25
								Text {
									text: 'Move Down'
									anchors.centerIn: parent
								}
							}
							Rectangle {
								color: 'plum'
								width: parent.width / 4
								height: 25
								Text {
									text: 'Delete'
									anchors.centerIn: parent
								}
								MouseArea {
									anchors.fill: parent
									onClicked: {
										
									}
								}
							}
						}
						Rectangle {
							width: parent.width
							height: parent.height  - syllabus_tabs.height - syllabus_leftWindow_buttons.height
							anchors.top: syllabus_leftWindow_buttons.bottom
							clip: true
							color: 'red'
							ScrollBar {
								id: syllabus_leftWindow_vbar
								hoverEnabled: true
								active: hovered || pressed
								orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
								size: 0.5 //parent.height / (summary_leftWindow_title.height + summary_leftWindow_version.height + summary_leftWindow_security + summary_leftWindow_purpose + summary_leftWindow_restrictions)
								anchors.top: parent.top
								anchors.right: parent.right
								anchors.bottom: parent.bottom
							}
							StackLayout {
								id: syllabus_stack_left
								currentIndex: syllabus_tabs.currentIndex
								///////////////////////////////////////////////////////////////////////////

								///////////////////////////////////////////////////////////////////////////
							}
						}
					}
					Rectangle {
						id: syllabus_rightWindow
						width: (parent.width / 2) - 10
						height: parent.height
						color: 'blue'
						StackLayout {
							id: syllabus_stack_right
							currentIndex: syllabus_tabs.currentIndex
							width: syllabus_rightWindow.width
							height: syllabus_rightWindow.height
							Layout.fillWidth: true
							Layout.fillHeight: true
							Rectangle {
								id: syllabus_rightWindow_objectives
								width: (syllabus_rightWindow.width / 2) - 10 
								height: syllabus_rightWindow.height
								anchors.centerIn: syllabus_rightWindow
								clip: true
								color: 'blue'
								Row {
									id: syllabus_rightWindow_objectives_medicalName
									spacing: 10
									leftPadding: 5
									rightPadding: 5
									topPadding: 5
									width: syllabus_rightWindow.width
									height: syllabus_rightWindow.height / 10
									Rectangle {
										color: 'orange'
										width: parent.width / 5 - 10
										height: 25
										Text {
											text: 'Medical Name:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
							        	width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
							        	height: 25
							        	color: 'pink'
							        	TextInput {
							        		id: medicalName
							    	    	text: 'String Field (128 Characters)'
								        	maximumLength: 128
								        	cursorVisible: false
								        	anchors.centerIn: parent
						        		}
									}
								}
								Row {
									id: syllabus_rightWindow_objectives_commonName
									spacing: 10
									leftPadding: 5
									rightPadding: 5
									width: syllabus_rightWindow.width
									height: syllabus_rightWindow.height / 10
									anchors.top: syllabus_rightWindow_objectives_medicalName.bottom
									Rectangle {
										color: 'orange'
										width: parent.width / 5 - 10
										height: 25
										Text {
											text: 'Common Name:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
							        	width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
							        	height: 25
							        	color: 'pink'
							        	TextInput {
							        		id: commonName
							    	    	text: 'String Field (128 Characters)'
								        	maximumLength: 128
								        	cursorVisible: false
								        	anchors.centerIn: parent
						        		}
									}
								}
								Row {
									id: syllabus_rightWindow_objectives_references
									spacing: 10
									leftPadding: 5
									rightPadding: 5
									width: parent.width
									height: parent.height / 3
									anchors.top: syllabus_rightWindow_objectives_commonName.bottom
									Rectangle {
										color: 'orange'
										width: parent.width / 5 - 10
										height: 25
										Text {
											text: 'References:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'pink'
										width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
										height: parent.height
										Row {
											width: parent.width
											height: parent.height
											Rectangle {
												width: parent.width - 25
												height: parent.height
												Row {
													spacing: 10
													leftPadding: 5
													rightPadding: 5
													topPadding: 5
													width: parent.width
													height: 25
													Rectangle { // Add
														color: 'lightblue'
														width: parent.width / 2 - 20
														height: 25
														Text {
															text: 'Add'
															anchors.centerIn: parent
														}
													}
													Rectangle { // Remove
														color: 'plum'
														width: parent.width / 2 - 20
														height: 25
														Text {
															text: 'Remove'
															anchors.centerIn: parent
														}
													}
												}
												ListView { // Reference List

												}
											}
											Rectangle { // Expander
												color: 'green'
												height: parent.height
												width: 25
												Text {
													text: 'Expander'
													rotation: 90
													anchors.centerIn: parent
												}
											}
										}
									}
								}
								Row {
									id: syllabus_rightWindow_objectives_description
									spacing: 10
									leftPadding: 5
									rightPadding: 5
									topPadding: 20
									width: parent.width
									height: parent.height / 3
									anchors.top: syllabus_rightWindow_objectives_references.bottom
									Rectangle {
										color: 'orange'
										width: parent.width / 5 - 10
										height: 25
										Text {
											text: 'Description:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'pink'
										width: syllabus_rightWindow.width * (4 / 5) - 10 - syllabus_rightWindow_objectives_vbar.width
										height: parent.height 
										Row {
											width: parent.width
											height: parent.height
											TextInput { // Text Area
												width: parent.width - 25
												height: parent.height
												Text {
													text: 'Text Area'
												}
											}
											Rectangle { // Expander
												color: 'green'
												width: 25
												height: parent.height
												Text {
													text: 'Expands'
													rotation: 90
													anchors.centerIn: parent
												}
											}
										}
									}
								}
								Row {
									id: syllabus_rightWindow_objectives_severityRange
									spacing: 10
									leftPadding: 5
									rightPadding: 5
									topPadding: 40
									width: parent.width
									height: parent.height / 3
									anchors.top: syllabus_rightWindow_objectives_description.bottom
									Rectangle {
										color: 'red'
										width: parent.width / 6
										height: 25
										Text {
											text: 'Severity Range:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'yellow'
										width: parent.width / 6
										height: 25
										Text {
											text: 'Min:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'lightblue'
										width: parent.width / 6
										height: 25
										Text {
											text: 'Number Spinner'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'green'
										width: parent.width / 6
										height: 25
										Text {
											text: 'Max:'
											anchors.centerIn: parent
										}
									}
									Rectangle {
										color: 'plum'
										width: parent.width / 6
										height: 25
										Text {
											text: 'Number Spinner'
											anchors.centerIn: parent
										}
									}
								}
								ScrollBar {
									id: syllabus_rightWindow_objectives_vbar
									hoverEnabled: true
									active: hovered || pressed
									orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
									size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
									anchors.top: syllabus_rightWindow_objectives.top
									anchors.right: syllabus_rightWindow_objectives.right
									anchors.bottom: syllabus_rightWindow_objectives.bottom
								}
							}
							Rectangle {
								id: syllabus_rightWindow_assessments
								width: (syllabus_rightWindow.width / 2) - 10
								height: syllabus_rightWindow.height
								clip: true
								color: 'blue'
								ScrollBar {
									id: syllabus_rightWindow_assessments_vbar
									hoverEnabled: true
									active: hovered || pressed
									orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
									size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
									anchors.top: syllabus_rightWindow_assessments.top
									anchors.right: syllabus_rightWindow_assessments.right
									anchors.bottom: syllabus_rightWindow_assessments.bottom
								}
							}
							Rectangle {
								id: syllabus_rightWindow_injuries
								width: (syllabus_rightWindow.width / 2) - 10
								height: syllabus_rightWindow.height
								clip: true
								color: 'blue'
								ScrollBar {
									id: syllabus_rightWindow_injuries_vbar
									hoverEnabled: true
									active: hovered || pressed
									orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
									size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
									anchors.top: syllabus_rightWindow_injuries.top
									anchors.right: syllabus_rightWindow_injuries.right
									anchors.bottom: syllabus_rightWindow_injuries.bottom
								}
							}
							Rectangle {
								id: syllabus_rightWindow_treatments
								width: (syllabus_rightWindow.width / 2) - 10
								height: syllabus_rightWindow.height
								clip: true
								color: 'blue'
								ScrollBar {
									id: syllabus_rightWindow_treatments_vbar
									hoverEnabled: true
									active: hovered || pressed
									orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
									size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
									anchors.top: syllabus_rightWindow_treatments.top
									anchors.right: syllabus_rightWindow_treatments.right
									anchors.bottom: syllabus_rightWindow_treatments.bottom
								}
							}
							Rectangle {
								id: syllabus_rightWindow_equipment
								width: (syllabus_rightWindow.width / 2) - 10
								height: syllabus_rightWindow.height
								clip: true
								color: 'blue'
								ScrollBar {
									id: syllabus_rightWindow_equipment_vbar
									hoverEnabled: true
									active: hovered || pressed
									orientation: Qt.Vertical // the 25 is the sum of topPadding on title and restrictions
									size: 0.5 //parent.height / (syllabus_leftWindow_title.height + syllabus_leftWindow_version.height + syllabus_leftWindow_security + syllabus_leftWindow_purpose + syllabus_leftWindow_restrictions)
									anchors.top: syllabus_rightWindow_equipment.top
									anchors.right: syllabus_rightWindow_equipment.right
									anchors.bottom: syllabus_rightWindow_equipment.bottom
								}
							}
						}
					}
				}
				Row {
					id: narrative
					Layout.alignment: Qt.AlignHCenter
					Layout.fillHeight: true
					Layout.fillWidth: true
					width: parent.width - 10
					height: parent.height - 10
					Rectangle {
						id: narrative_leftWindow
						color: 'green'
					}
				}
			}
		}
	}

}