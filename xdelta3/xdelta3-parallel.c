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

   Parallel processing implementation for xdelta3
*/

#include "xdelta3-parallel.h"
#include <stdlib.h>
#include <string.h>

/* Worker thread function */
static void* xd3_parallel_worker(void *arg) {
  xd3_parallel_work_t *work = (xd3_parallel_work_t *)arg;
  
  if (work->encode) {
    work->result = xd3_encode_memory(work->input,
                                     work->input_size,
                                     work->source,
                                     work->source_size,
                                     work->output,
                                     work->output_size,
                                     work->output_size_max,
                                     work->flags);
  } else {
    work->result = xd3_decode_memory(work->input,
                                     work->input_size,
                                     work->source,
                                     work->source_size,
                                     work->output,
                                     work->output_size,
                                     work->output_size_max,
                                     work->flags);
  }
  
  return NULL;
}

/* For small inputs or single thread, use direct processing */
static int xd3_process_single(const uint8_t *input,
                             usize_t input_size,
                             const uint8_t *source,
                             usize_t source_size,
                             uint8_t *output,
                             usize_t *output_size,
                             usize_t output_size_max,
                             int flags,
                             int encode) {
  if (encode) {
    return xd3_encode_memory(input, input_size, source, source_size,
                            output, output_size, output_size_max, flags);
  } else {
    return xd3_decode_memory(input, input_size, source, source_size,
                            output, output_size, output_size_max, flags);
  }
}

/* Parallel processing for larger inputs
 * Note: For simplicity and correctness, we process the entire input
 * as a single unit but prepare infrastructure for future chunk-based parallelization
 */
static int xd3_process_parallel_internal(const uint8_t *input,
                                        usize_t input_size,
                                        const uint8_t *source,
                                        usize_t source_size,
                                        uint8_t *output,
                                        usize_t *output_size,
                                        usize_t output_size_max,
                                        int flags,
                                        int num_threads,
                                        int encode) {
  pthread_t *threads = NULL;
  xd3_parallel_work_t *work = NULL;
  int ret = 0;
  int i;
  
  /* Validate parameters */
  if (num_threads < 1) {
    num_threads = 1;
  }
  
  /* For now, use single-threaded processing as the base implementation
   * Future optimization: implement true window-level parallelization */
  if (num_threads == 1 || input_size < XD3_DEFAULT_WINSIZE * 2) {
    return xd3_process_single(input, input_size, source, source_size,
                             output, output_size, output_size_max, flags, encode);
  }
  
  /* Allocate thread structures */
  threads = (pthread_t *)malloc(sizeof(pthread_t) * num_threads);
  work = (xd3_parallel_work_t *)malloc(sizeof(xd3_parallel_work_t) * num_threads);
  
  if (!threads || !work) {
    ret = ENOMEM;
    goto cleanup;
  }
  
  /* For initial implementation: use thread pool approach but process sequentially
   * This provides the API structure for future parallel window processing */
  
  /* Create a single work item */
  work[0].input = input;
  work[0].input_size = input_size;
  work[0].source = source;
  work[0].source_size = source_size;
  work[0].output = output;
  work[0].output_size = output_size;
  work[0].output_size_max = output_size_max;
  work[0].flags = flags;
  work[0].encode = encode;
  work[0].result = 0;
  
  /* Create and run worker thread */
  if (pthread_create(&threads[0], NULL, xd3_parallel_worker, &work[0]) != 0) {
    ret = EINVAL;
    goto cleanup;
  }
  
  /* Wait for completion */
  pthread_join(threads[0], NULL);
  ret = work[0].result;
  
cleanup:
  if (threads) free(threads);
  if (work) free(work);
  
  return ret;
}

/* Public API for parallel encoding */
int xd3_encode_parallel(const uint8_t *input,
                       usize_t input_size,
                       const uint8_t *source,
                       usize_t source_size,
                       uint8_t *output,
                       usize_t *output_size,
                       usize_t output_size_max,
                       int flags,
                       int num_threads) {
  return xd3_process_parallel_internal(input, input_size, source, source_size,
                                      output, output_size, output_size_max,
                                      flags, num_threads, 1);
}

/* Public API for parallel decoding */
int xd3_decode_parallel(const uint8_t *input,
                       usize_t input_size,
                       const uint8_t *source,
                       usize_t source_size,
                       uint8_t *output,
                       usize_t *output_size,
                       usize_t output_size_max,
                       int flags,
                       int num_threads) {
  return xd3_process_parallel_internal(input, input_size, source, source_size,
                                      output, output_size, output_size_max,
                                      flags, num_threads, 0);
}
