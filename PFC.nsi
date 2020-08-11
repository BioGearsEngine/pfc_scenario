Outfile "pfc_scenario_installer.exe"
RequestExecutionLevel user
#In order to use this you need to:
#	install NSIS compiler https://nsis.sourceforge.io/Download
#	makensis PFC.nsi
#And then you're good to go, it'll spit out a file called pfc_scenario_installer.exe
InstallDir $APPDATA\PFC
Section "Install" 
	SetOutPath $INSTDIR
	!define QTDIR "D:\Qt\5.12.9\msvc2017_64"
	!define BUILDDIR "build-msvc16"
	File /nonfatal "README.md"
	File /nonfatal "${BUILDDIR}\pfc_sb_working.sqlite"
	File "${QTDIR}\bin\Qt5Core.dll"
	File "${QTDIR}\bin\Qt5Gui.dll"
	File "${QTDIR}\bin\Qt5Network.dll"
	File "${QTDIR}\bin\Qt5Qml.dll"
	File "${QTDIR}\bin\Qt5Quick.dll"
	File "${QTDIR}\bin\Qt5QuickControls2.dll"
	File "${QTDIR}\bin\Qt5QuickTemplates2.dll"
	File "${QTDIR}\bin\Qt5Sql.dll"
	File "${QTDIR}\bin\Qt5Widgets.dll"
	File "${BUILDDIR}\outputs\Release\bin\xerces-c_3_2.dll"
	File "C:\windows\system32\VCRUNTIME140.dll"
	File /r "${BUILDDIR}\outputs\Release\bin\scenario_builder.exe"
	SetOutPath $INSTDIR\plugins
	File /r "${QTDIR}\plugins\imageformats"
	File /r "${QTDIR}\plugins\platforms"
	File /r "${QTDIR}\plugins\platformthemes"
	File /r "${QTDIR}\plugins\sqldrivers"
	SetOutPath $INSTDIR\qml
	File /r "${QTDIR}\qml\Qt"
	File /r "${QTDIR}\qml\QtCharts"
	File /r "${QTDIR}\qml\QtQml"
	File /r "${QTDIR}\qml\QtQuick"
	File /r "${QTDIR}\qml\QtQuick.2"
	File /r "${QTDIR}\qml\QtWinExtras"
	File /r "${QTDIR}\qml\builtins.qmltypes"
	#File "${BUILDDIR}\outputs\Release\bin\unit_scenario_builder.exe"
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
