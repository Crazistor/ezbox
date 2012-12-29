PKG_INSTALL:=1

ifneq ($(findstring c,$(OPENWRT_VERBOSE)),)
  MAKE_FLAGS+=VERBOSE=1
endif

CMAKE_SOURCE_DIR:=.

ifeq ($(CONFIG_CCACHE),)
 ifeq ($(CONFIG_EXTERNAL_TOOLCHAIN),)
  CMAKE_C_COMPILER:=$(TOOLCHAIN_DIR)/bin/$(TARGET_CC)
  CMAKE_C_COMPILER_ARG1:=
  CMAKE_CXX_COMPILER:=$(TOOLCHAIN_DIR)/bin/$(TARGET_CXX)
  CMAKE_CXX_COMPILER_ARG1:=
 else
  CMAKE_C_COMPILER:=$(shell which $(TARGET_CC))
  CMAKE_C_COMPILER_ARG1:=
  CMAKE_CXX_COMPILER:=$(shell which $(TARGET_CXX))
  CMAKE_CXX_COMPILER_ARG1:=
 endif
else
  CCACHE:=$(shell which ccache)
  ifeq ($(CCACHE),)
    CCACHE:=$(STAGING_DIR_HOST)/bin/ccache
  endif
  CMAKE_C_COMPILER:=$(CCACHE)
  CMAKE_C_COMPILER_ARG1:=$(filter-out ccache,$(TARGET_CC))
  CMAKE_CXX_COMPILER:=$(CCACHE)
  CMAKE_CXX_COMPILER_ARG1:=$(filter-out ccache,$(TARGET_CXX))
endif

define Build/Configure/Default
	(cd $(PKG_BUILD_DIR); \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		cmake \
			-DCMAKE_SYSTEM_NAME=Linux \
			-DCMAKE_SYSTEM_VERSION=1 \
			-DCMAKE_SYSTEM_PROCESSOR=$(ARCH) \
			-DCMAKE_BUILD_TYPE=Release \
			-DCMAKE_C_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_CXX_FLAGS_RELEASE="-DNDEBUG" \
			-DCMAKE_C_COMPILER="$(CMAKE_C_COMPILER)" \
			-DCMAKE_C_COMPILER_ARG1="$(CMAKE_C_COMPILER_ARG1)" \
			-DCMAKE_CXX_COMPILER="$(CMAKE_CXX_COMPILER)" \
			-DCMAKE_CXX_COMPILER_ARG1="$(CMAKE_CXX_COMPILER_ARG1)" \
			-DCMAKE_EXE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_MODULE_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_SHARED_LINKER_FLAGS:STRING="$(TARGET_LDFLAGS)" \
			-DCMAKE_FIND_ROOT_PATH=$(STAGING_DIR) \
			-DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=BOTH \
			-DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
			-DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
			-DCMAKE_STRIP=: \
			-DCMAKE_INSTALL_PREFIX=/usr \
			-DDL_LIBRARY=$(STAGING_DIR) \
                        -DCMAKE_PREFIX_PATH=$(STAGING_DIR) \
			$(CMAKE_OPTIONS) \
		$(CMAKE_SOURCE_DIR) \
	)
endef

define Build/InstallDev/cmake
	$(INSTALL_DIR) $(1)
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
endef

Build/InstallDev = $(if $(CMAKE_INSTALL),$(Build/InstallDev/cmake))
