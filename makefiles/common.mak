# Common compiler flags

UwUMaker-linker-flags-$(CONFIG_ASAN) += -fsanitize=address
UwUMaker-linker-flags-$(CONFIG_MSAN) += -fsanitize=memory
UwUMaker-linker-flags-$(CONFIG_TSAN) += -fsanitize=thread
UwUMaker-linker-flags-$(CONFIG_UBSAN) += -fsanitize=undefined
