@echo off
pushd %~dp0

for /f "usebackq tokens=*" %%i in (`"%~dp0bin\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath`) do (
  set _vcvarsall="%%i\VC\Auxiliary\Build\vcvarsall.bat"
)

if not exist %_vcvarsall% (
   if defined VSINSTALLDIR (
      set _vcvarsall="%VSINSTALLDIR%VC\vcvarsall.bat"
   ) else (
      set _vcvarsall="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
   )

   if not exist %_vcvarsall% (
      set _vcvarsall="C:\Program Files (x86)\Microsoft Visual C++ Build Tools\vcbuildtools.bat"
      if not exist %_vcvarsall% (
         echo Failed to locate vcvarsall.bat!
         pause
         exit /b 1
      )
   )
)

call %_vcvarsall% x64
set PATH=%~dp0stage;%~dp0bin;%PATH%
cmd /K "cd /d %~dp0"
popd
