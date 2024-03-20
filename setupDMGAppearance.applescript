tell application "Finder"
	tell disk "WordSearchCreator"
		open
		
		set the bounds of container window to {100, 100, 650, 425}
		set current view of container window to icon view
		set toolbar visible of container window to false
		set statusbar visible of container window to false
		set arrangement of icon view options of container window to not arranged
		set background picture of icon view options of container window to file ".background:background.png"
		make new alias to POSIX file "/Applications" at POSIX file "/Volumes/WordSearchCreator/"
		set position of item "Word Search Creator.app" of container window to {145, 180}
		set position of item "Applications" of container window to {410, 180}
		
		close
	end tell
end tell