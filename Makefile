################################
# Makefile ʾ�� (ͨ�ð棬��Ŀ¼���Զ����������ļ�)
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
#VPATH = $(SRC_DIR) #VPATH: makefile ר�ñ�������ʾȫ������·�������ڵ�ǰĿ¼���޷�������Դ�ļ�ʱ��ȥVPATH����������Ŀ¼��Ѱ��Դ�ļ�
vpath %.c $(SRC_DIR) #vpath: makefile �ؼ��֣� �������ò�ͬ�ļ�������·��
vpath %.cpp $(SRC_DIR) #vpath: makefile �ؼ��֣� �������ò�ͬ�ļ�������·��
vpath %.o $(OBJ_DIR)

################################
# flags
################################

# CFLAGS: gcc��g++������ָ����ѡ�ͨ������ָ��ͷ�ļ�·��(-Idir, ��-I./inc)
# LDFLAGS: gcc��g++������ָ����ѡ�ͨ������ָ�����ļ�·��(-Ldir, ��-L./lib)
# LIBS: gcc��g++������ָ����ѡ�ͨ������ָ�����ļ�(-llib, ��-lpthread)
CFLAGS := -g -D__LINUX__ -rdynamic -ldl -O2  $(INC_DIR) #-ldl -DDEBUG_TEST -DBCM_ROBO_SUPPORT -DBCM_53280_A0

LDFLAGS := $(SYS_SO_DIR) $(SYS_SO_LIBS) $(USR_SO_DIR) $(USR_SO_LIBS) -Wl,--relax -g
LIBS :=

################################
# variables
################################

# PROGRAMS: ���ɵĿ�ִ���ļ���
PROGRAMS := 61850_sw 
 
# wildcard����: 
# ��ʽ: $(wildcard PATTEN) 
# ����: PATTEN �ļ�ͨ���
# ����: �������ļ�ͨ����������ļ�չ��Ϊһϵ���ļ������ļ���֮���Կո�����������һϵ���ļ�������
SRCS := $(wildcard $(addsuffix /*.c,$(SRC_DIR)))
CPPSRCS += $(wildcard $(addsuffix /*.cpp,$(SRC_DIR)))

# patsubst����
# ��ʽ: $(patsubst PATTEN, REPLACEMENT, TEXT) 
# ����: PATTEN 			Դʽ��
#				REPLACEMENT Ŀ��ʽ��
#				TEXT 				��Ҫ��������Կո���������
# ����: ����Դʽ��Ϊ��ʽ����Ҫ����������� ��Ŀ��ʽ���滻
#OBJS = $(patsubst %.c, %.o, $(SRCS))
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS))) #$(notdir ${SRCS})
CPPOBJS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(CPPSRCS))) 
#OBJS_ALL := $(OBJ_DIR)/*.o  #$(addsuffix /*.o,$(LIBOBJ_DIR))#$(LIBOBJ_DIR)/*.o

# $(SRCS:.c=.d)
# ����: ��SRCS�ļ��б�.c�滻Ϊ.d, �õ������ļ��б�DEPENDS
#DEPENDS = $(SRCS:.c=.d)
DEPENDS := $(patsubst %.o, %.d, $(OBJS))
DEPENDS += $(patsubst %.o, %.d, $(CPPOBJS))
#DEPENDS_ALL := $(OBJ_DIR)/*.d

################################
# make all command
################################
# .PHONY: all ��ʾall��һ��αĿ��
# command ����һ�б�����tab��ͷ
.PHONY: all
all:$(LIBDIRS) $(OBJS) $(CPPOBJS) #$(SUBDIRS)   
	-mkdir $(BLDDIR)
	$(CC) $(OBJS) $(CPPOBJS) $(CFLAGS) $(STATIC_LIBS) $(LDFLAGS) -o $(BIN_DIR)/$(PROGRAMS)
	$(STRIP) $(BIN_DIR)/$(PROGRAMS)

#�ݹ����libĿ¼��Makefile
$(LIBDIRS):ECHOL
	make -C $@	
ECHOL:
	@echo $(LIBDIRS)
	
#�ݹ������Ŀ¼��Makefile
#$(SUBDIRS):ECHOO
	#make -C $@		
#ECHOO:
	#@echo $(SUBDIRS)
    
#������������ϵ
# 	sinclude: 			��ͣ�Ա��ļ��Ķ�ȡ��ת��ȥ��ȡsincludeָʾ���ļ��б���.d
# 	%.d:%.c: 				.d�ļ�������ͬ��.c�ļ�
# 	-MM/-M: 				-MM:������ϵͳ�ļ�   -M:����ϵͳ�ļ�
#   $@:							Ŀ���ļ������ƣ�target�ļ��ϣ��� �����б�ʾ%.d
#		$^:							���е������ļ����Կո�ֿ����������ظ��������ļ�
#		$<:							��һ�������ļ������ƣ� �����һ�������ļ���Ϊ%.c ,��˼���ʾ���е�%.c�ļ�
# 	$< > $@.$$$$: 	�Զ�����������ϵ, Ϊÿ��$<(��%.c�ļ�)���������ļ�$@.$$$$(��%.d.$$$$ $$$$��ʾ������)
# 									���磬Ϊ name.c ���� name.d.12345
#		sed:						˵����sed��һ������������ �����������߱༭����Ҫ���ܰ��������滻��׷�ӡ�ɾ����
#										��ʽ��sed [option] 'command' file(s)   ���� sed [option] -f scriptfile file(s)     
#													�����еĸ�ʽ sed [option] 'command' <oldfile> newfile
#										����ʵ�֣�'s,MATCH,REPLACE,[�޶���ʽ]' ����s��ʾ�滻�����޶���ʽ�½�MATCH�滻ΪREPLACE�������������޶���ʽg��ʾ���о��滻
#															\($*\)\.o[ :]* ��ʾ���е�������ʽ���ַ���*.o:*�� �� ��*.o *�� (ƥ�����ɸ��ո��ð��)
#															\1.o $@ ��ʾ���е�������ʽ���ַ���*.o *.d:*����*.o *.d*��		(ƥ�����ɸ��ո��ð��) (\1��ʾǰ���һ������������ݣ���\($*\) )
#															����sed�����ʾ ��%.d.$$$$�ļ��ж�ȡ�ַ����� *.o:* �滻Ϊ *.o *.d:* , Ȼ�󽫽�������%.d
#		rm -f $@.$$$$:	ɾ����ʱ�ļ�%.d.$$$$
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

