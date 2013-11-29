ifeq ($(CFG_BSP),y)
BSP_CPPFLAGS+=-I$(TOPDIR)/bsp/$(BOARD)/include
endif
