@echo off
set workDir=%~dp0
rem cd /d %~dp0


rem set path=%workDir%third_party\python_26;%path%
rem 产生chrome及相关的yygame工程。
python build\gyp_chromium -D"component=shared_library" -D"disable_nacl=1"  -G msvs_version=2008 base\base.gyp
rem 产生单独yygame工程。
rem python build\gyp_chromium -D"component=shared_library" -D"disable_nacl=1"  -G msvs_version=2008 yygame\yygame.gyp

rem 最后复制已配置好的ffmpegsumo工程，保证ffmpegsumo在vs2008上编译通过。by ZC. 2013-5-28 16:20.
rem copy third_party\ffmpeg\ffmpegsumo.vcproj.backup third_party\ffmpeg\ffmpegsumo.vcproj /Y
rem call third_party\cygwin\setup_mount.bat

pause
