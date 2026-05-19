#include "KSPlayerONNXRuntimeSupport.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

#define KSORT_API_VERSION 24
#define KSORT_FLOAT_TENSOR 1
#define KSORT_LOGGING_WARNING 2
#define KSORT_ARENA_ALLOCATOR 1
#define KSORT_MEM_TYPE_DEFAULT 0

typedef void OrtStatus;
typedef void OrtEnv;
typedef void OrtSession;
typedef void OrtSessionOptions;
typedef void OrtRunOptions;
typedef void OrtMemoryInfo;
typedef void OrtValue;
typedef void OrtTensorTypeAndShapeInfo;
typedef void OrtAllocator;

typedef const struct OrtApiBase {
    const void *(*GetApi)(uint32_t version);
    const char *(*GetVersionString)(void);
} OrtApiBase;

typedef const OrtApiBase *(*OrtGetApiBaseFunction)(void);

typedef const char *(*KSORTGetErrorMessageFunction)(const OrtStatus *status);
typedef OrtStatus *(*KSORTCreateEnvFunction)(int log_severity_level, const char *logid, OrtEnv **out);
typedef OrtStatus *(*KSORTCreateSessionFunction)(const OrtEnv *env, const char *model_path, const OrtSessionOptions *options, OrtSession **out);
typedef OrtStatus *(*KSORTRunFunction)(
    OrtSession *session,
    const OrtRunOptions *run_options,
    const char *const *input_names,
    const OrtValue *const *inputs,
    size_t input_len,
    const char *const *output_names,
    size_t output_names_len,
    OrtValue **outputs
);
typedef OrtStatus *(*KSORTCreateSessionOptionsFunction)(OrtSessionOptions **options);
typedef OrtStatus *(*KSORTSetSessionGraphOptimizationLevelFunction)(OrtSessionOptions *options, int graph_optimization_level);
typedef OrtStatus *(*KSORTSetThreadsFunction)(OrtSessionOptions *options, int thread_count);
typedef OrtStatus *(*KSORTSessionGetCountFunction)(const OrtSession *session, size_t *out);
typedef OrtStatus *(*KSORTSessionGetNameFunction)(const OrtSession *session, size_t index, OrtAllocator *allocator, char **value);
typedef OrtStatus *(*KSORTCreateTensorWithDataAsOrtValueFunction)(
    const OrtMemoryInfo *info,
    void *data,
    size_t data_length,
    const int64_t *shape,
    size_t shape_length,
    int type,
    OrtValue **out
);
typedef OrtStatus *(*KSORTIsTensorFunction)(const OrtValue *value, int *out);
typedef OrtStatus *(*KSORTGetTensorMutableDataFunction)(OrtValue *value, void **out);
typedef OrtStatus *(*KSORTGetTensorElementTypeFunction)(const OrtTensorTypeAndShapeInfo *info, int *out);
typedef OrtStatus *(*KSORTGetDimensionsCountFunction)(const OrtTensorTypeAndShapeInfo *info, size_t *out);
typedef OrtStatus *(*KSORTGetDimensionsFunction)(const OrtTensorTypeAndShapeInfo *info, int64_t *dimensions, size_t dimensions_length);
typedef OrtStatus *(*KSORTGetTensorShapeElementCountFunction)(const OrtTensorTypeAndShapeInfo *info, size_t *out);
typedef OrtStatus *(*KSORTGetTensorTypeAndShapeFunction)(const OrtValue *value, OrtTensorTypeAndShapeInfo **out);
typedef OrtStatus *(*KSORTCreateCpuMemoryInfoFunction)(int type, int memory_type, OrtMemoryInfo **out);
typedef OrtStatus *(*KSORTAllocatorFreeFunction)(OrtAllocator *allocator, void *pointer);
typedef OrtStatus *(*KSORTGetAllocatorWithDefaultOptionsFunction)(OrtAllocator **out);
typedef void (*KSORTReleaseFunction)(void *input);

enum {
    KSORT_API_GET_ERROR_MESSAGE = 2,
    KSORT_API_CREATE_ENV = 3,
    KSORT_API_CREATE_SESSION = 7,
    KSORT_API_RUN = 9,
    KSORT_API_CREATE_SESSION_OPTIONS = 10,
    KSORT_API_SET_GRAPH_OPTIMIZATION = 23,
    KSORT_API_SET_INTRA_OP_THREADS = 24,
    KSORT_API_SET_INTER_OP_THREADS = 25,
    KSORT_API_SESSION_GET_INPUT_COUNT = 30,
    KSORT_API_SESSION_GET_OUTPUT_COUNT = 31,
    KSORT_API_SESSION_GET_INPUT_NAME = 36,
    KSORT_API_SESSION_GET_OUTPUT_NAME = 37,
    KSORT_API_CREATE_TENSOR_WITH_DATA = 49,
    KSORT_API_IS_TENSOR = 50,
    KSORT_API_GET_TENSOR_MUTABLE_DATA = 51,
    KSORT_API_GET_TENSOR_ELEMENT_TYPE = 60,
    KSORT_API_GET_DIMENSIONS_COUNT = 61,
    KSORT_API_GET_DIMENSIONS = 62,
    KSORT_API_GET_TENSOR_SHAPE_ELEMENT_COUNT = 64,
    KSORT_API_GET_TENSOR_TYPE_AND_SHAPE = 65,
    KSORT_API_CREATE_CPU_MEMORY_INFO = 69,
    KSORT_API_ALLOCATOR_FREE = 76,
    KSORT_API_GET_ALLOCATOR_WITH_DEFAULT_OPTIONS = 78,
    KSORT_API_RELEASE_ENV = 92,
    KSORT_API_RELEASE_STATUS = 93,
    KSORT_API_RELEASE_MEMORY_INFO = 94,
    KSORT_API_RELEASE_SESSION = 95,
    KSORT_API_RELEASE_VALUE = 96,
    KSORT_API_RELEASE_TYPE_INFO = 98,
    KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO = 99,
    KSORT_API_RELEASE_SESSION_OPTIONS = 100
};

struct KSORTSession {
    void *library_handle;
    int owns_library_handle;
    const void *api;
    char *runtime_version;
    OrtEnv *env;
    OrtSession *session;
    OrtMemoryInfo *memory_info;
    char *input_name;
    char *output_name;
};

static void ksort_set_error(char *error, size_t error_length, const char *message) {
    if (error == NULL || error_length == 0) {
        return;
    }
    if (message == NULL) {
        message = "Unknown ONNX Runtime error.";
    }
    size_t length = strlen(message);
    if (length >= error_length) {
        length = error_length - 1;
    }
    memcpy(error, message, length);
    error[length] = '\0';
}

static void *ksort_api_function(const void *api, int index) {
    return (void *)((const void *const *)api)[index];
}

static int ksort_check_status(const void *api, OrtStatus *status, char *error, size_t error_length) {
    if (status == NULL) {
        return 1;
    }
    KSORTGetErrorMessageFunction get_error_message = (KSORTGetErrorMessageFunction)ksort_api_function(api, KSORT_API_GET_ERROR_MESSAGE);
    const char *message = get_error_message != NULL ? get_error_message(status) : "ONNX Runtime call failed.";
    ksort_set_error(error, error_length, message);
    KSORTReleaseFunction release_status = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_STATUS);
    if (release_status != NULL) {
        release_status(status);
    }
    return 0;
}

static char *ksort_copy_string(const char *string) {
    if (string == NULL) {
        return NULL;
    }
    size_t length = strlen(string);
    char *copy = (char *)malloc(length + 1);
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, string, length + 1);
    return copy;
}

static OrtGetApiBaseFunction ksort_load_api_base(const char *library_path, void **library_handle, int *owns_library_handle, char *error, size_t error_length) {
    *library_handle = NULL;
    *owns_library_handle = 0;

    OrtGetApiBaseFunction get_api_base = (OrtGetApiBaseFunction)dlsym(RTLD_DEFAULT, "OrtGetApiBase");
    if (get_api_base != NULL) {
        return get_api_base;
    }

    const char *paths[4];
    size_t path_count = 0;
    if (library_path != NULL && library_path[0] != '\0') {
        paths[path_count++] = library_path;
    } else {
        paths[path_count++] = "@rpath/onnxruntime.framework/onnxruntime";
        paths[path_count++] = "onnxruntime.framework/onnxruntime";
        paths[path_count++] = "libonnxruntime.dylib";
    }

    for (size_t index = 0; index < path_count; index++) {
        void *handle = dlopen(paths[index], RTLD_NOW | RTLD_LOCAL);
        if (handle == NULL) {
            continue;
        }
        get_api_base = (OrtGetApiBaseFunction)dlsym(handle, "OrtGetApiBase");
        if (get_api_base != NULL) {
            *library_handle = handle;
            *owns_library_handle = 1;
            return get_api_base;
        }
        dlclose(handle);
    }

    const char *loader_error = dlerror();
    ksort_set_error(error, error_length, loader_error != NULL ? loader_error : "ONNX Runtime C API was not found.");
    return NULL;
}

int KSORTIsRuntimeAvailable(const char *runtime_library_path, char *error, size_t error_length) {
    void *handle = NULL;
    int owns_handle = 0;
    OrtGetApiBaseFunction get_api_base = ksort_load_api_base(runtime_library_path, &handle, &owns_handle, error, error_length);
    if (get_api_base == NULL) {
        return 0;
    }
    const OrtApiBase *base = get_api_base();
    if (base == NULL || base->GetApi == NULL || base->GetApi(KSORT_API_VERSION) == NULL) {
        ksort_set_error(error, error_length, "ONNX Runtime does not provide the required C API version.");
        if (owns_handle) {
            dlclose(handle);
        }
        return 0;
    }
    if (owns_handle) {
        dlclose(handle);
    }
    return 1;
}

static char *ksort_session_name(const void *api, OrtSession *session, int name_index, const char *preferred_name, char *error, size_t error_length) {
    if (preferred_name != NULL && preferred_name[0] != '\0') {
        return ksort_copy_string(preferred_name);
    }

    OrtAllocator *allocator = NULL;
    KSORTGetAllocatorWithDefaultOptionsFunction get_allocator = (KSORTGetAllocatorWithDefaultOptionsFunction)ksort_api_function(api, KSORT_API_GET_ALLOCATOR_WITH_DEFAULT_OPTIONS);
    if (!ksort_check_status(api, get_allocator(&allocator), error, error_length)) {
        return NULL;
    }

    char *allocated_name = NULL;
    KSORTSessionGetNameFunction get_name = (KSORTSessionGetNameFunction)ksort_api_function(api, name_index);
    if (!ksort_check_status(api, get_name(session, 0, allocator, &allocated_name), error, error_length)) {
        return NULL;
    }
    char *name = ksort_copy_string(allocated_name);
    KSORTAllocatorFreeFunction allocator_free = (KSORTAllocatorFreeFunction)ksort_api_function(api, KSORT_API_ALLOCATOR_FREE);
    if (allocator_free != NULL) {
        allocator_free(allocator, allocated_name);
    }
    return name;
}

KSORTSession *KSORTCreateSession(const char *model_path, KSORTSessionConfiguration configuration, char *error, size_t error_length) {
    if (model_path == NULL || model_path[0] == '\0') {
        ksort_set_error(error, error_length, "An ONNX model path is required.");
        return NULL;
    }

    void *handle = NULL;
    int owns_handle = 0;
    OrtGetApiBaseFunction get_api_base = ksort_load_api_base(configuration.runtime_library_path, &handle, &owns_handle, error, error_length);
    if (get_api_base == NULL) {
        return NULL;
    }

    const OrtApiBase *base = get_api_base();
    const void *api = base != NULL && base->GetApi != NULL ? base->GetApi(KSORT_API_VERSION) : NULL;
    if (api == NULL) {
        ksort_set_error(error, error_length, "ONNX Runtime does not provide the required C API version.");
        if (owns_handle) {
            dlclose(handle);
        }
        return NULL;
    }

    KSORTSession *session = (KSORTSession *)calloc(1, sizeof(KSORTSession));
    if (session == NULL) {
        ksort_set_error(error, error_length, "Unable to allocate ONNX Runtime session wrapper.");
        if (owns_handle) {
            dlclose(handle);
        }
        return NULL;
    }
    session->library_handle = handle;
    session->owns_library_handle = owns_handle;
    session->api = api;
    if (base->GetVersionString != NULL) {
        session->runtime_version = ksort_copy_string(base->GetVersionString());
    }

    KSORTCreateEnvFunction create_env = (KSORTCreateEnvFunction)ksort_api_function(api, KSORT_API_CREATE_ENV);
    if (!ksort_check_status(api, create_env(KSORT_LOGGING_WARNING, "KSPlayerDepthAnything", &session->env), error, error_length)) {
        KSORTReleaseSession(session);
        return NULL;
    }

    KSORTCreateSessionOptionsFunction create_options = (KSORTCreateSessionOptionsFunction)ksort_api_function(api, KSORT_API_CREATE_SESSION_OPTIONS);
    OrtSessionOptions *options = NULL;
    if (!ksort_check_status(api, create_options(&options), error, error_length)) {
        KSORTReleaseSession(session);
        return NULL;
    }

    KSORTSetSessionGraphOptimizationLevelFunction set_graph_optimization = (KSORTSetSessionGraphOptimizationLevelFunction)ksort_api_function(api, KSORT_API_SET_GRAPH_OPTIMIZATION);
    ksort_check_status(api, set_graph_optimization(options, configuration.graph_optimization_level), error, error_length);
    if (configuration.intra_op_num_threads > 0) {
        KSORTSetThreadsFunction set_threads = (KSORTSetThreadsFunction)ksort_api_function(api, KSORT_API_SET_INTRA_OP_THREADS);
        ksort_check_status(api, set_threads(options, configuration.intra_op_num_threads), error, error_length);
    }
    if (configuration.inter_op_num_threads > 0) {
        KSORTSetThreadsFunction set_threads = (KSORTSetThreadsFunction)ksort_api_function(api, KSORT_API_SET_INTER_OP_THREADS);
        ksort_check_status(api, set_threads(options, configuration.inter_op_num_threads), error, error_length);
    }

    KSORTCreateSessionFunction create_session = (KSORTCreateSessionFunction)ksort_api_function(api, KSORT_API_CREATE_SESSION);
    if (!ksort_check_status(api, create_session(session->env, model_path, options, &session->session), error, error_length)) {
        KSORTReleaseFunction release_options = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_SESSION_OPTIONS);
        release_options(options);
        KSORTReleaseSession(session);
        return NULL;
    }
    KSORTReleaseFunction release_options = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_SESSION_OPTIONS);
    release_options(options);

    KSORTSessionGetCountFunction input_count_function = (KSORTSessionGetCountFunction)ksort_api_function(api, KSORT_API_SESSION_GET_INPUT_COUNT);
    KSORTSessionGetCountFunction output_count_function = (KSORTSessionGetCountFunction)ksort_api_function(api, KSORT_API_SESSION_GET_OUTPUT_COUNT);
    size_t input_count = 0;
    size_t output_count = 0;
    if (!ksort_check_status(api, input_count_function(session->session, &input_count), error, error_length) || input_count == 0) {
        ksort_set_error(error, error_length, "ONNX model has no inputs.");
        KSORTReleaseSession(session);
        return NULL;
    }
    if (!ksort_check_status(api, output_count_function(session->session, &output_count), error, error_length) || output_count == 0) {
        ksort_set_error(error, error_length, "ONNX model has no outputs.");
        KSORTReleaseSession(session);
        return NULL;
    }

    session->input_name = ksort_session_name(api, session->session, KSORT_API_SESSION_GET_INPUT_NAME, configuration.preferred_input_name, error, error_length);
    session->output_name = ksort_session_name(api, session->session, KSORT_API_SESSION_GET_OUTPUT_NAME, configuration.preferred_output_name, error, error_length);
    if (session->input_name == NULL || session->output_name == NULL) {
        KSORTReleaseSession(session);
        return NULL;
    }

    KSORTCreateCpuMemoryInfoFunction create_memory_info = (KSORTCreateCpuMemoryInfoFunction)ksort_api_function(api, KSORT_API_CREATE_CPU_MEMORY_INFO);
    if (!ksort_check_status(api, create_memory_info(KSORT_ARENA_ALLOCATOR, KSORT_MEM_TYPE_DEFAULT, &session->memory_info), error, error_length)) {
        KSORTReleaseSession(session);
        return NULL;
    }

    return session;
}

void KSORTReleaseSession(KSORTSession *session) {
    if (session == NULL) {
        return;
    }
    const void *api = session->api;
    if (api != NULL) {
        KSORTReleaseFunction release_memory_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_MEMORY_INFO);
        KSORTReleaseFunction release_session = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_SESSION);
        KSORTReleaseFunction release_env = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_ENV);
        if (release_memory_info != NULL && session->memory_info != NULL) {
            release_memory_info(session->memory_info);
        }
        if (release_session != NULL && session->session != NULL) {
            release_session(session->session);
        }
        if (release_env != NULL && session->env != NULL) {
            release_env(session->env);
        }
    }
    free(session->runtime_version);
    free(session->input_name);
    free(session->output_name);
    if (session->owns_library_handle && session->library_handle != NULL) {
        dlclose(session->library_handle);
    }
    free(session);
}

const char *KSORTGetRuntimeVersion(KSORTSession *session) {
    return session != NULL ? session->runtime_version : NULL;
}

const char *KSORTGetInputName(KSORTSession *session) {
    return session != NULL ? session->input_name : NULL;
}

const char *KSORTGetOutputName(KSORTSession *session) {
    return session != NULL ? session->output_name : NULL;
}

int KSORTRunFloat32(
    KSORTSession *session,
    const float *input_data,
    size_t input_count,
    const int64_t *input_shape,
    size_t input_shape_count,
    float **output_data,
    size_t *output_count,
    int64_t **output_shape,
    size_t *output_shape_count,
    char *error,
    size_t error_length
) {
    if (session == NULL || input_data == NULL || input_shape == NULL || output_data == NULL || output_count == NULL || output_shape == NULL || output_shape_count == NULL) {
        ksort_set_error(error, error_length, "Invalid ONNX Runtime inference arguments.");
        return 0;
    }
    *output_data = NULL;
    *output_count = 0;
    *output_shape = NULL;
    *output_shape_count = 0;

    const void *api = session->api;
    OrtValue *input_value = NULL;
    KSORTCreateTensorWithDataAsOrtValueFunction create_tensor = (KSORTCreateTensorWithDataAsOrtValueFunction)ksort_api_function(api, KSORT_API_CREATE_TENSOR_WITH_DATA);
    if (!ksort_check_status(api, create_tensor(session->memory_info, (void *)input_data, input_count * sizeof(float), input_shape, input_shape_count, KSORT_FLOAT_TENSOR, &input_value), error, error_length)) {
        return 0;
    }

    const char *input_names[1] = { session->input_name };
    const char *output_names[1] = { session->output_name };
    const OrtValue *inputs[1] = { input_value };
    OrtValue *output_value = NULL;
    KSORTRunFunction run = (KSORTRunFunction)ksort_api_function(api, KSORT_API_RUN);
    int success = ksort_check_status(api, run(session->session, NULL, input_names, inputs, 1, output_names, 1, &output_value), error, error_length);
    KSORTReleaseFunction release_value = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_VALUE);
    release_value(input_value);
    if (!success) {
        return 0;
    }

    int is_tensor = 0;
    KSORTIsTensorFunction is_tensor_function = (KSORTIsTensorFunction)ksort_api_function(api, KSORT_API_IS_TENSOR);
    if (!ksort_check_status(api, is_tensor_function(output_value, &is_tensor), error, error_length) || !is_tensor) {
        ksort_set_error(error, error_length, "ONNX Runtime output is not a tensor.");
        release_value(output_value);
        return 0;
    }

    OrtTensorTypeAndShapeInfo *shape_info = NULL;
    KSORTGetTensorTypeAndShapeFunction get_type_and_shape = (KSORTGetTensorTypeAndShapeFunction)ksort_api_function(api, KSORT_API_GET_TENSOR_TYPE_AND_SHAPE);
    if (!ksort_check_status(api, get_type_and_shape(output_value, &shape_info), error, error_length)) {
        release_value(output_value);
        return 0;
    }

    int element_type = 0;
    KSORTGetTensorElementTypeFunction get_element_type = (KSORTGetTensorElementTypeFunction)ksort_api_function(api, KSORT_API_GET_TENSOR_ELEMENT_TYPE);
    if (!ksort_check_status(api, get_element_type(shape_info, &element_type), error, error_length) || element_type != KSORT_FLOAT_TENSOR) {
        ksort_set_error(error, error_length, "ONNX Runtime output tensor must be Float32.");
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }

    KSORTGetDimensionsCountFunction get_dimensions_count = (KSORTGetDimensionsCountFunction)ksort_api_function(api, KSORT_API_GET_DIMENSIONS_COUNT);
    size_t dimension_count = 0;
    if (!ksort_check_status(api, get_dimensions_count(shape_info, &dimension_count), error, error_length)) {
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }

    int64_t *dimensions = (int64_t *)calloc(dimension_count > 0 ? dimension_count : 1, sizeof(int64_t));
    if (dimensions == NULL) {
        ksort_set_error(error, error_length, "Unable to allocate ONNX output shape.");
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }
    KSORTGetDimensionsFunction get_dimensions = (KSORTGetDimensionsFunction)ksort_api_function(api, KSORT_API_GET_DIMENSIONS);
    if (!ksort_check_status(api, get_dimensions(shape_info, dimensions, dimension_count), error, error_length)) {
        free(dimensions);
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }

    size_t element_count = 0;
    KSORTGetTensorShapeElementCountFunction get_element_count = (KSORTGetTensorShapeElementCountFunction)ksort_api_function(api, KSORT_API_GET_TENSOR_SHAPE_ELEMENT_COUNT);
    if (!ksort_check_status(api, get_element_count(shape_info, &element_count), error, error_length)) {
        free(dimensions);
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }

    void *tensor_data = NULL;
    KSORTGetTensorMutableDataFunction get_tensor_data = (KSORTGetTensorMutableDataFunction)ksort_api_function(api, KSORT_API_GET_TENSOR_MUTABLE_DATA);
    if (!ksort_check_status(api, get_tensor_data(output_value, &tensor_data), error, error_length)) {
        free(dimensions);
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }

    float *copied_output = (float *)malloc(element_count * sizeof(float));
    if (copied_output == NULL) {
        ksort_set_error(error, error_length, "Unable to allocate ONNX output buffer.");
        free(dimensions);
        KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
        release_shape_info(shape_info);
        release_value(output_value);
        return 0;
    }
    memcpy(copied_output, tensor_data, element_count * sizeof(float));

    *output_data = copied_output;
    *output_count = element_count;
    *output_shape = dimensions;
    *output_shape_count = dimension_count;

    KSORTReleaseFunction release_shape_info = (KSORTReleaseFunction)ksort_api_function(api, KSORT_API_RELEASE_TENSOR_TYPE_AND_SHAPE_INFO);
    release_shape_info(shape_info);
    release_value(output_value);
    return 1;
}

void KSORTReleaseBuffer(void *buffer) {
    free(buffer);
}
