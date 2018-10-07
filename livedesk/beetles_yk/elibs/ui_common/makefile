#/*
#*********************************************************************************************************
#*                                                    MELIS
#*                                    the Easy Portable/Player Develop Kits
#*                                               Shared Library
#*
#*                                    (c) Copyright 2006-2010, kevin.z China
#*                                             All Rights Reserved
#*
#* File    : makefile
#* By      : Bennis
#* Version : v1.0
#* Date    : 2012-12-27
#* Descript:
#* Update  : date                auther      ver     notes
#*********************************************************************************************************
#*/

#导入编译器配置
include make.cfg


#从所有子目录中得到源代码的列表
SRCCS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c))
SRCSS=$(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.s))

#得到源代码对应的目标文件的列表
OBJS=$(SRCCS:.c=.o) $(SRCSS:.s=.o)

all:$(LOCALTARGET)
	cp $(LOCALTARGET) $(TARGET)
	@echo -------------------------------
	@echo Library $(LOCALTARGET) built!
	@echo -------------------------------

$(LOCALTARGET):$(OBJS)
	$(AR) $(ARFLAGS) $(LOCALTARGET) $(filter %.o ,$+)


# 删除生成的中间文件
clean:
	-rm $(OBJS) $(LOCALTARGET)
	-rm $(TARGET)
	@echo -------------------------------
	@echo Library $(LOCALTARGET) clean up!
	@echo -------------------------------
