#ifndef header_1706007321_a724a59c_4fab_4bf8_afd3_2f00c5df4a22_typecheck_h
#define header_1706007321_a724a59c_4fab_4bf8_afd3_2f00c5df4a22_typecheck_h

// Takes the desired type and returns pointer to "var"
#define FLUP_CHECK_VAR_TYPE(var, ...) \
  (*((struct {__VA_ARGS__* __flup_check_ptr;}) \
     {&(var)}).__flup_check_ptr)

#define FLUP_CHECK_VAR_FUNC_LIKE_TYPE(var, prefix, ...) \
  (*((struct {prefix *__flup_check_ptr __VA_ARGS__;}) \
     {&(var)}).__flup_check_ptr)

#endif

