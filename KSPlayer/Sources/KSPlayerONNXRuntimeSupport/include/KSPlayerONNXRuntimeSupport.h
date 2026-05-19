#ifndef KSPLAYER_ONNX_RUNTIME_SUPPORT_H
#define KSPLAYER_ONNX_RUNTIME_SUPPORT_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct KSORTSession KSORTSession;

typedef struct KSORTSessionConfiguration {
    const char *runtime_library_path;
    const char *preferred_input_name;
    const char *preferred_output_name;
    int graph_optimization_level;
    int intra_op_num_threads;
    int inter_op_num_threads;
} KSORTSessionConfiguration;

int KSORTIsRuntimeAvailable(const char *runtime_library_path, char *error, size_t error_length);

KSORTSession *KSORTCreateSession(
    const char *model_path,
    KSORTSessionConfiguration configuration,
    char *error,
    size_t error_length
);

void KSORTReleaseSession(KSORTSession *session);

const char *KSORTGetRuntimeVersion(KSORTSession *session);
const char *KSORTGetInputName(KSORTSession *session);
const char *KSORTGetOutputName(KSORTSession *session);

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
);

void KSORTReleaseBuffer(void *buffer);

#ifdef __cplusplus
}
#endif

#endif
