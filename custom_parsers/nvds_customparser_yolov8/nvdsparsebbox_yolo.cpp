#include <algorithm>
#include <cmath>
#include <sstream>
#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cuda_runtime_api.h>

#include "nvdsinfer_custom_impl.h"

uint kNUM_CLASSES;
float clamp(const float val, const float minVal, const float maxVal) {
    assert(minVal <= maxVal);
    return std::min(maxVal, std::max(minVal, val));
}
extern "C" bool NvDsInferParseYolo(
    std::vector<NvDsInferLayerInfo> const &outputLayersInfo, NvDsInferNetworkInfo const &networkInfo,
    NvDsInferParseDetectionParams const &detectionParams, std::vector<NvDsInferParseObjectInfo> &objectList);

static NvDsInferParseObjectInfo convertBBox(
    const float &bx1, const float &by1, const float &bx2, const float &by2, const uint &netW, const uint &netH) {
    NvDsInferParseObjectInfo b;

    float x1 = bx1;
    float y1 = by1;
    float x2 = bx2;
    float y2 = by2;

    x1 = clamp(x1, 0, netW);
    y1 = clamp(y1, 0, netH);
    x2 = clamp(x2, 0, netW);
    y2 = clamp(y2, 0, netH);

    b.left = x1;
    b.width = clamp(x2 - x1, 0, netW);
    b.top = y1;
    b.height = clamp(y2 - y1, 0, netH);

    return b;
}

static void addBBoxProposal(
    const float bx1, const float by1, const float bx2, const float by2, const uint &netW, const uint &netH,
    const int maxIndex, const float maxProb, std::vector<NvDsInferParseObjectInfo> &binfo)
{
    NvDsInferParseObjectInfo bbi = convertBBox(bx1, by1, bx2, by2, netW, netH);
    if (bbi.width < 1 || bbi.height < 1)
        return;

    bbi.detectionConfidence = maxProb;
    bbi.classId = maxIndex;
    binfo.push_back(bbi);
}

static std::vector<NvDsInferParseObjectInfo> decodeYoloTensor(
    const int *counts, const float *boxes, const float *scores, const int *classes, const uint &netW, const uint &netH)
{
    std::vector<NvDsInferParseObjectInfo> binfo;

    uint numBoxes = counts[0];
    for (uint b = 0; b < numBoxes; ++b)
    {
        float bx1 = boxes[b * 4 + 0];
        float by1 = boxes[b * 4 + 1];
        float bx2 = boxes[b * 4 + 2];
        float by2 = boxes[b * 4 + 3];

        float maxProb = scores[b];
        int maxIndex = classes[b];

        addBBoxProposal(bx1, by1, bx2, by2, netW, netH, maxIndex, maxProb, binfo);
    }
    return binfo;
}

static bool NvDsInferParseCustomYolo(
    std::vector<NvDsInferLayerInfo> const &outputLayersInfo, NvDsInferNetworkInfo const &networkInfo,
    NvDsInferParseDetectionParams const &detectionParams, std::vector<NvDsInferParseObjectInfo> &objectList,
    const uint &numClasses)
{
    if (outputLayersInfo.empty())
    {
        std::cerr << "ERROR: Could not find output layer in bbox parsing" << std::endl;
        return false;
    }

    std::vector<NvDsInferParseObjectInfo> objects;

    const NvDsInferLayerInfo &counts = outputLayersInfo[0];
    const NvDsInferLayerInfo &boxes = outputLayersInfo[1];
    const NvDsInferLayerInfo &scores = outputLayersInfo[2];
    const NvDsInferLayerInfo &classes = outputLayersInfo[3];

    std::vector<NvDsInferParseObjectInfo> outObjs =
        decodeYoloTensor(
            (const int *)(counts.buffer), (const float *)(boxes.buffer), (const float *)(scores.buffer),
            (const int *)(classes.buffer), networkInfo.width, networkInfo.height);

    objects.insert(objects.end(), outObjs.begin(), outObjs.end());

    objectList = objects;

    return true;
}

extern "C" bool NvDsInferParseYolo(
    std::vector<NvDsInferLayerInfo> const &outputLayersInfo, NvDsInferNetworkInfo const &networkInfo,
    NvDsInferParseDetectionParams const &detectionParams, std::vector<NvDsInferParseObjectInfo> &objectList)
{
    int num_classes = kNUM_CLASSES;

    return NvDsInferParseCustomYolo(
        outputLayersInfo, networkInfo, detectionParams, objectList, num_classes);
}

CHECK_CUSTOM_PARSE_FUNC_PROTOTYPE(NvDsInferParseYolo);
