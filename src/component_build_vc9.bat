@echo off
set workDir=%~dp0
rem cd /d %~dp0


rem set path=%workDir%third_party\python_26;%path%
rem ����chrome����ص�yygame���̡�
python build\gyp_chromium -D"component=shared_library" -D"disable_nacl=1"  -G msvs_version=2008 base\base.gyp
rem ��������yygame���̡�
rem python build\gyp_chromium -D"component=shared_library" -D"disable_nacl=1"  -G msvs_version=2008 yygame\yygame.gyp

rem ����������úõ�ffmpegsumo���̣���֤ffmpegsumo��vs2008�ϱ���ͨ����by ZC. 2013-5-28 16:20.
rem copy third_party\ffmpeg\ffmpegsumo.vcproj.backup third_party\ffmpeg\ffmpegsumo.vcproj /Y
rem call third_party\cygwin\setup_mount.bat

pause
