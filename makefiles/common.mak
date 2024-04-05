# Common compiler flags

UwUMaker-linker-flags-$(CONFIG_ASAN) += -fsanitize=address
UwUMaker-linker-flags-$(CONFIG_MSAN) += -fsanitize=memory
UwUMaker-linker-flags-$(CONFIG_TSAN) += -fsanitize=thread
UwUMaker-linker-flags-$(CONFIG_UBSAN) += -fsanitize=undefined

# C23 support headers
UwUMaker-c-flags-$(CONFIG_USE_BUILTIN_STDBIT) += -isystem '$(ROOT_PROJECT_DIR)/c23_support/stdbit'
UwUMaker-c-flags-$(CONFIG_USE_BUILTIN_STDCKINT) += -isystem '$(ROOT_PROJECT_DIR)/c23_support/stdckint'
