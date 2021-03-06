@REM #change current directory to this file
@%~d0
@cd %~dp0

@rem mkdir _build/libsita-x64-windows/

cmake -B _build/libsita-x64-windows .

@pause