::/*
::*********************************************************************************************************
::*                                                    MELIS
::*                                    the Easy Portable/Player Develop Kits
::*                                         8bpp, pack, compress config
::*
::*                                    (c) Copyright 2011-2014, Kingvan China
::*                                             All Rights Reserved
::*
::* File    : 8bpp_pack_compress.bat
::* By      : Kingvan
::* Version : v1.0
::* Date    : 2011-6-28
::* Descript: 8bpp, pack, compress
::* Update  : date                auther      ver     notes
::*           2011-6-28           Sunny       1.0     8bpp, pack, compress
::*********************************************************************************************************
::*/

@echo off

set EXEPATH=.\8bpp_pack_compree
set PALNAME=wkm.pal

:START
    @echo ********************************************************
    @echo *   选择需要的转换                                     *
    @echo ********************************************************
    @echo  0: 将png转换成32bpp
    @echo  1: 将32bpp转换成8bpp
    @echo  2: 将bmp压缩
    @echo  3: 设置32bpp为透明属性
    @echo  4: 清除32bpp为透明属性
    @echo  5: 将32bpp转换成8bpp,然后将8bpp压缩
    @echo  6: 将32bpp转换成8bpp,然后将8bpp打包
    @echo  7: 将32bpp转换成8bpp,然后将8bpp打包,最后将打包文件压缩
    @echo  8: 将wav压缩
    @echo  9: 修改32bpp透明度
    @echo  10: 退出

    @echo ********************************************************
:SELECT
    set /p SEL=Please Select:
    if %SEL%==0     goto PNG_32bpp
    if %SEL%==1     goto 32bpp_8bpp
    if %SEL%==2     goto bmp_COMPRESS
    if %SEL%==3     goto modify_32bpp_1
    if %SEL%==4     goto modify_32bpp_0
    if %SEL%==5     goto 32bpp_8bpp_COMPRESS
    if %SEL%==6     goto 32bpp_8bpp_PACK
    if %SEL%==7     goto 32bpp_8bpp_PACK_COMPRESS    
    if %SEL%==8     goto wav_COMPRESS
    if %SEL%==9     goto 32bpp_ALPHA
    if %SEL%==10     goto EXIT_CHANGE

    @echo 输入错误,请按照要求选择!
    goto SELECT

:PNG_32bpp
    %EXEPATH%\png2bmp.exe
    goto START
    
:32bpp_8bpp    
    %EXEPATH%\bmp_list.exe
    %EXEPATH%\8bpp_interface.exe  bmp_list.txt        ^
                                  custom_color.txt    ^
                                  0xffffff            ^
                                  0x000000            ^
    	                          %PALNAME%            
    %EXEPATH%\rename.exe
    goto START

:32bpp_8bpp_COMPRESS
    %EXEPATH%\bmp_list.exe
    %EXEPATH%\8bpp_interface.exe  bmp_list.txt        ^
    	                          custom_color.txt    ^
    	                          0xffffff            ^
    	                          0x000000            ^
    	                          %PALNAME%            
    %EXEPATH%\rename.exe
    
    for /f "delims=" %%i in ('dir /b /a-d /s "*.bmp"') do %EXEPATH%\az100_batch.exe e %%i tmp.o & copy tmp.o %%i
    del tmp.o 
    goto START
:bmp_COMPRESS
    for /f "delims=" %%i in ('dir /b /a-d /s "*.bmp"') do %EXEPATH%\az100_batch.exe e %%i tmp.o & copy tmp.o %%i
    del tmp.o 
    goto START

:wav_COMPRESS
    for /f "delims=" %%i in ('dir /b /a-d /s "*.wav"') do %EXEPATH%\az100.exe e "%%i" tmp.o & copy tmp.o "%%i"
    del tmp.o
    goto START

:32bpp_ALPHA
    set /p ALPHA_VAL=please input alpha val(hex or dec):

    for /f "delims=" %%i in ('dir /b /a-d /s "*.bmp"') do %EXEPATH%\Modify32BppAlpha.exe alpha "%%i" tmp.o 0x20 & copy tmp.o "%%i"
    goto START


:modify_32bpp_1
    for /f "delims=" %%i in ('dir /b /a-d /s "*.bmp"') do %EXEPATH%\modify_32bpp.exe %%i 1
    goto START

:modify_32bpp_0
    for /f "delims=" %%i in ('dir /b /a-d /s "*.bmp"') do %EXEPATH%\modify_32bpp.exe %%i 0
    goto START

:32bpp_8bpp_PACK
    %EXEPATH%\bmp_list.exe
    %EXEPATH%\8bpp_interface.exe  bmp_list.txt        ^
    	                          custom_color.txt    ^
    	                          0xffffff            ^
    	                          0x000000            ^
    	                          %PALNAME%            
    %EXEPATH%\rename.exe

    @echo ***********************************************************
    @echo	makePack.exe将.bmp文件打包成.pack,
    @echo   用户根据自己的模块自定义包名   
    @echo ***********************************************************
    set /p SEL=Please Input Pack Name:
    %EXEPATH%\makePack.exe %SEL%
    goto START
    
:32bpp_8bpp_PACK_COMPRESS
    %EXEPATH%\bmp_list.exe
    %EXEPATH%\8bpp_interface.exe  bmp_list.txt        ^
    	                          custom_color.txt    ^
    	                          0xffffff            ^
    	                          0x000000            ^
    	                          %PALNAME%            
    %EXEPATH%\rename.exe

    @echo ***********************************************************
    @echo	makePack.exe将.bmp文件打包成.pack,
    @echo   用户根据自己的模块自定义包名   
    @echo ***********************************************************
    set /p SEL=Please Input Pack Name:
    %EXEPATH%\makePack.exe %SEL%

    for /f "delims=" %%i in ('dir /b /a-d /s "*.pack"') do %EXEPATH%\az100_batch.exe e %%i tmp.o & copy tmp.o %%i
    del tmp.o 
    goto START


:EXIT_CHANGE
    exit

:FINISH
    @echo *********************************************
    pause

