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
	-rm touchtheme.script
	-rm $(ROOT)/../../include/res/theme.h
	-rm $(WORKSPACEPATH)/beetles/rootfs/apps/theme.bin
	
	# make scripts for every application
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./init           1       1000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./demo           1001    2000
	#make -C common																															#2001		3000		
	#make -C video																												        #3001   4000
	#make -C music_new																														#4101   5000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./music          4001    4100
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./photo          5001    6000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./main           6001    7000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./explorer       7001    8000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./ebook          8001    9000
	#$(ESTUDIOROOT)/Softwares/Face200/MakeScript/MakeScript.exe ./setting        9001    10000
	
	find ./ -name "*.script" | xargs rm -f 
	find -type d > dirs.txt 
	./makescript dirs.txt touchtheme.script
	 -rm dirs.txt
	# create theme for the whole case

	#cat ./config/touchthemehdr.script   \
	#	./init/init.script              \
	#	./demo/demo.script              \
	#	./music/music.script              \
	#	./photo/photo.script              \
	#	./main/main.script              \
	#	./explorer/explorer.script              \
	#	./ebook/ebook.script              \
	#	./setting/setting.script              \
        #>> touchtheme.script

	# build the theme
	$(ESTUDIOROOT)/Softwares/Face200/FaceBuilderCmd/FaceBuilderCmd.exe ./config/config.ini
	# copy the theme to workspace
	cp -f ./touchtheme.h   $(ROOT)/../../include/res/theme.h
	cp -f ./touchtheme.bin $(WORKSPACEPATH)/beetles/rootfs/apps/theme.bin
	#-rm ./touchtheme.h

# 删除生成的中间文件
clean:
	-rm ./touchtheme.bin
	-rm ./touchtheme.face
	-rm ./touchtheme.h
	-rm ./touchtheme.script
	-rm $(ROOT)/../../include/res/theme.h
	-rm $(WORKSPACEPATH)/beetles/rootfs/apps/theme.bin
	
	
