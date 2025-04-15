@echo off
setlocal enabledelayedexpansion

:: Check if an argument was provided
if "%~1"=="" (
    echo Usage: %0 excludedUserName
    exit /b 1
)
set "excludedUser=%~1"


for /f "skip=1 tokens=1,2,3,4" %%a in ('qwinsta') do (
    set "sessionName=%%a"
    set "userName=%%b"
    set "sessionID=%%c"
    set "sessionState=%%d"

    
    set "userName=!userName: =!"
	set "sessionID=!sessionID: =!"
    :: Check if the session ID is not 65536 and the user is not excluded
    if /i not "!sessionID!"=="65536" (
        if /i not "!userName!"=="!excludedUser!" (
            if /i not "!userName!"=="services" (
                echo Logging off user !userName! with Session ID: !sessionID!
                logoff !sessionID! 2>nul
            )
        )
    )
)

endlocal
