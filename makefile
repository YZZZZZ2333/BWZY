###### 子目录配置区
DIROBJS		= \
			src \
			shbin \

###### 加载mktpl模板库
#@ FILESYSTEM
#@ dir_all
#@ dir_make
#@ dir_clean
#@ dir_install
#@ dir_uninstall
include $(MKTPL2_HOME)/makedir_$(MKTPL2_OS).inc

