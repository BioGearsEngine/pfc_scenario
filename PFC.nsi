Outfile "pfc_scenario_installer.exe"
RequestExecutionLevel user
#In order to use this you need to:
#	install NSIS compiler https://nsis.sourceforge.io/Download
#	makensis PFC.nsi
#And then you're good to go, it'll spit out a file called pfc_scenario_installer.exe
InstallDir $APPDATA\PFC
Section "Install" 
	SetOutPath $INSTDIR
	File "README.md"
	File "build-msvc16\pfc_sb_working.sqlite"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Core.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Gui.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Network.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Qml.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Quick.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5QuickControls2.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5QuickTemplates2.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Sql.dll"
	File "D:\Qt\5.12.9\msvc2017_64\bin\Qt5Widgets.dll"
	File "build-msvc16\outputs\Release\bin\xerces-c_3_2.dll"
	File "C:\windows\system32\VCRUNTIME140.dll"
	File /r "build-msvc16\outputs\Release\bin\scenario_builder.exe"
	SetOutPath $INSTDIR\plugins
	File /r "D:\Qt\5.12.9\msvc2017_64\plugins\imageformats"
	File /r "D:\Qt\5.12.9\msvc2017_64\plugins\platforms"
	File /r "D:\Qt\5.12.9\msvc2017_64\plugins\platformthemes"
	File /r "D:\Qt\5.12.9\msvc2017_64\plugins\sqldrivers"
	SetOutPath $INSTDIR\qml
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\Qt"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\QtCharts"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\QtQml"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\QtQuick"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\QtQuick.2"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\QtWinExtras"
	File /r "D:\Qt\5.12.9\msvc2017_64\qml\builtins.qmltypes"
	#File "build-msvc16\outputs\Release\bin\unit_scenario_builder.exe"
	writeUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Desktop Shortcut" SHORTCUT
	SetOutPath "$INSTDIR"
	CreateShortcut "$DESKTOP\scenario_builder.lnk" "$INSTDIR\scenario_builder.exe" "" "$ICONDIR\${DESKICON}"
SectionEnd

Section "Uninstall"
	delete "$INSTDIR\uninstall.exe"
	RMDir /r "$INSTDIR"
SectionEnd 

# Change min required for events/roles in scene to 0
