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
  OutFile "Word Search Creator 1.2 Installer.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES64\Word Search Creator"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Word Search Creator" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel none

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "COPYING"
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Word Search Creator" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
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

  SetOutPath "$INSTDIR"
  
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

  SetOutPath "$INSTDIR\plugins\platforms"
  File "qwindows.dll"
  SetOutPath "$INSTDIR\plugins\styles"
  File "qwindowsvistastyle.dll"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Word Search Creator" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "DisplayName" "Word Search Creator 1.2"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator" "NoRepair" 1
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Word Search Creator.lnk" "$INSTDIR\Word Search Creator.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall Word Search Creator.lnk" "$INSTDIR\Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Word Search Creator Website.lnk" "http://wsc.sf.net"
  
  !insertmacro MUI_STARTMENU_WRITE_END

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

  RMDir "$INSTDIR\plugins\platforms"
  RMDir "$INSTDIR\plugins\styles"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Word Search Creator.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall Word Search Creator.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Word Search Creator Website.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey /ifempty HKCU "Software\Word Search Creator"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Word Search Creator"
  DeleteRegKey /ifempty HKCU "Software\Word Search Creator"


  ${unregisterExtension} ".wordsearch" "Word Search Worksheet File"
  ${unregisterExtension} ".wsh" "Word Search Worksheet File"

  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'

SectionEnd
