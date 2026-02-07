/* xdelta3 - delta compression tools and library
   Copyright 2016 Joshua MacDonald

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Parallel processing support for xdelta3
*/

#ifndef _XDELTA3_PARALLEL_H_
#define _XDELTA3_PARALLEL_H_

#include <pthread.h>
#include <stdint.h>
#include "xdelta3.h"

/* Structure for passing work to threads */
typedef struct {
  const uint8_t *input;
  usize_t input_size;
  const uint8_t *source;
  usize_t source_size;
  uint8_t *output;
  usize_t *output_size;
  usize_t output_size_max;
  int flags;
  int result;
  int encode;  /* 1 for encode, 0 for decode */
} xd3_parallel_work_t;

/* Parallel encoding with multiple threads
 * Divides input into chunks and processes them in parallel
 * Returns 0 on success, error code on failure
 */
int xd3_encode_parallel(const uint8_t *input,
                       usize_t input_size,
                       const uint8_t *source,
                       usize_t source_size,
                       uint8_t *output,
                       usize_t *output_size,
                       usize_t output_size_max,
                       int flags,
                       int num_threads);

/* Parallel decoding with multiple threads
 * Returns 0 on success, error code on failure
 */
int xd3_decode_parallel(const uint8_t *input,
                       usize_t input_size,
                       const uint8_t *source,
                       usize_t source_size,
                       uint8_t *output,
                       usize_t *output_size,
                       usize_t output_size_max,
                       int flags,
                       int num_threads);

#endif /* _XDELTA3_PARALLEL_H_ */
