#include <gst/gst.h>
#include <glib.h>

#include "gstnvdsmeta.h"
#include "nvdsmeta_schema.h"
#include "nvbufsurface.h"

#ifndef PLATFORM_TEGRA
  #include "gst-nvmessage.h"
#endif

#include <cuda.h>
 
#include <string>
#include <fstream>
#include <cmath>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace std::chrono;

#define SOURCE_PATH "inputsources.txt"

#define PERF_INTERVAL 2

#define MAX_DISPLAY_LEN 64

// Network Compute Mode
#define COMPUTE_MODE "fp16"

#define MAX_TRACKING_ID_LEN 16

// Muxer Resolution
#define MUXER_OUTPUT_WIDTH 1920
#define MUXER_OUTPUT_HEIGHT 1080

/* Muxer batch formation timeout, for e.g. 40 millisec. Should ideally be set
 * based on the fastest source's framerate. */
#define MUXER_BATCH_TIMEOUT_USEC 4000000

// Tiles Resolution
#define TILED_OUTPUT_WIDTH 1920
#define TILED_OUTPUT_HEIGHT 1080

/* NVIDIA Decoder source pad memory feature. This feature signifies that source
 * pads having this capability will push GstBuffers containing cuda buffers. */
#define GST_CAPS_FEATURES_NVMM "memory:NVMM"

#define CHECK_ERROR(error) \
    if (error) { \
        g_printerr ("Error while parsing config file: %s\n", error->message); \
        goto done; \
    }

#define CONFIG_GROUP_TRACKER "tracker"
#define CONFIG_GROUP_TRACKER_WIDTH "tracker-width"
#define CONFIG_GROUP_TRACKER_HEIGHT "tracker-height"
#define CONFIG_GROUP_TRACKER_LL_CONFIG_FILE "ll-config-file"
#define CONFIG_GROUP_TRACKER_LL_LIB_FILE "ll-lib-file"
#define CONFIG_GROUP_TRACKER_ENABLE_BATCH_PROCESS "enable-batch-process"
#define CONFIG_GPU_ID "gpu-id"

int num_sources = 0;