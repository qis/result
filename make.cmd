@echo off
rem
rem Download make executable and add it to %Path%:
rem
rem   http://www.equation.com/servlet/equation.cmd?fa=make
rem

set __VS_LOCATION=%ProgramFiles(x86)%\Microsoft Visual Studio\2019
set __VS_EDITIONS="Enterprise,Professional,Community"

set __SKIP_VCVARS=1
if "%1"=="" set __SKIP_VCVARS=0

for %%i in (%*) do (
  (echo :format: :clean: :ports: | findstr /i ":%%i:" 1>nul 2>nul) || set __SKIP_VCVARS=0
)

if "%__SKIP_VCVARS%"=="1" (
  goto :skip_vcvars
)

if not "%MAKE_WRAPPER_VCVARS_INITIALIZED%"=="1" (
  for %%i in (%__VS_EDITIONS:"=%) do (
    if exist "%__VS_LOCATION:"=%\%%i\VC\Auxiliary\Build\vcvarsall.bat" (
      pushd "%__VS_LOCATION:"=%\%%i\VC\Auxiliary\Build"
      goto :vcvarsall_found
    )
  )

  echo error: missing script: "vcvarsall.bat" 1>&2
  goto :cleanup

  :vcvarsall_found
  call vcvarsall.bat x64 && goto initialized
  popd
  echo error: could not initialize vcvars 1>&2
  goto :cleanup

  :initialized
  popd
  set MAKE_WRAPPER_VCVARS_INITIALIZED=1
)

:skip_vcvars

make.exe %*

:cleanup
set __SKIP_VCVARS=
set __VS_EDITIONS=
set __VS_LOCATION=
