@echo off
set workDir=%~dp0
rem cd /d %~dp0


rem set path=%workDir%third_party\python_26;%path%
rem 产生cp1工程。
python build\gyp_chromium -D"component=shared_library" -G msvs_version=2008 cp1\cp1.gyp

pause
