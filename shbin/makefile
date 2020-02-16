###### 源文件配置区

###### 目标文件、安装目录配置区
include ../makeinstall
NOCLEAN_OBJ	=	\
			xdocker_ldd_and_cp_lib64.sh \
			xdocker_profile_template.sh \
			xdocker_etc_profile_template.sh \
			xdocker_install_test.sh \
			xdocker_create_image_rhel-7.4-x86_64.sh \
			xdocker_create_image_rhel-7.4-gcc-x86_64.sh \
			xdocker_create_image_rhel-7.4-sshd-x86_64.sh \
			xdocker_container_root.sh \

NOCLEAN_OBJINST	=	$(_BINBASE)

###### 编译选项
CFLAGS		=	$(_CFLAGS) \

###### 链接选项
LFLAGS		=	$(_LFLAGS) \

###### 额外宏定义区
CLEAN_ADDITION	=

###### 加载mktpl模板库
#@ make_all
#@ make_clean
#@ make_install
#@ make_install_NOCLEAN_OBJINST
#@ make_uninstall
#@ make_uninstall_NOCLEAN_OBJINST
include $(MKTPL2_HOME)/makeobj_$(MKTPL2_OS).inc

###### 目标文件依赖关系

