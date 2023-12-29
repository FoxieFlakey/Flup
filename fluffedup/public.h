#ifndef header_1703854949_34a6b46a_97d6_4480_936b_977b424d8aa7_public_h
#define header_1703854949_34a6b46a_97d6_4480_936b_977b424d8aa7_public_h

#include "attributes.h"

#ifndef PUBLIC
# define PUBLIC ATTRIBUTE_USED() ATTRIBUTE((visibility("default"))) extern
#endif

#endif

