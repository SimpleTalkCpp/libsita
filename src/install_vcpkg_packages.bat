@REM #change current directory to this file
%~d0
cd %~dp0

git clone https://github.com/microsoft/vcpkg.git externals\vcpkg

call externals\vcpkg\bootstrap-vcpkg.bat

externals\vcpkg\vcpkg install fmt:x64-windows --recurse
externals\vcpkg\vcpkg install eastl:x64-windows --recurse

externals\vcpkg\vcpkg install imgui[sdl2-binding]:x64-windows --recurse
externals\vcpkg\vcpkg install imgui[opengl3-glew-binding]:x64-windows --recurse

@pause