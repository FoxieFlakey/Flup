
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
    -isystem $(PROJECT_DIR)/c23_support/

UwUMaker-linker-flags-y += -lm -lBlocksRuntime

UwUMaker-dirs-y += data_structs core c23_support interface
UwUMaker-dirs-$(CONFIG_ENABLE_TEST_EXE) += test

UwUMaker-is-executable := m
UwUMaker-name := Flup$(CONFIG_VERSION_MAJOR)






