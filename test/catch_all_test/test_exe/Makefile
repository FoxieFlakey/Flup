include $(ROOT_PROJECT_DIR)/makefiles/common.mak

UwUMaker-c-sources-y += main.c

UwUMaker-c-flags-y += -I$(ROOT_PROJECT_DIR)\
	-I$(ROOT_PROJECT_DIR)/test/ \
	-UFLUP_LOG_CATEGORY \
  -DFLUP_LOG_CATEGORY='"Test/Executable"'

UwUMaker-is-executable := y
UwUMaker-name := Test

UwUMaker-shared-lib-subprojects-y += $(ROOT_PROJECT_DIR)
