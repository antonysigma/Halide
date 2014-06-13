#include "mini_stdint.h"
#include "../buffer_t.h"
#include "HalideRuntime.h"

extern "C" {

extern int atoi(const char *);
extern char *getenv(const char *);
extern int64_t halide_current_time_ns(void *user_context);
extern void *malloc(size_t);
extern int snprintf(char *, size_t, const char *, ...);

#ifdef DEBUG
#define DEBUG_PRINTF halide_printf
#else
// This ensures that DEBUG and non-DEBUG have the same semicolon eating behavior.
static void __noop_printf(void *, const char *, ...) { }
#define DEBUG_PRINTF __noop_printf
#endif

// TODO: On windows this needs __stdcall
//#define CUDAAPI __stdcall
#define CUDAAPI

// API version > 3020
#define cuCtxCreate                         cuCtxCreate_v2
#define cuMemAlloc                          cuMemAlloc_v2
#define cuMemFree                           cuMemFree_v2
#define cuMemcpyHtoD                        cuMemcpyHtoD_v2
#define cuMemcpyDtoH                        cuMemcpyDtoH_v2
// API version >= 4000
#define cuCtxDestroy                        cuCtxDestroy_v2
#define cuCtxPopCurrent                     cuCtxPopCurrent_v2
#define cuCtxPushCurrent                    cuCtxPushCurrent_v2
#define cuStreamDestroy                     cuStreamDestroy_v2
#define cuEventDestroy                      cuEventDestroy_v2

#ifdef BITS_64
typedef unsigned long long CUdeviceptr;
#else
typedef unsigned int CUdeviceptr;
#endif

typedef int CUdevice;                                     /**< CUDA device */
typedef struct CUctx_st *CUcontext;                       /**< CUDA context */
typedef struct CUmod_st *CUmodule;                        /**< CUDA module */
typedef struct CUfunc_st *CUfunction;                     /**< CUDA function */
typedef struct CUstream_st *CUstream;                     /**< CUDA stream */
typedef struct CUevent_st *CUevent;                       /**< CUDA event */
typedef enum {
    CUDA_SUCCESS                              = 0,
    CUDA_ERROR_INVALID_VALUE                  = 1,
    CUDA_ERROR_OUT_OF_MEMORY                  = 2,
    CUDA_ERROR_NOT_INITIALIZED                = 3,
    CUDA_ERROR_DEINITIALIZED                  = 4,
    CUDA_ERROR_PROFILER_DISABLED           = 5,
    CUDA_ERROR_PROFILER_NOT_INITIALIZED       = 6,
    CUDA_ERROR_PROFILER_ALREADY_STARTED       = 7,
    CUDA_ERROR_PROFILER_ALREADY_STOPPED       = 8,
    CUDA_ERROR_NO_DEVICE                      = 100,
    CUDA_ERROR_INVALID_DEVICE                 = 101,
    CUDA_ERROR_INVALID_IMAGE                  = 200,
    CUDA_ERROR_INVALID_CONTEXT                = 201,
    CUDA_ERROR_CONTEXT_ALREADY_CURRENT        = 202,
    CUDA_ERROR_MAP_FAILED                     = 205,
    CUDA_ERROR_UNMAP_FAILED                   = 206,
    CUDA_ERROR_ARRAY_IS_MAPPED                = 207,
    CUDA_ERROR_ALREADY_MAPPED                 = 208,
    CUDA_ERROR_NO_BINARY_FOR_GPU              = 209,
    CUDA_ERROR_ALREADY_ACQUIRED               = 210,
    CUDA_ERROR_NOT_MAPPED                     = 211,
    CUDA_ERROR_NOT_MAPPED_AS_ARRAY            = 212,
    CUDA_ERROR_NOT_MAPPED_AS_POINTER          = 213,
    CUDA_ERROR_ECC_UNCORRECTABLE              = 214,
    CUDA_ERROR_UNSUPPORTED_LIMIT              = 215,
    CUDA_ERROR_CONTEXT_ALREADY_IN_USE         = 216,
    CUDA_ERROR_INVALID_SOURCE                 = 300,
    CUDA_ERROR_FILE_NOT_FOUND                 = 301,
    CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND = 302,
    CUDA_ERROR_SHARED_OBJECT_INIT_FAILED      = 303,
    CUDA_ERROR_OPERATING_SYSTEM               = 304,
    CUDA_ERROR_INVALID_HANDLE                 = 400,
    CUDA_ERROR_NOT_FOUND                      = 500,
    CUDA_ERROR_NOT_READY                      = 600,
    CUDA_ERROR_LAUNCH_FAILED                  = 700,
    CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES        = 701,
    CUDA_ERROR_LAUNCH_TIMEOUT                 = 702,
    CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING  = 703,
    CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED = 704,
    CUDA_ERROR_PEER_ACCESS_NOT_ENABLED    = 705,
    CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE         = 708,
    CUDA_ERROR_CONTEXT_IS_DESTROYED           = 709,
    CUDA_ERROR_UNKNOWN                        = 999
} CUresult;

#define CU_POINTER_ATTRIBUTE_CONTEXT 1

CUresult CUDAAPI cuInit(unsigned int Flags);
CUresult CUDAAPI cuDeviceGetCount(int *count);
CUresult CUDAAPI cuDeviceGet(CUdevice *device, int ordinal);
CUresult CUDAAPI cuCtxCreate(CUcontext *pctx, unsigned int flags, CUdevice dev);
CUresult CUDAAPI cuCtxDestroy(CUcontext pctx);
CUresult CUDAAPI cuModuleLoadData(CUmodule *module, const void *image);
CUresult CUDAAPI cuModuleUnload(CUmodule module);
CUresult CUDAAPI cuModuleGetFunction(CUfunction *hfunc, CUmodule hmod, const char *name);
CUresult CUDAAPI cuMemAlloc(CUdeviceptr *dptr, size_t bytesize);
CUresult CUDAAPI cuMemFree(CUdeviceptr dptr);
CUresult CUDAAPI cuMemcpyHtoD(CUdeviceptr dstDevice, const void *srcHost, size_t ByteCount);
CUresult CUDAAPI cuMemcpyDtoH(void *dstHost, CUdeviceptr srcDevice, size_t ByteCount);
CUresult CUDAAPI cuLaunchKernel(CUfunction f,
                                unsigned int gridDimX,
                                unsigned int gridDimY,
                                unsigned int gridDimZ,
                                unsigned int blockDimX,
                                unsigned int blockDimY,
                                unsigned int blockDimZ,
                                unsigned int sharedMemBytes,
                                CUstream hStream,
                                void **kernelParams,
                                void **extra);
CUresult CUDAAPI cuCtxSynchronize();

CUresult CUDAAPI cuCtxPushCurrent(CUcontext ctx);
CUresult CUDAAPI cuCtxPopCurrent(CUcontext *pctx);

CUresult CUDAAPI cuEventRecord(CUevent hEvent, CUstream hStream);
CUresult CUDAAPI cuEventCreate(CUevent *phEvent, unsigned int Flags);
CUresult CUDAAPI cuEventDestroy(CUevent phEvent);
CUresult CUDAAPI cuEventSynchronize(CUevent hEvent);
CUresult CUDAAPI cuEventElapsedTime(float *pMilliseconds, CUevent hStart, CUevent hEnd);
CUresult CUDAAPI cuPointerGetAttribute(void *result, int query, CUdeviceptr ptr);

}
extern "C" {

// A cuda context defined in this module with weak linkage
CUcontext WEAK weak_cuda_ctx = 0;
volatile int WEAK weak_cuda_lock = 0;

// A pointer to the cuda context to use, which may not be the one above. This pointer is followed at init_kernels time.
CUcontext WEAK *cuda_ctx_ptr = NULL;
volatile int WEAK *cuda_lock_ptr = NULL;

WEAK void halide_set_cuda_context(CUcontext *ctx_ptr, volatile int *lock_ptr) {
    cuda_ctx_ptr = ctx_ptr;
    cuda_lock_ptr = lock_ptr;
}

static CUresult create_context(void *user_context, CUcontext *ctx);

// The default implementation of halide_acquire_cl_context uses the global
// pointers above, and serializes access with a spin lock.
// Overriding implementations of acquire/release must implement the following
// behavior:
// - halide_acquire_cl_context should always store a valid context/command
//   queue in ctx/q, or return an error code.
// - A call to halide_acquire_cl_context is followed by a matching call to
//   halide_release_cl_context. halide_acquire_cl_context should block while a
//   previous call (if any) has not yet been released via halide_release_cl_context.
WEAK int halide_acquire_cuda_context(void *user_context, CUcontext *ctx) {
    // TODO: Should we use a more "assertive" assert? these asserts do
    // not block execution on failure.
    halide_assert(user_context, ctx != NULL);

    if (cuda_ctx_ptr == NULL) {
        cuda_ctx_ptr = &weak_cuda_ctx;
        cuda_lock_ptr = &weak_cuda_lock;
    }

    halide_assert(user_context, cuda_lock_ptr != NULL);
    while (__sync_lock_test_and_set(cuda_lock_ptr, 1)) { }

    // If the context has not been initialized, initialize it now.
    halide_assert(user_context, cuda_ctx_ptr != NULL);
    if (*cuda_ctx_ptr == NULL) {
        CUresult error = create_context(user_context, cuda_ctx_ptr);
        if (error != CUDA_SUCCESS) {
            __sync_lock_release(cuda_lock_ptr);
            return error;
        }
    }

    *ctx = *cuda_ctx_ptr;
    return 0;
}

WEAK int halide_release_cuda_context(void *user_context) {
    __sync_lock_release(cuda_lock_ptr);
    return 0;
}

}

// Helper object to acquire and release the OpenCL context.
class CudaContext {
    void *user_context;

public:
    CUcontext context;
    int error;

    // Constructor sets 'error' if any occurs.
    CudaContext(void *user_context) : user_context(user_context),
                                      context(NULL),
                                      error(CUDA_SUCCESS) {
        error = halide_acquire_cuda_context(user_context, &context);
        halide_assert(user_context, context != NULL);
        if (error != 0) {
            return;
        }

        error = cuCtxPushCurrent(context);
    }

    ~CudaContext() {
        CUcontext old;
        cuCtxPopCurrent(&old);

        halide_release_cuda_context(user_context);
    }
};

extern "C" {
// Structure to hold the state of a module attached to the context.
// Also used as a linked-list to keep track of all the different
// modules that are attached to a context in order to release them all
// when then context is released.
struct _module_state_ WEAK *state_list = NULL;
typedef struct _module_state_ {
    CUmodule module;
    _module_state_ *next;
} module_state;

WEAK bool halide_validate_dev_pointer(void *user_context, buffer_t* buf, size_t size=0) {
// The technique using cuPointerGetAttribute and CU_POINTER_ATTRIBUTE_CONTEXT
// requires unified virtual addressing is enabled and that is not the case
// for 32-bit processes on Mac OS X. So for now, as a total hack, just return true
// in 32-bit. This could of course be wrong the other way for cards that only
// support 32-bit addressing in 64-bit processes, but I expect those cards do not
// support unified addressing at all.
// TODO: figure out a way to validate pointers in all cases if strictly necessary.
#ifdef BITS_32
    return true;
#else
    if (buf->dev == 0)
        return true;

    CUcontext ctx;
    CUresult result = cuPointerGetAttribute(&ctx, CU_POINTER_ATTRIBUTE_CONTEXT, buf->dev);
    if (result) {
        halide_printf(user_context, "Bad device pointer %p: cuPointerGetAttribute returned %d\n",
                      (void *)buf->dev, result);
        return false;
    }
    return true;
#endif
}

WEAK int halide_dev_free(void *user_context, buffer_t* buf) {
    DEBUG_PRINTF( user_context, "CUDA: halide_dev_free (user_context: %p, buf: %p)\n", user_context, buf );

    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS)
        return ctx.error;

    // halide_dev_free, at present, can be exposed to clients and they
    // should be allowed to call halide_dev_free on any buffer_t
    // including ones that have never been used with a GPU.
    if (buf->dev == 0)
        return 0;

    #ifdef DEBUG
    uint64_t t_before = halide_current_time_ns(user_context);
    #endif

    halide_assert(user_context, halide_validate_dev_pointer(user_context, buf));

    CUresult err = cuMemFree(buf->dev);
    // If cuMemFree fails, it isn't likely to succeed later, so just drop
    // the reference.
    buf->dev = 0;
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuMemFree failed (%d)", err);
        return err;
    }

    #ifdef DEBUG
    uint64_t t_after = halide_current_time_ns(user_context);
    halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
    #endif

    return 0;
}

static CUresult create_context(void *user_context, CUcontext *ctx) {
    // Initialize CUDA
    CUresult err = cuInit(0);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuInit failed (%d)", err);
        return err;
    }

    // Make sure we have a device
    int deviceCount = 0;
    err = cuDeviceGetCount(&deviceCount);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuGetDeviceCount failed (%d)", err);
        return err;
    }
    if (deviceCount <= 0) {
        halide_error(user_context, "CUDA: No devices available");
        return CUDA_ERROR_NO_DEVICE;
    }

    char *device_str = getenv("HL_GPU_DEVICE");

    CUdevice dev;
    // Get device
    CUresult status;
    if (device_str) {
        status = cuDeviceGet(&dev, atoi(device_str));
    } else {
        // Try to get a device >0 first, since 0 should be our display device
        // For now, don't try devices > 2 to maintain compatibility with previous behavior.
        if (deviceCount > 2)
            deviceCount = 2;
        for (int id = deviceCount - 1; id >= 0; id--) {
            status = cuDeviceGet(&dev, id);
            if (status == CUDA_SUCCESS) break;
        }
    }
    if (status != CUDA_SUCCESS) {
        halide_error(user_context, "CUDA: Failed to get device\n");
        return status;
    }

    DEBUG_PRINTF( user_context, "    Got device %d\n", dev );

    // Create context
    err = cuCtxCreate(ctx, 0, dev);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuCtxCreate failed (%d)", err);
        return err;
    }
    DEBUG_PRINTF( user_context, "    Created context %p", *ctx );

    return CUDA_SUCCESS;
}

WEAK void* halide_init_kernels(void *user_context, void *state_ptr, const char* ptx_src, int size) {
    DEBUG_PRINTF( user_context, "CUDA: halide_init_kernels (user_context: %p, state_ptr: %p, ptx_src: %p, %i)\n",
                  user_context, state_ptr, ptx_src, size );

    CudaContext ctx(user_context);
    if (ctx.error != 0) {
        return NULL;
    }

    #ifdef DEBUG
    uint64_t t_before = halide_current_time_ns(user_context);
    #endif

    module_state *state = (module_state*)state_ptr;

    // Create the module itself if necessary.
    if (!(state && state->module)) {
        CUmodule module;
        CUresult err = cuModuleLoadData(&module, ptx_src);
        if (err != CUDA_SUCCESS) {
            halide_error_varargs(user_context, "CUDA: cuModuleLoadData failed (%d)", err);
            return NULL;
        }

        // Create the module state if necessary
        if (!state) {
            state = (module_state*)malloc(sizeof(module_state));
        }

        // Add module to state list
        state->module = module;
        state->next = state_list;
        state_list = state;
    }

    #ifdef DEBUG
    uint64_t t_after = halide_current_time_ns(user_context);
    halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
    #endif

    return state;
}

WEAK void halide_release(void *user_context) {
    DEBUG_PRINTF( user_context, "CUDA: halide_release (user_context: %p)\n", user_context );

    int err;
    CUcontext ctx;
    err = halide_acquire_cuda_context(user_context, &ctx);
    if (err != CUDA_SUCCESS || !ctx) {
        return;
    }

    // It's possible that this is being called from the destructor of
    // a static variable, in which case the driver may already be
    // shutting down.
    err = cuCtxSynchronize();
    halide_assert(user_context, err == CUDA_SUCCESS || err == CUDA_ERROR_DEINITIALIZED);

    // Unload the modules attached to this context
    module_state *state = state_list;
    while (state) {
        if (state->module) {
            DEBUG_PRINTF(user_context, "    cuModuleUnload %p\n", state->module);
            err = cuModuleUnload(state->module);
            halide_assert(user_context, err == CUDA_SUCCESS || err == CUDA_ERROR_DEINITIALIZED);
            state->module = 0;
        }
        state = state->next;
    }

    // Only destroy the context if we own it
    if (ctx == weak_cuda_ctx) {
        DEBUG_PRINTF(user_context, "    cuCtxDestroy %p\n", weak_cuda_ctx);
        err = cuCtxDestroy(weak_cuda_ctx);
        halide_assert(user_context, err == CUDA_SUCCESS || err == CUDA_ERROR_DEINITIALIZED);
        weak_cuda_ctx = NULL;
    }

    halide_release_cuda_context(user_context);
}

static size_t __buf_size(void *user_context, buffer_t *buf) {
    size_t size = 0;
    for (size_t i = 0; i < sizeof(buf->stride) / sizeof(buf->stride[0]); i++) {
        size_t total_dim_size = buf->elem_size * buf->extent[i] * buf->stride[i];
        if (total_dim_size > size) {
            size = total_dim_size;
        }
    }
    halide_assert(user_context, size);
    return size;
}

WEAK int halide_dev_malloc(void *user_context, buffer_t *buf) {
    DEBUG_PRINTF( user_context, "CUDA: halide_dev_malloc (user_context: %p, buf: %p)\n", user_context, buf );

    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS) {
        return ctx.error;
    }

    size_t size = __buf_size(user_context, buf);
    if (buf->dev) {
        // This buffer already has a device allocation
        halide_assert(user_context, halide_validate_dev_pointer(user_context, buf, size));
        return 0;
    }

    DEBUG_PRINTF(user_context, "    allocating buffer of %lld bytes, "
                 "extents: %lldx%lldx%lldx%lld strides: %lldx%lldx%lldx%lld (%d bytes per element)\n",
                 (long long)size,
                 (long long)buf->extent[0], (long long)buf->extent[1],
                 (long long)buf->extent[2], (long long)buf->extent[3],
                 (long long)buf->stride[0], (long long)buf->stride[1],
                 (long long)buf->stride[2], (long long)buf->stride[3],
                 buf->elem_size);

    #ifdef DEBUG
    uint64_t t_before = halide_current_time_ns(user_context);
    #endif

    CUdeviceptr p;
    CUresult err = cuMemAlloc(&p, size);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuMemAlloc failed (%d)", err);
        return err;
    }
    halide_assert(user_context, p);
    buf->dev = (uint64_t)p;

    #ifdef DEBUG
    uint64_t t_after = halide_current_time_ns(user_context);
    halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
    #endif

    return 0;
}

WEAK int halide_copy_to_dev(void *user_context, buffer_t* buf) {
    int err = halide_dev_malloc(user_context, buf);
    if (err) {
        return err;
    }

    DEBUG_PRINTF( user_context, "CUDA: halide_copy_to_dev (user_context: %p, buf: %p)\n", user_context, buf );

    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS) {
        return ctx.error;
    }

    if (buf->host_dirty) {
        #ifdef DEBUG
        uint64_t t_before = halide_current_time_ns(user_context);
        #endif

        halide_assert(user_context, buf->host && buf->dev);
        size_t size = __buf_size(user_context, buf);
        DEBUG_PRINTF( user_context, "    copy_to_dev (%lld bytes, %p -> %p)\n",
                      (long long)size, buf->host, (void *)buf->dev );
        halide_assert(user_context, halide_validate_dev_pointer(user_context, buf));
        CUresult err = cuMemcpyHtoD(buf->dev, buf->host, size);
        if (err != CUDA_SUCCESS) {
            halide_error_varargs(user_context, "CUDA: cuMemcpyHtoD failed (%d)", err);
            return err;
        }

        #ifdef DEBUG
        uint64_t t_after = halide_current_time_ns(user_context);
        halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
        #endif
    }
    buf->host_dirty = false;
    return 0;
}

WEAK int halide_copy_to_host(void *user_context, buffer_t* buf) {
    DEBUG_PRINTF( user_context, "CUDA: halide_copy_to_host (user_context: %p, buf: %p)\n", user_context, buf );

    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS) {
        return ctx.error;
    }

    if (buf->dev_dirty) {
        #ifdef DEBUG
        uint64_t t_before = halide_current_time_ns(user_context);
        #endif

        halide_assert(user_context, buf->dev && buf->dev);
        size_t size = __buf_size(user_context, buf);
        DEBUG_PRINTF( user_context, "    copy_to_host (%lld bytes, %p -> %p)\n",
                      (long long)size, buf->host, (void *)buf->dev );
        halide_assert(user_context, halide_validate_dev_pointer(user_context, buf));
        CUresult err = cuMemcpyDtoH(buf->host, buf->dev, size);
        if (err != CUDA_SUCCESS) {
            halide_error_varargs(user_context, "CUDA: cuMemcpyDtoH failed (%d)", err);
            return err;
        }

        #ifdef DEBUG
        uint64_t t_after = halide_current_time_ns(user_context);
        halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
        #endif
    }
    buf->dev_dirty = false;
    return 0;
}

// Used to generate correct timings when tracing
WEAK int halide_dev_sync(void *user_context) {
    DEBUG_PRINTF( user_context, "CUDA: halide_dev_sync (user_context: %p)\n", user_context );

    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS) {
        return ctx.error;
    }

    #ifdef DEBUG
    uint64_t t_before = halide_current_time_ns(user_context);
    #endif

    CUresult err = cuCtxSynchronize();
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuCtxSynchronize failed (%d)", err);
        return err;
    }

    #ifdef DEBUG
    uint64_t t_after = halide_current_time_ns(user_context);
    halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
    #endif

    return 0;
}

WEAK int halide_dev_run(void *user_context,
                        void *state_ptr,
                        const char* entry_name,
                        int blocksX, int blocksY, int blocksZ,
                        int threadsX, int threadsY, int threadsZ,
                        int shared_mem_bytes,
                        size_t arg_sizes[],
                        void* args[]) {
    DEBUG_PRINTF( user_context, "CUDA: halide_dev_run (user_context: %p, entry: %s, blocks: %dx%dx%d, threads: %dx%dx%d, shmem: %d)\n",
                  user_context, entry_name,
                  blocksX, blocksY, blocksZ,
                  threadsX, threadsY, threadsZ,
                  shared_mem_bytes );

    CUresult err;
    CudaContext ctx(user_context);
    if (ctx.error != CUDA_SUCCESS) {
        return ctx.error;
    }

    #ifdef DEBUG
    uint64_t t_before = halide_current_time_ns(user_context);
    #endif

    halide_assert(user_context, state_ptr);
    CUmodule mod = ((module_state*)state_ptr)->module;
    halide_assert(user_context, mod);
    CUfunction f;
    err = cuModuleGetFunction(&f, mod, entry_name);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuModuleGetFunction failed (%d)", err);
        return err;
    }

    err = cuLaunchKernel(f,
                         blocksX,  blocksY,  blocksZ,
                         threadsX, threadsY, threadsZ,
                         shared_mem_bytes,
                         NULL, // stream
                         args,
                         NULL);
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuLaunchKernel failed (%d)", err);
        return err;
    }

    #ifdef DEBUG
    err = cuCtxSynchronize();
    if (err != CUDA_SUCCESS) {
        halide_error_varargs(user_context, "CUDA: cuCtxSynchronize failed (%d)\n", err);
        return err;
    }
    uint64_t t_after = halide_current_time_ns(user_context);
    halide_printf(user_context, "    Time: %f ms\n", (t_after - t_before) / 1.0e6);
    #endif
    return 0;
}

} // extern "C" linkage
