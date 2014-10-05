@echo OFF
rem IMPORTANT Call this batch from eos_client/build/ directory

if "%1"=="" goto usage

rem Версия используемой VisualStudio
set VSVER=%1

set BUILD_TYPE=Debug
if /i "%2"=="debug"		set BUILD_TYPE=Debug
if /i "%2"=="release"	set BUILD_TYPE=Release

set PLATFORM=x86
if /i "%3"=="x86"       set PLATFORM=x86
if /i "%3"=="amd64"     set PLATFORM=amd64
if /i "%3"=="x64"       set PLATFORM=amd64
if /i "%3"=="arm"       set PLATFORM=arm
if /i "%3"=="x86_arm"   set PLATFORM=x86_arm
if /i "%3"=="x86_amd64" set PLATFORM=x86_amd64

rem Платформа, используется при генерации проекта MSVS
set PROJ_TYPE=
if /i %PLATFORM%==amd64     set PROJ_TYPE= Win64
if /i %PLATFORM%==x64       set PROJ_TYPE= Win64
if /i %PLATFORM%==x86_amd64 set PROJ_TYPE= Win64

rem Платформа (x86/x64), используется в CMakeLists.txt для определения папки с библиотеками
set LIB_PLATFORM_PARAM=x86
if /i %PLATFORM%==amd64     set LIB_PLATFORM_PARAM=x64
if /i %PLATFORM%==x64       set LIB_PLATFORM_PARAM=x64
if /i %PLATFORM%==x86_amd64 set LIB_PLATFORM_PARAM=x64

rem Каталог установки используемой VisualStudio
set VSDIR=
for /F "tokens=1,2*" %%i in ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\%VSVER%.0" /V "ShellFolder"') do (
	if "%%i"=="ShellFolder" set "VSDIR=%%k"
)
if "%VSDIR%"=="" (
	echo VisualStudio %VSVER% could not be found.
	goto end
)

rem Очищаем текущий каталог для генерации новых файлов проекта
for /F "delims=" %%i in ('dir /b') do (rmdir "%%i" /s/q || del "%%i" /s/q)

rem Выполняем конфигурирование VisualStudio
call "%VSDIR%\VC\vcvarsall.bat" %PLATFORM%

rem Генерируем файлы решения VisualStudio
cmake %~dp0 -DLIB_PLATFORM=%LIB_PLATFORM_PARAM% -G "Visual Studio %VSVER%%PROJ_TYPE%"

rem Запускаем сборку проекта
cmake --build .\ --target ALL_BUILD --config %BUILD_TYPE%

goto end

:usage
echo:
echo Error in script usage. The correct usage is:
echo     %0 [version] [build] [platform]
echo where [version] is version of installed Visual Studio, e.g. 11 is VisualStudio 2012 and 12 is VisualStudio 2013
echo       [build] is release or debug (optional, default is debug)
echo       [platform] is [x86 ^| x64 ^| arm ^| x86_arm ^| x86_amd64] (optional, default is x86)
echo:
echo For example:
echo     %0 11 debug x64
goto :end

:end
