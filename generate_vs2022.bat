@echo off
setlocal enabledelayedexpansion

pushd %~dp0

echo ===========================
echo Getting all the submodules
echo ===========================
call %~dp0\get_submodules.bat

set ROOT_DIR=%~dp0
set CMAKE_OUTPUT_DIR="%~dp0\cmake_output\build_win64"
set PRESET="win_x64"

echo ===========================
echo Looking for MSBuild
echo ===========================
set "PATH=%PATH%;%ProgramFiles(x86)%\Microsoft Visual Studio\Installer"
set MSBUILD_PATH=""
for /f "tokens=* USEBACKQ" %%i in (`vswhere.exe -version "[17.0,)" -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
    set MSBUILD_PATH=%%i
)

if "%MSBUILD_PATH%" == """" (
    echo "VS2022 wasn't found"
    pause
    popd
    exit
)
echo "MSbuild path is: %MSBUILD_PATH%"

set CMAKE_EXE="%~dp0\Tools\Build\CMake\bin\cmake.exe"

if not exist %CMAKE_OUTPUT_DIR% (
    mkdir %CMAKE_OUTPUT_DIR%
)

echo "Generating cmake project"
%CMAKE_EXE% %ROOT_DIR% --preset=%PRESET% %cmake_args%
if "%ERRORLEVEL%" NEQ "0" (
	echo "Failed to generate cmake project"
	pause
    popd
    exit
)

popd

echo ===========================
echo Configuring Visual Studio solution
echo ===========================

"%MSBUILD_PATH%" /nologo -m -t:restore %CMAKE_OUTPUT_DIR%/GameEngine.sln

echo ===========================
echo Create CLangd environment for VSCode
echo ===========================

where git >nul 2>nul
if errorlevel 1 (
    echo Git not found. Please install Git.
    pause
    popd
    exit /b 1
)
where dotnet >nul 2>nul
if errorlevel 1 (
    echo .NET SDK not found. Please install .NET SDK.
    pause
    popd
    exit /b 1
)

set VSCCC_DIR=%ROOT_DIR%Tools\vsccc
if not exist "%VSCCC_DIR%" (
    echo Cloning vsccc...
    git clone https://github.com/jdm3/vsccc.git "%VSCCC_DIR%"
    if errorlevel 1 (
        echo Failed to clone vsccc.
        pause
        popd
        exit /b 1
    )
)

pushd "%VSCCC_DIR%"
echo Building vsccc...
dotnet build -c Release
if errorlevel 1 (
    echo Failed to build vsccc. Ensure .NET SDK is installed.
    popd
    pause
    popd
    exit /b 1
)
popd

set VSCCC_EXE=%VSCCC_DIR%\bin\Release\net8.0\vsccc.exe
if not exist "%VSCCC_EXE%" (
    echo vsccc.exe not found after build. Check build output.
    pause
    popd
    exit /b 1
)

echo Generating compile_commands.json from "%CMAKE_OUTPUT_DIR%\GameEngine.sln"
pushd "%CMAKE_OUTPUT_DIR%"
"%VSCCC_EXE%" -p:Platform=x64 -p:Configuration=Debug GameEngine.sln
if errorlevel 1 (
    echo Failed to generate compile_commands.json
    popd
    pause
    popd
    exit /b 1
)
popd

echo Copying compile_commands.json to %ROOT_DIR%
copy /Y "%CMAKE_OUTPUT_DIR%\compile_commands.json" "%ROOT_DIR%"
if errorlevel 1 (
    echo Failed to copy compile_commands.json
    pause
    popd
    exit /b 1
)

echo Done. compile_commands.json is ready for clangd.
pause
