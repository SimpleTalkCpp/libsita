# How to Build 

## Windows
```
install_vcpkg_packages.bat
gen_vs2019.bat
```
- Open Visual Studio project in `build/libstia-x64-windows`

![[<img src="doc/screenshot/2021-03-17_Project_Setup-Windows-quad.png">](doc/screenshot/2021-03-17_Project_Setup-Windows.png)]

## Mac
```
sh install_vcpkg_packages.sh
sh gen_xcode.sh
```
- Open Xcode project in `build/libstia-xcode`

![[<img src="doc/screenshot/2021-03-17_Project_Setup-Mac-quad.png">](doc/screenshot/2021-03-17_Project_Setup-Mac.png)]

## Ubuntu Linux
```
sh install_ubunta_packages.sh
sh install_vcpkg_packages.sh
make
make run
```

## Visual Studio Code + Cmake Tool
```
sh install_ubunta_packages.sh
sh install_vcpkg_packages.sh
```
- install vscode extension `CMake Tools` and `CMake`
- Build and Run from bottom status bar

![[<img src="doc/screenshot/2021-03-17_Project_Setup-Linux-half.png">](doc/screenshot/2021-03-17_Project_Setup-Linux.png)