#ifndef header_1703857190_fc62eeb6_9d57_43cc_be4a_90c105f4145e_container_of_h
#define header_1703857190_fc62eeb6_9d57_43cc_be4a_90c105f4145e_container_of_h

#ifdef __GNUC__
# define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)((void*) ((char *)__mptr - offsetof(type,member) ));})
#else
# define container_of(ptr, type, member) \
  ((type *)((void*) ( (char *)__mptr - offsetof(type,member) )))
#endif

#endif

