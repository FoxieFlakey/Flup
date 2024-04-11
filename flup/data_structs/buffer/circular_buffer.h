#ifndef UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU
#define UWU_D97A6D5D_56B3_4FDF_8623_0E075A3B7D06_UWU

#include <stddef.h>
#include <sys/types.h>

#include "flup/attributes.h"

/**
 * @file
 * @copydoc flup_circular_buffer
 */

/**
 * @brief An circular buffer
 * 
 * A simple fixed sized buffer which is optimized for first in last out
 * or essentially queue of data. It can be statically initialized with
 * fixed size.
 */
typedef struct flup_circular_buffer {
  /// Offset to current next read position
  size_t readOffset;
  /// Offset to current next write position
  size_t writeOffset;
  
  /// Total buffer size
  size_t bufferSize;
  /// Used buffer size
  size_t usedSize;
  /// Pointer to buffer of size @ref flup_circular_buffer.usedSize
  void* buffer;
} flup_circular_buffer;

/**
 * @brief Statically define a preinitialized circular buffer
 */
#define FLUP_CIRCULAR_BUFFER_DEFINE(name, _buffer, size) \
  flup_circular_buffer name = { \
    .readOffset = 0, \
    .writeOffset = 0, \
    .bufferSize = (size), \
    .buffer = (_buffer) \
  }

/**
 * @brief Statically define a statically linked preinitialized circular buffer
 */
#define FLUP_CIRCULAR_BUFFER_DEFINE_STATIC(name, _buffer, size) \
  static flup_circular_buffer name = { \
    .readOffset = 0, \
    .writeOffset = 0, \
    .bufferSize = (size), \
    .buffer = (_buffer) \
  }


/**
 * @brief Allocate a new circular buffer
 * @public @memberof flup_circular_buffer
 *
 * @param size Buffer size
 *
 * @return New instance or NULL if failed
 */
FLUP_PUBLIC
flup_circular_buffer* flup_circular_buffer_new(size_t size);

/**
 * @brief Free a circular buffer
 * @public @memberof flup_circular_buffer
 *
 * @param self The instance to be free'd
 */
FLUP_PUBLIC
void flup_circular_buffer_free(flup_circular_buffer* self);

/**
 * @brief Write data to buffer
 * @public @memberof flup_circular_buffer
 *
 * @param self The instance of the buffer
 * @param data The data to be written
 * @param size Size of data to be written
 *
 * @throws -ENOSPC Not enough space to write
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_circular_buffer_write(flup_circular_buffer* self, const void* data, size_t size);

/**
 * @brief Read data from buffer
 * @public @memberof flup_circular_buffer
 *
 * @param self The instance of the buffer
 * @param data The data to be read into
 * @param size Size of data to be read
 *
 * @throws -ENODATA Not enough data to be read
 *
 * @return 0 on success or -errno on error (see above)
 */
FLUP_PUBLIC
int flup_circular_buffer_read(flup_circular_buffer* self, void* data, size_t size);

#endif
