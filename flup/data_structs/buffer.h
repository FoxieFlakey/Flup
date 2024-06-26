#ifndef header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h
#define header_1705715261_58beea09_3c90_46da_8632_592919891b01_queue_h

#include <stddef.h>

/**
 * @file
 * @copydoc flup_buffer
 */

#include "flup/attributes.h"
#include "flup/concurrency/cond.h"
#include "flup/concurrency/mutex.h"
#include "flup/data_structs/buffer/circular_buffer.h"

/**
 * @brief An thread safe buffer
 *
 * This thread safe buffer only atomic (in respect to other read and writes
 * to the same buffer) only if the size is less than or equal whole buffer
 * size and write or read request going to be rejected if larger
 */
typedef struct flup_buffer {
  /// Lock protecting access to buffer
  flup_mutex* lock;
  /// An event for a new data added
  flup_cond* dataWrittenEvent;
  /// An event for a data has read
  flup_cond* dataReadEvent;
  
  /// The backing non thread safe buffer
  flup_circular_buffer* buffer;
} flup_buffer;

/**
 * @brief Create new instance of buffer
 * @public @memberof flup_buffer
 *
 * @param capacityInBytes Size of buffer in bytes
 *
 * @return New instance of the buffer
 */
FLUP_PUBLIC
flup_buffer* flup_buffer_new(size_t capacityInBytes);

/**
 * @brief Deallocate a buffer
 * @public @memberof flup_buffer
 *
 * @param self The instance of buffer to be freed
 */
FLUP_PUBLIC
void flup_buffer_free(flup_buffer* self);

/**
 * @brief Write a data into buffer
 * @public @memberof flup_buffer
 *
 * @param self The instance of buffer
 * @param data The pointer to the data to be written
 * @param size The size of data to be written
 *
 * @throws -EMSGSIZE The data is too big to be written atomicly
 *
 * @return 0 on success or -errno on failure (no data written on failure)
 */
FLUP_PUBLIC
int flup_buffer_write(flup_buffer* self, const void* data, size_t size);

/**
 * @brief Read a data from buffer
 * @public @memberof flup_buffer
 *
 * @param self The instance of buffer
 * @param readData The pointer to the data to be read
 * @param size The size of data to be read
 *
 * @throws -EMSGSIZE The data is too big to be read atomicly
 *
 * @return 0 on success or -errno on failure (no data read on failure)
 */
FLUP_PUBLIC
int flup_buffer_read(flup_buffer* self, void* readData, size_t size);

/**
 * @brief Read buffer if buffer can be read without waiting for data
 *
 * Flag for @ref flup_buffer.flup_buffer_read2 to read
 * only if it can be read without waiting for data
 */
#define FLUP_BUFFER_READ2_DONT_WAIT_FOR_DATA (0x01)

/**
 * @brief Read a data from buffer (with extra flags)
 * @public @memberof flup_buffer
 *
 * If @p flags is zero then its the same as @ref flup_buffer.flup_buffer_read
 *
 * @param self The instance of buffer
 * @param readData The pointer to the data to be read
 * @param size The size of data to be read
 * @param flags Bitmask of flags to tune behaviour
 *
 * @throws -EMSGSIZE The data is too big to be read atomicly
 * @throws -ENODATA The buffer was empty
 *
 * @return 0 on success or -errno on failure (no data read on failure)
 */
FLUP_PUBLIC
int flup_buffer_read2(flup_buffer* self, void* readData, size_t size, unsigned int flags);

/**
 * @brief Wait until buffer empty
 * @public @memberof flup_buffer
 *
 * @param self Buffer to wait for
 */
FLUP_PUBLIC
void flup_buffer_flush(flup_buffer* self);

/**
 * @brief Never fail version of @ref flup_buffer_write
 * @public @memberof flup_buffer
 *
 * Behaves exactly like @ref flup_buffer_write but never
 * fail, if it somehow fail, it will panic. Its for codes
 * which don't expect error and fine with panic on edge
 * case or buggy codes.
 *
 * @param self The instance of buffer
 * @param data The pointer to the data to be written
 * @param size The size of data to be written
 */
FLUP_PUBLIC
void flup_buffer_write_no_fail(flup_buffer* self, const void* data, size_t size);

/**
 * @brief Never fail version of @ref flup_buffer_read
 * @public @memberof flup_buffer
 *
 * Behaves exactly like @ref flup_buffer_read but never
 * fail, if it somehow fail, it will panic. Its for codes
 * which don't expect error and fine with panic on edge
 * case or buggy codes.
 *
 * @param self The instance of buffer
 * @param readData The pointer to the data to be read
 * @param size The size of data to be read
 */
FLUP_PUBLIC
void flup_buffer_read_no_fail(flup_buffer* self, void* readData, size_t size);

#endif

