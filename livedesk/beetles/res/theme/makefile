#/*
#*********************************************************************************************************
#*                                                    MELIS
#*                                    the Easy Portable/Player Develop Kits
#*                                               makefile Module
#*
#*                                    (c) Copyright 2006-2010, kevin.z China
#*                                             All Rights Reserved
#*
#* File    : makefile
#* By      : kevin.z
#* Version : v1.0
#* Date    : 2010-9-3 9:39
#* Descript:
#* Update  : date                auther      ver     notes
#*********************************************************************************************************
#*/

ROOT    = .
SDKROOT = $(ROOT)/../../../..

#导入交叉编译器通用配置
include $(SDKROOT)/includes/cfgs/CROSSTOOL.CFG

buildtheme:
	-rm theme.script
	-rm $(ROOT)/../../include/res/theme.h
	-rm $(WORKSPACEPATH)/beetles/rootfs/apps/theme.bin
	
	# make scripts for every application
	find ./ -name "*.script" | xargs rm -f
	find -type d > dirs.txt 
	./makescript dirs.txt theme.script
	-rm dirs.txt

	# build the theme
	$(ESTUDIOROOT)/Softwares/Face200/FaceBuilderCmd/FaceBuilderCmd.exe ./config/config.ini
	
	# copy the theme to workspace
	cp ./theme.h   $(ROOT)/../../include/res/theme.h
	cp ./theme.bin $(WORKSPACEPATH)/beetles/rootfs/apps/theme.bin

# 删除生成的中间文件
clean:
	-rm ./theme.bin
	-rm ./theme.face
	-rm ./theme.h
	-rm ./theme.script

