/* xdelta3_all.c - Wrapper to build everything together for Python bindings */

/* First include xdelta3.c which has all the core functions */
#include "xdelta3.c"

/* Then include parallel functions which depend on xdelta3 types */
#ifdef XDELTA3_PARALLEL

#include <pthread.h>

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

/* Parallel processing for larger inputs */
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
  
  /* Validate parameters */
  if (num_threads < 1) {
    num_threads = 1;
  }
  
  /* For now, use single-threaded processing as the base implementation */
  if (num_threads == 1 || input_size < XD3_DEFAULT_WINSIZE * 2) {
    return xd3_process_single(input, input_size, source, source_size,
                             output, output_size, output_size_max, flags, encode);
  }
  
  /* Allocate thread structures */
  threads = (pthread_t *)malloc(sizeof(pthread_t));
  work = (xd3_parallel_work_t *)malloc(sizeof(xd3_parallel_work_t));
  
  if (!threads || !work) {
    ret = ENOMEM;
    goto cleanup;
  }
  
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

#endif /* XDELTA3_PARALLEL */
