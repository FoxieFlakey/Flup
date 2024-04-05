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
		-Wundef

UwUMaker-linker-flags-y += -lm -lBlocksRuntime

UwUMaker-dirs-y += data_structs core interface util concurrency thread
UwUMaker-dirs-$(CONFIG_ENABLE_TEST_EXE) += test

UwUMaker-is-executable := m
UwUMaker-name := Flup$(CONFIG_VERSION_MAJOR)

proj_test:
	@$(MAKE) -C $(UWUMAKER_HOME) PROJECT_DIR="$(PROJECT_DIR)" cmd_all
	@cd "$(PROJECT_DIR)" && LD_LIBRARY_PATH="./build/objs:$$LD_LIBRARY_PATH" ./build/objs/test/test_exe/objs/Test

proj_doxygen:
	@cd "$(PROJECT_DIR)" && doxygen
	@echo "Doxygen result viewable at http://localhost:8080"
	@cd "$(PROJECT_DIR)/doxygen_output/html" && php -S localhost:8080



