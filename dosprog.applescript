#!/usr/bin/env osascript
tell application "Virtual ]["
	activate
	close every machine saving no
	make new AppleIIe
	set p to (path to me as text) & "::"
	set disk0 to (POSIX path of p & "dosprog.dsk")
	set disk1 to (POSIX path of p & "blank.dsk")
	tell front machine
		set speaker volume to 0.0
		set caps lock to true
		set monochrome screen to false
		set speed to maximum
		insert disk0 into device "S6D1"
		insert disk1 into device "S6D2"
		type line "BRUN TEST"
	end tell
end tell
