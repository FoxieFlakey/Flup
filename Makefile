include $(ROOT_PROJECT_DIR)/makefiles/common.mak

UwUMaker-c-flags-y += -std=c2x -g \
		-Wall -Wshadow -Wpointer-arith \
		-Wmissing-prototypes \
		-fpic -fblocks -Wextra \
		-D_POSIX_C_SOURCE=200809L \
		-fvisibility=hidden -fno-common \
		-Wmissing-field-initializers \
		-Wstrict-prototypes \
		-Waddress -Wconversion -Wunused \
		-Wcast-align -Wfloat-equal -Wformat=2 \
		-fstrict-flex-arrays=3 -Warray-bounds \
		-Wno-initializer-overrides \
		-Wundef -fno-omit-frame-pointer

UwUMaker-linker-flags-y += -lm -lBlocksRuntime -lunwind

UwUMaker-pkg-config-libs-y += mimalloc libxxhash

UwUMaker-dirs-y += data_structs core interface util concurrency thread stacktrace lib hashing
UwUMaker-dirs-$(CONFIG_ENABLE_TEST_EXE) += test

UwUMaker-is-executable := m
UwUMaker-name := Flup

# Set PROJ_TEST_NAME to select which test to run
proj_run:
	@$(MAKE) -C $(UWUMAKER_DIR) PROJECT_DIR="$(PROJECT_DIR)" cmd_all
	@cd "$(PROJECT_DIR)" && LD_LIBRARY_PATH="$(BUILD_DIR)/objs:$$LD_LIBRARY_PATH" $(BUILD_DIR)/objs/test/$(PROJ_TEST_NAME)/exe/objs/Test

# Set PROJ_TEST_NAME to select which test to run
proj_run_gdb:
	@$(MAKE) -C $(UWUMAKER_DIR) PROJECT_DIR="$(PROJECT_DIR)" cmd_all
	@cd "$(PROJECT_DIR)" && LD_LIBRARY_PATH="$(BUILD_DIR)/objs:$$LD_LIBRARY_PATH" gdb $(BUILD_DIR)/objs/test/$(PROJ_TEST_NAME)/exe/objs/Test

# Set PROJ_TEST_NAME to select which test to run
proj_run_strace:
	@$(MAKE) -C $(UWUMAKER_DIR) PROJECT_DIR="$(PROJECT_DIR)" cmd_all
	@cd "$(PROJECT_DIR)" && LD_LIBRARY_PATH="$(BUILD_DIR)/objs:$$LD_LIBRARY_PATH" strace $(BUILD_DIR)/objs/test/$(PROJ_TEST_NAME)/exe/objs/Test

proj_doxygen:
	@cd "$(PROJECT_DIR)" && doxygen
	@echo "Doxygen result viewable at http://localhost:8080"
	@cd "$(PROJECT_DIR)/doxygen_output/html" && php -S localhost:8080

proj_install_check:
	@if [ -z "$$CONFIG_PREFIX" ]; then \
			1>&2 echo "[Error] CONFIG_PREFIX is not properly configured rerun cmd_config"; \
		fi
	@case "$$CONFIG_PREFIX" in \
	   /*) 1>&2 echo "[Error] CONFIG_PREFIX is not absolute path!" ;; \
		  *) ;; \
	 esac

proj_install_bin: proj_install_check
# $PROJ_OUTDIR is a variable which pretend to be root for easing packaging
# 755 permission correspond to rwxr-xr-x
	@install -Dm 755 \
		"$(BUILD_DIR)/objs/lib$(UwUMaker-name).so" \
		"$(PROJ_OUTDIR)/$(CONFIG_PREFIX)/lib/lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR).$(CONFIG_VERSION_MINOR).$(CONFIG_VERSION_PATCH)" 
	@ln -s lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR).$(CONFIG_VERSION_MINOR).$(CONFIG_VERSION_PATCH) $(PROJ_OUTDIR)/$(CONFIG_PREFIX)/lib/lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR).$(CONFIG_VERSION_MINOR)
	@ln -s lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR).$(CONFIG_VERSION_MINOR).$(CONFIG_VERSION_PATCH) $(PROJ_OUTDIR)/$(CONFIG_PREFIX)/lib/lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR)
	@ln -s lib$(UwUMaker-name).so.$(CONFIG_VERSION_MAJOR).$(CONFIG_VERSION_MINOR).$(CONFIG_VERSION_PATCH) $(PROJ_OUTDIR)/$(CONFIG_PREFIX)/lib/lib$(UwUMaker-name).so

# Install development headers
# $PROJ_OUTDIR is a variable which pretend to be root for easing packaging
proj_install_dev: proj_install_check
	@install -Dm 755 \
		"$(PROJECT_DIR)/flup/bug.h" \
		"$(PROJ_OUTDIR)/$(CONFIG_PREFIX)/include/flup/bug.h"
# 644 permission correspond to rw-r--r--
	@cd $(PROJECT_DIR)/flup && find . -type f -exec install -Dm 644 \
		"$(PROJECT_DIR)/flup/{}" \
		"$(PROJ_OUTDIR)/$(CONFIG_PREFIX)/include/flup/{}" ';'

proj_install: proj_install_bin proj_install_dev
	@:
