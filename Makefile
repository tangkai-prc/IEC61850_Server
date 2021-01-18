################################
# Makefile 示例 (通用版，根目录下自动生成依赖文件)
################################

#CROSS_COMPILE=/opt/zynq/gcc-arm-linux-gnueabi_5.2.1/bin/arm-linux-gnueabihf-
CROSS_COMPILE=arm-linux-gnueabihf-
export CROSS_COMPILE

CC := $(CROSS_COMPILE)gcc
CPP := $(CROSS_COMPILE)g++
AR := $(CROSS_COMPILE)ar
STRIP := $(CROSS_COMPILE)strip
OBJCOPY := $(CROSS_COMPILE)objcopy


#esp: embedded system platform
ifeq (arm-linux-gnueabihf-, $(CROSS_COMPILE))
target_platform := zynq
else
target_platform := pc
endif
export target_platform

################################
# dirs
################################
CURDIR = $(shell pwd)
#SUBDIRS = $(shell ls -l | grep ^d | awk '{if($$9 != "build") print $$9}')
SUBDIRS = $(shell find -type d ! -path '*build*' ! -path '.' | xargs)  #$(shell ls -FR | grep /$ | sed "s:^:`pwd`/:")
COMNDIRS = $(CURDIR)/../common
MODLDIRS = ../module
LIBDIRS = $(COMNDIRS)/switch_msg $(COMNDIRS)/switch_sql $(COMNDIRS)/switch_wdt 
LIBDIRS += $(COMNDIRS)/switch_interface $(COMNDIRS)/switch_timer $(COMNDIRS)/switch_log
LIBDIRS += $(COMNDIRS)/hash_cal $(COMNDIRS)/xmlconfig


ifndef PROJECT_DIR
BLDDIR := $(CURDIR)/build
OBJ_DIR := $(BLDDIR)/obj
LIB_DIR := $(BLDDIR)/lib
BIN_DIR := $(CURDIR)
LIBOBJ_DIR = $(OBJ_DIR)
else
PROJECT_OBJ := $(PROJECT_DIR)/obj
BLDDIR := $(PROJECT_OBJ)/$(shell basename $(CURDIR))
OBJ_DIR := $(BLDDIR)
LIB_DIR := $(PROJECT_DIR)/lib
BIN_DIR := $(PROJECT_DIR)/bin
LIBOBJ_DIR = $(addprefix $(PROJECT_OBJ)/,$(shell ls -l $(COMNDIRS) | grep ^d | awk '{print $$9}'))
export OBJ_DIR
endif

export BLDDIR

################################
# libs
################################
SYS_INC_DIR := -I/opt/zynq/gcc-arm-linux-gnueabi_5.2.1/arm-linux-gnueabihf/include
SYS_INC_DIR += -I/opt/sqlite3_build/include
SYS_INC_DIR += -I/opt/ncurses/include
SYS_INC_DIR += -I/opt/swzynq_lib/libiconv/include

SYS_SO_DIR := -L/opt/zynq/gcc-arm-linux-gnueabi_5.2.1/arm-linux-gnueabihf/lib
SYS_SO_DIR += -L/opt/zynq/gcc-arm-linux-gnueabi_5.2.1/arm-linux-gnueabihf/libc/lib
SYS_SO_DIR += -L/opt/sqlite3_build/lib
SYS_SO_DIR += -L/opt/ncurses/lib
SYS_SO_DIR += -L/opt/swzynq_lib/libiconv/lib
SYS_SO_LIBS := -lc -lpthread -lsqlite3 -lncurses -lstdc++  -liconv -lm
SYS_A_LIBS := 

USR_INC_DIR := -I./ -I../include $(addprefix -I,${SUBDIRS}) $(addprefix -I,${LIBDIRS})
USR_SO_DIR := $(addprefix -L,${LIB_DIR})  -L./lib 
USR_SO_LIBS := #$(addsuffix /*.so,$(LIB_DIR))#$(wildcard $(LIB_DIR)/*.so) #$(addprefix -L,${$(wildcard $(LIB_DIR)/*.so)}) #-lcommon_so 
USR_SO_LIBS += -lexpat -l61850 -lasn1 -lcidparser -ldcisys -lfmap -lnetlog -lssacsi -lutils 
USR_A_LIBS := $(addsuffix /*_$(target_platform).a,$(LIB_DIR))	$(wildcard ./lib/*.a)	

STATIC_LIBS := -Wl,--start-group $(USR_A_LIBS) -Wl,--end-group #--copy-dt-needed-entries,

SRC_DIR := $(CURDIR) $(SUBDIRS)
INC_DIR := $(SYS_INC_DIR) $(USR_INC_DIR) 

################################
# VPATH
################################
#VPATH = $(SRC_DIR) #VPATH: makefile 专用变量，表示全局搜索路径，当在当前目录中无法搜索到源文件时，去VPATH中所包含的目录下寻找源文件
vpath %.c $(SRC_DIR) #vpath: makefile 关键字， 用于设置不同文件的搜索路径
vpath %.cpp $(SRC_DIR) #vpath: makefile 关键字， 用于设置不同文件的搜索路径
vpath %.o $(OBJ_DIR)

################################
# flags
################################

# CFLAGS: gcc或g++编译器指定的选项，通常用于指定头文件路径(-Idir, 如-I./inc)
# LDFLAGS: gcc或g++编译器指定的选项，通常用于指定库文件路径(-Ldir, 如-L./lib)
# LIBS: gcc或g++编译器指定的选项，通常用于指定库文件(-llib, 如-lpthread)
CFLAGS := -g -D__LINUX__ -rdynamic -ldl -O2  $(INC_DIR) #-ldl -DDEBUG_TEST -DBCM_ROBO_SUPPORT -DBCM_53280_A0

LDFLAGS := $(SYS_SO_DIR) $(SYS_SO_LIBS) $(USR_SO_DIR) $(USR_SO_LIBS) -Wl,--relax -g
LIBS :=

################################
# variables
################################

# PROGRAMS: 生成的可执行文件名
PROGRAMS := 61850_sw 
 
# wildcard函数: 
# 格式: $(wildcard PATTEN) 
# 参数: PATTEN 文件通配符
# 作用: 将符合文件通配符描述的文件展开为一系列文件名，文件名之间以空格间隔，并将这一系列文件名返回
SRCS := $(wildcard $(addsuffix /*.c,$(SRC_DIR)))
CPPSRCS += $(wildcard $(addsuffix /*.cpp,$(SRC_DIR)))

# patsubst函数
# 格式: $(patsubst PATTEN, REPLACEMENT, TEXT) 
# 参数: PATTEN 			源式样
#				REPLACEMENT 目标式样
#				TEXT 				需要被处理的以空格间隔的字列
# 作用: 将以源式样为格式的需要被处理的字列 用目标式样替换
#OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS))) #$(notdir ${SRCS})
CPPOBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(CPPSRCS))) 
#OBJS_ALL := $(OBJ_DIR)/*.o  #$(addsuffix /*.o,$(LIBOBJ_DIR))#$(LIBOBJ_DIR)/*.o

# $(SRCS:.c=.d)
# 作用: 将SRCS文件列表.c替换为.d, 得到依赖文件列表DEPENDS
#DEPENDS = $(SRCS:.c=.d)
DEPENDS := $(patsubst %.o, %.d, $(OBJS))
DEPENDS += $(patsubst %.o, %.d, $(CPPOBJS))
#DEPENDS_ALL := $(OBJ_DIR)/*.d

################################
# make all command
################################
# .PHONY: all 表示all是一个伪目标
# command 另起一行必须以tab开头
.PHONY: all
all:$(LIBDIRS) $(OBJS) $(CPPOBJS) #$(SUBDIRS)   
	-mkdir $(BLDDIR)
	$(CC) $(OBJS) $(CPPOBJS) $(CFLAGS) $(STATIC_LIBS) $(LDFLAGS) -o $(BIN_DIR)/$(PROGRAMS)
	$(STRIP) $(BIN_DIR)/$(PROGRAMS)

#递归调用lib目录的Makefile
$(LIBDIRS):ECHOL
	make -C $@	
ECHOL:
	@echo $(LIBDIRS)
	
#递归调用子目录的Makefile
#$(SUBDIRS):ECHOO
	#make -C $@		
#ECHOO:
	#@echo $(SUBDIRS)
    
#下面是依赖关系
# 	sinclude: 			暂停对本文件的读取，转而去读取sinclude指示的文件列表，即.d
# 	%.d:%.c: 				.d文件依赖于同名.c文件
# 	-MM/-M: 				-MM:不包括系统文件   -M:包括系统文件
#   $@:							目标文件的名称（target的集合）， 本例中表示%.d
#		$^:							所有的依赖文件，以空格分开，不包括重复的依赖文件
#		$<:							第一个依赖文件的名称， 这里第一个依赖文件名为%.c ,因此即表示所有的%.c文件
# 	$< > $@.$$$$: 	自动产生依赖关系, 为每个$<(即%.c文件)生成依赖文件$@.$$$$(即%.d.$$$$ $$$$表示随机编号)
# 									例如，为 name.c 生成 name.d.12345
#		sed:						说明：sed是一种批处理命令 可以用于在线编辑，主要功能包括正则替换、追加、删除等
#										格式：sed [option] 'command' file(s)   或者 sed [option] -f scriptfile file(s)     
#													本例中的格式 sed [option] 'command' <oldfile> newfile
#										具体实现：'s,MATCH,REPLACE,[限定方式]' 命令s表示替换，在限定方式下将MATCH替换为REPLACE，下述命令中限定方式g表示整行均替换
#															\($*\)\.o[ :]* 表示所有的如下形式的字符【*.o:*】 或 【*.o *】 (匹配若干个空格或冒号)
#															\1.o $@ 表示所有的如下形式的字符【*.o *.d:*】或【*.o *.d*】		(匹配若干个空格或冒号) (\1表示前面第一个被括起的内容，即\($*\) )
#															整行sed命令表示 从%.d.$$$$文件中读取字符，将 *.o:* 替换为 *.o *.d:* , 然后将结果输出到%.d
#		rm -f $@.$$$$:	删除临时文件%.d.$$$$
sinclude $(DEPENDS)
#%.d:%.c 
$(OBJ_DIR)/%.d:%.c 
	-mkdir $(BLDDIR)
	-mkdir $(OBJ_DIR)
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : , g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
	
#%.d:%.cpp
$(OBJ_DIR)/%.d:%.cpp
	-mkdir $(BLDDIR)
	-mkdir $(OBJ_DIR)
	$(CPP) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $@ : , g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

#compile source files
$(OBJ_DIR)/%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@
	

$(OBJ_DIR)/%.o:%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@

################################
# make clean
################################
.PHONY: clean
clean:
	-rm -f $(OBJS) $(CPPOBJS) $(DEPENDS) $(PROGRAMS)
	-rm -rf $(OBJ_DIR) $(LIB_DIR) $(BLDDIR)

