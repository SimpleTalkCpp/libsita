@REM #change current directory to this file
@%~d0
@cd %~dp0

cmake -B build/libsita-x64-windows .

@pause