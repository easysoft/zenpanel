@echo off
setlocal enabledelayedexpansion 
set remain=%PATH%
::待查找字符串
set toAdd=%ENV_PATH%
::标记,默认没有重复
set finded=false
:loop
for /f "tokens=1* delims=;" %%a in ("%remain%") do (
	::如果找到相同的了
	if "%toAdd%"=="%%a" (
		::直接退出
		goto :isFinded
		::该表标记,true表示有重复的了
		set finded=true
	)
	rem 将截取剩下的部分赋给变量remain，其实这里可以使用延迟变量开关
	set remain=%%b
)
::如果还有剩余,则继续分割
if defined remain goto :loop
::如果没有重复:
if "%finded%"=="false" (
	echo 正在修改系统path环境变量...
	setx PATH "%toAdd%;%PATH%" /m
	::结束程序
	goto :end
)
:isFinded
echo path环境变量中已经有了该环境变量,无须重复添加.
:end
