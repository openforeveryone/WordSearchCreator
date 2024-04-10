;Based on a script by Joost Verburg from http://nsis.sourceforge.net/Examples/Modern%20UI/

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "FileAssociation.nsh"

;--------------------------------
;Definitions

  !define SHCNE_ASSOCCHANGED 0x08000000
  !define SHCNF_IDLIST 0

;--------------------------------
;General

  ;Name and file
  Name "Word Search Creator"
  OutFile "Word Search Creator Installer.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\Word Search Creator"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\OpenForEveryone\Word Search Creator" "InstallDir"

  ;Request application privileges for Windows Vista
  RequestExecutionLevel none

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "COPYING"
  !insertmacro MUI_PAGE_DIRECTORY
  
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections
Section "Dummy Section" SecDummy

  ;remove files from old versions
  ;Get old installation folder (for versions prior to 1.2 which were 32bit) from registry
  Var /GLOBAL OLDINSTDIR
  Var /GLOBAL OldStartMenuFolder
  ReadRegStr $OLDINSTDIR HKCU "Software\Word Search Creator" ""
  ReadRegStr $OldStartMenuFolder HKCU "Software\Word Search Creator" "Start Menu Folder"
  ${If} ${Errors}
    DetailPrint "No old 32bit install found"
  ${Else}
    DetailPrint "Old 32bit install found at: $OLDINSTDIR, removing it."
    Delete "$OLDINSTDIR\Uninstall.exe"
    Delete "$OLDINSTDIR\COPYING"
    Delete "$OLDINSTDIR\Word Search Creator.exe"
    Delete "$OLDINSTDIR\libgcc_s_dw2-1.dll"
    Delete "$OLDINSTDIR\mingwm10.dll"
    Delete "$OLDINSTDIR\QtCore4.dll"
    Delete "$OLDINSTDIR\QtGui4.dll"
    Delete "$OLDINSTDIR\QtNetwork4.dll"
    Delete "$OLDINSTDIR\QtSvg4.dll"
    Delete "$OLDINSTDIR\QtXml4.dll"
    Delete "$OLDINSTDIR\Readme.txt"
    Delete "$OLDINSTDIR\Uninstall.exe"
    Delete "$OLDINSTDIR\URWGothicL-Book.ttf"
    RMDir "$OLDINSTDIR"
    Delete "$SMPROGRAMS\$OldStartMenuFolder\Word Search Creator.lnk"
    Delete "$SMPROGRAMS\$OldStartMenuFolder\Uninstall Word Search Creator.lnk"
    Delete "$SMPROGRAMS\$OldStartMenuFolder\Word Search Creator Website.lnk"
    Delete "$SMPROGRAMS\$OldStartMenuFolder\Readme.lnk"
    RMDir "$SMPROGRAMS\$StartMenuFolder"
    DeleteRegValue HKCU "Software\Word Search Creator" ""
    DeleteRegValue HKCU "Software\Word Search Creator" "Start Menu Folder"
    DeleteRegKey /ifempty HKCU "Software\Word Search Creator"
  ${EndIf}

  SetOutPath "$INSTDIR"

  SetDetailsView show
  
  File "COPYING"
  File "Word Search Creator.exe"
  File "Qt5Core.dll"
  File "Qt5Gui.dll"
  File "Qt5Network.dll"
  File "Qt5PrintSupport.dll"
  File "Qt5Svg.dll"
  File "Qt5Widgets.dll"
  File "Qt5Xml.dll"
  File "libgcc_s_seh-1.dll"
  File "libstdc++-6.dll"
  File "libwinpthread-1.dll"
  File "URWGothicL-Book.ttf"
  File "README.md"

  SetOutPath "$INSTDIR\plugins\platforms"
  File "qwindows.dll"
  SetOutPath "$INSTDIR\plugins\styles"
  File "qwindowsvistastyle.dll"
  
  ;Store installation folder
  WriteRegStr HKLM "Software\OpenForEveryone\Word Search Creator" "InstallDir" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "DisplayName" "Word Search Creator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "Publisher" "Matthew Wellings"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "DisplayIcon" "$INSTDIR\Word Search Creator.exe,0"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "Version" "1.2.1"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "DisplayVersion" "1.2.1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "EstimatedSize" 35738
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "NoRepair" 1
    
  ;Create shortcuts
  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\Word Search Creator.lnk" "$INSTDIR\Word Search Creator.exe"

  ${registerExtension}  "$INSTDIR\Word Search Creator.exe" ".wordsearch" "Word Search Worksheet File"
  ${registerExtension}  "$INSTDIR\Word Search Creator.exe" ".wsh" "Word Search Worksheet File"

  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "Word Search Creator."

  ;Assign language strings to sections
;  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
;  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\COPYING"
  Delete "$INSTDIR\Word Search Creator.exe"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Network.dll"
  Delete "$INSTDIR\Qt5PrintSupport.dll"
  Delete "$INSTDIR\Qt5Svg.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\Qt5Xml.dll"
  Delete "$INSTDIR\plugins\platforms\qwindows.dll"
  Delete "$INSTDIR\plugins\styles\qwindowsvistastyle.dll"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libwinpthread-1.dll"
  Delete "$INSTDIR\URWGothicL-Book.ttf"
  Delete "$INSTDIR\README.md"

  RMDir "$INSTDIR\plugins\platforms"
  RMDir "$INSTDIR\plugins\styles"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR"

  SetShellVarContext all
  Delete "$SMPROGRAMS\Word Search Creator.lnk"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator"
  DeleteRegValue HKLM "Software\OpenForEveryone\Word Search Creator" "InstallDir"
  DeleteRegKey /ifempty HKLM "Software\OpenForEveryone\Word Search Creator"
  DeleteRegKey /ifempty HKLM "Software\OpenForEveryone"

  ${unregisterExtension} ".wordsearch" "Word Search Worksheet File"
  ${unregisterExtension} ".wsh" "Word Search Worksheet File"

  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'

SectionEnd
