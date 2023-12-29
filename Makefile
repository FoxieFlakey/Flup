
UwUMaker-c-flags-y += -std=c2x -g \
		-Wall -Wshadow -Wpointer-arith \
		-Wmissing-prototypes \
		-fpic -fblocks -Wextra \
		-D_POSIX_C_SOURCE=200809L \
		-fvisibility=hidden -fno-common 

UwUMaker-linker-flags-y += -lm -lBlocksRuntime

UwUMaker-dirs-$(CONFIG_ENABLE_TEST_EXE) += test

UwUMaker-is-executable := m
UwUMaker-name := FluffedLib$(CONFIG_VERSION_MAJOR)

