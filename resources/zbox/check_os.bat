@echo off
if exist "%windir%\SysWOW64" (echo 64) else (echo 32)