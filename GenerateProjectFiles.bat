@ECHO OFF

ECHO.
ECHO ==========================================================
ECHO ======   Generating Bagual Engine Project Files... =======
ECHO ==========================================================
ECHO.

IF NOT EXIST "premake5.exe" (
	ECHO Downloading Premake5.zip ...
	POWERSHELL -Command "Invoke-WebRequest https://github.com/premake/premake-core/releases/download/v5.0.0-beta2/premake-5.0.0-beta2-windows.zip -OutFile premake-5.0.0-beta2-windows.zip"
	ECHO Extracting Premake5.exe ...
	POWERSHELL -Command "expand-archive -path 'premake-5.0.0-beta2-windows.zip' -destinationpath '.\'"
	ECHO Deleting cached zip ...
	DEL "premake-5.0.0-beta2-windows.zip"
	DEL example.*
	DEL luasocket.*
	DEL premake5.pdb
	ECHO.
)

IF EXIST "premake5.exe" (
	ECHO Running premake5 ...
	ECHO.
	CALL premake5.exe vs2022
	ECHO.
) ELSE (
	ECHO Could not run premake5.exe, please get it manually from https://github.com/premake/premake-core/releases/
	ECHO.
)

PAUSE
