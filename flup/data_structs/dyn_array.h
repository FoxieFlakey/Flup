#ifndef header_1705728056_a27e30b2_3cba_46ed_8603_f86729fdf4ca_dyn_array_h
#define header_1705728056_a27e30b2_3cba_46ed_8603_f86729fdf4ca_dyn_array_h

/**
 * @file
 * @copydoc flup_dyn_array
 */

#include <stddef.h>

#include "flup/attributes.h"
#include "flup/interface/ilist.h"
#include "flup/tags.h"

/**
 * @brief Dynamicly expanding array
 *
 * @implements flup_ilist
 */
typedef struct flup_dyn_array {
  /// Interfaces implemented
  struct {
    /// Implementation of list interface
    flup_ilist IList;
  } interface;

  /// Length of array
  size_t length;
  /// Capacity of array (including reserves)
  size_t capacity;
  /// Sizs of each element
  size_t elementSize;
  /// Pointer to allocated array
  void* array;
} flup_dyn_array;

/**
 * @brief Create new instance of @ref flup_dyn_array
 * @public @memberof flup_dyn_array
 *
 * @param elementSize Size of each element
 * @param capacity Pre-allocated count
 *
 * @throws NULL Out of memory while allocating new instance
 *
 * @return New instance of @ref flup_dyn_array
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
flup_dyn_array* flup_dyn_array_new(size_t elementSize, size_t capacity);

/**
 * @brief Reserve @p count elements
 * @public @memberof flup_dyn_array
 *
 * Reserves @p counf elements so any insertion
 * will never cause out-of-memory error.
 *
 * @param self Instance of @ref flup_dyn_array
 * @param count Number of elements to be pre-allocate/reserve
 *
 * @throws ENOMEM Out of memory while reserving
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_reserve(flup_dyn_array* self, size_t count);

/**
 * @brief Free an instance of @ref flup_dyn_array
 * @public @memberof flup_dyn_array
 *
 * @param self The @ref flup_dyn_array instance
 */
FLUP_DEALLOCS_MEM
FLUP_PUBLIC
void flup_dyn_array_free(flup_dyn_array* self);

/**
 * @brief Get an element
 * @public @memberof flup_dyn_array
 *
 * Gets an element at @p index and saves it to @p element
 *
 * @param self Instance of  @ref flup_dyn_array to retrieve
 *   element from
 * @param index Index of the element
 * @param[out] element The element retrieved
 *
 * @return true on success, false on out of bound
 */
FLUP_PUBLIC
int flup_dyn_array_get(flup_dyn_array* self, size_t index, void** element);

/**
 * @brief Append an element
 * @public @memberof flup_dyn_array
 *
 * Appends @p element to the end of @p self array
 *
 * @param self Instance of @ref flup_dyn_array to append to
 * @param[in] element Pointer to element to be appended to array
 *
 * @throws ENOMEM Out of memory while appending
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_append(flup_dyn_array* self, const void* element);

/**
 * @brief Prepend an element
 * @public @memberof flup_dyn_array
 *
 * Prepends @p element to the end of @p self array
 *
 * @param self Instance of @ref flup_dyn_array to prepend to
 * @param[in] element Pointer to element to be prepended to array
 *
 * @throws ENOMEM Out of memory while prepending
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_prepend(flup_dyn_array* self, const void* element);

/**
 * @brief Set an element
 * @public @memberof flup_dyn_array
 *
 * Sets an element at @p index
 *
 * @param self Instance of @ref flup_dyn_array to set on
 * @param index Index where element be set to
 * @param[in] element Element which new content at @p index  will be
 *
 * @throws EINVAL @p index points to outside of array
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_PUBLIC
int flup_dyn_array_set(flup_dyn_array* self, size_t index, const void* element);

/**
 * @brief Trims excess capacity
 * @public @memberof flup_dyn_array
 *
 * Trims array so that excess capacity is freed or no-op
 * if array is already at smallest capacity
 *
 * @param self Instance of @ref flup_dyn_array to trim on
 *
 * @throws ENOMEM Out of memory during shrinking
 *
 * @returns 0 on success, -errno on error (see above)
 */
FLUP_DEALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_trim(flup_dyn_array* self);

/**
 * @brief Remove n element
 * @public @memberof flup_dyn_array
 *
 * Removes an element at @p index
 *
 * @param self Instance of @ref flup_dyn_array
 * @param index Index where element to be removed
 * @param count Amount of elements to be removed
 *
 * @note This function does not trim the array
 *
 * @throws EINVAL Range to be removed which is [@p index, @p index + @p count)
 *   is out of array
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_PUBLIC
int flup_dyn_array_remove(flup_dyn_array* self, size_t index, size_t count);

/**
 * @brief Inserts an element
 * @public @memberof flup_dyn_array
 *
 * Insert an element at @p index and moves other element
 * to make up a space for new element
 *
 * @param self Instance of @ref flup_dyn_array 
 * @param index Index where element be inserted at
 * @param[in] element Element which content at @p index will be
 *
 * @throws ENOMEM Out of memory
 * @throws EINVAL @p index points to outside of array
 *
 * @return 0 on success, -errno on error (see above)
 */
FLUP_ALLOCS_MEM
FLUP_PUBLIC
int flup_dyn_array_insert(flup_dyn_array* self, size_t index, const void* element);

#endif

