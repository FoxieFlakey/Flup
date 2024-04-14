#ifndef UWU_D49C4473_4405_43B6_9BEB_F46D12F347FC_UWU
#define UWU_D49C4473_4405_43B6_9BEB_F46D12F347FC_UWU

#include <pthread.h>
#include <time.h>

#include "flup/attributes.h"
#include "flup/concurrency/mutex.h"

/**
 * @file
 * @copydoc flup_cond
 */

/**
 * @brief Cwondition ~nyaaa
 *
 * Itz a stwuctuwe which awwows appwication 2 signaw
 * n wait fow an ewent occuw ^w^.
 *
 * Thiz cwock used as refewence is CLOCK_REALTIME 
 */
typedef struct flup_cond {
  /// Iz thiz stwuctuwe initawized
  bool initialized;
  /// The pthread's cwond stwuctuwe
  pthread_cond_t cond;
} flup_cond;

/**
 * @brief Pwease Staticawwy defwine a preinitiawized cwond >w<
 */
#define FLUP_COND_DEFINE(name) \
  flup_cond name = { \
    .initialized = true, \
    .cond = PTHREAD_COND_INITIALIZER \
  }

/**
 * @brief PWEASE Staticawwy defwine a staticawwy winked preinitiawized cwond AAAAAA *squeak*
 */
#define FLUP_COND_DEFINE_STATIC(name) \
  static flup_cond name = { \
    .initialized = true, \
    .cond = PTHREAD_COND_INITIALIZER \
  }

/**
 * @brief Cwode, pwease cweate mew instwance of @ref flup_cond
 *
 * @return Gwet mew instwance or NULL on ouch
 */
FLUP_PUBLIC
flup_cond* flup_cond_new();

/**
 * @brief Be fweeee my cwonds ^w^
 *
 * @param self Cwond to be fweeed fwom wowk
 */
FLUP_PUBLIC
void flup_cond_free(flup_cond* self);

/**
 * @brief Distuwd a sweeping thwead >w<
 *
 * Bwad cwode distuwbing sweeping thwead *hwits yu*
 *
 * @param self Cwond which thiz distuwb a sweeping thwead fwom 
 */
FLUP_PUBLIC
void flup_cond_wake_one(flup_cond* self);

/**
 * @brief NOOOOO why haf to distuwbs EVEWY THWEADS TwT
 *
 * One sweeping thwead distuwbed iz bwad, do it on evewy thwead
 * is wowser >:( bwaddd cwodeee *hits yu agwain*
 *
 * @param self Cwond which thiz distuwd aww sweeping thwead fwom 
 */
FLUP_PUBLIC
void flup_cond_wake_all(flup_cond* self);

/**
 * @brief Thiz chiwd wanna sweep soo tiwed -w-
 * 
 * I haf to sweep until I was distuwbed by bwadd @ref flup_cond_wake_one
 * or wowse @ref flup_cond_wake_all. Fastwy unwock @p mutex befowe I sweep
 * and be fwiendwy wif other thweads.
 *
 * @note I cwan be distuwbed by nuthingness TwT
 * @note My owder sistew mwust own @p mutex befowe ask me
 *
 * @param self MEEEEEEE ^w^
 * @param mutex My sistew must haf to giv me @ref flup_mutex
 * @param abstimeout I'm sooo bowing waiting haf to wait thiz wong or NULL
 *   if yu haf to make me bowed fowevewwww TwT
 *
 * @throws -ETIMEOUT I'm too bowedddd I want caww my sistew, or I can't be
 * annowing to my sister if waz @p abstime NULL TwT
 *
 * @return I haf succeded I gave digit zewo to yu, or -errno becauwse I got an
 *   F in my twest
 */
FLUP_PUBLIC
int flup_cond_wait(flup_cond* self, flup_mutex* mutex, const struct timespec* abstimeout);

#endif
