#ifndef header_1703857190_fc62eeb6_9d57_43cc_be4a_90c105f4145e_container_of_h
#define header_1703857190_fc62eeb6_9d57_43cc_be4a_90c105f4145e_container_of_h

/**
 * @file
 * @brief container_of macro
 *
 * @see @ref container_of
 */

/**
 * @def container_of
 * @brief Get container pointer from pointer to member.
 *
 * Gets pointer to container from pointer to member with
 * pointer magic UwU.
 *
 * @param ptr Pointer of the pointer to member
 * @param type Type name of container
 * @param member The member name of where the @p ptr coming from
 */
# define container_of(ptr, type, member) \
  ((type *)((void*) ( (char *)__mptr - offsetof(type,member) )))

#ifdef __GNUC__
#undef container_of
# define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)((void*) ((char *)__mptr - offsetof(type,member) ));})
#endif

#endif

