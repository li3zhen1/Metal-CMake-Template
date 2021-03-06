#include "Compute.h"

Compute::Compute(MTL::Device *pDevice)
    : _pDevice(pDevice->retain()) {

    std::cout << "Computing with device " << pDevice->name()->cString(NS::UTF8StringEncoding) << std::endl;
}

Compute::~Compute() {
    _pBufferA->release();
    _pBufferB->release();
    _pBufferResult->release();

    _pComputeCommandEncoder->release();
    _pCommandBuffer->release();
    _pCommandQueue->release();

    _pFunc->release();
    _pPSO->release();
    _pLibrary->release();

    _pDevice->release();
}

void Compute::buildShaders() {
    using NS::UTF8StringEncoding;

    NS::Error *pError = nullptr;
//    auto *pPath = NS::String::string("./shader/MetalAdd.metal", UTF8StringEncoding);
//    auto *pUrl = NS::URL::alloc()->initFileURLWithPath(pPath);

    auto src = R"(
kernel void add_arrays(device const float* inA,
                       device const float* inB,
                       device float* result,
                       uint index [[thread_position_in_grid]])
{
    // the for-loop is replaced with a collection of threads, each of which
    // calls this function.
    result[index] = inA[index] + inB[index];
}
)";

    _pLibrary = _pDevice->newLibrary(
        NS::String::string(src, UTF8StringEncoding),
        nullptr,
        &pError);

    if (pError) {
        std::cout << pError->description()->cString(UTF8StringEncoding) << std::endl;
    }

    _pFunc = _pLibrary->newFunction(NS::String::string("add_arrays", UTF8StringEncoding));

    _pPSO = _pDevice->newComputePipelineState(_pFunc, &pError);

    if (pError) {
        std::cout << pError->description()->cString(UTF8StringEncoding) << std::endl;
    }

    _pCommandQueue = _pDevice->newCommandQueue();

    pError->release();
}

void Compute::generateRandomData(MTL::Buffer *buffer, unsigned long size) {
    auto *dataPtr = static_cast<float *>(buffer->contents());

    std::default_random_engine randomEngine;

    for (unsigned long index = 0; index < size; index++) {
        dataPtr[index] = (float) (randomEngine()) / (float) (RAND_MAX);
    }
}

void Compute::calculate(unsigned long arrayLength) {

    unsigned long bufferSize = arrayLength * sizeof(float);

    _pBufferA = _pDevice->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    _pBufferB = _pDevice->newBuffer(bufferSize, MTL::ResourceStorageModeShared);
    _pBufferResult = _pDevice->newBuffer(bufferSize, MTL::ResourceStorageModeShared);


    generateRandomData(_pBufferA, arrayLength);
    generateRandomData(_pBufferB, arrayLength);


//    for (u_long index = 0; index < arrayLength; index++) {
//        std::cout << (dataPtrA[index]) << "\t\t" << (dataPtrB[index]) << std::endl;
//    }

    _pCommandBuffer = _pCommandQueue->commandBuffer();
    _pComputeCommandEncoder = _pCommandBuffer->computeCommandEncoder();
    _pComputeCommandEncoder->setComputePipelineState(_pPSO);
    _pComputeCommandEncoder->setBuffer(_pBufferA, 0, 0);
    _pComputeCommandEncoder->setBuffer(_pBufferB, 0, 1);
    _pComputeCommandEncoder->setBuffer(_pBufferResult, 0, 2);


    auto gridSize = MTL::Size(arrayLength, 1, 1);

    unsigned long maxThreadGroupSize = _pPSO->maxTotalThreadsPerThreadgroup();
    if (maxThreadGroupSize > arrayLength) {
        maxThreadGroupSize = arrayLength;
    }
    auto threadGroupSize = MTL::Size(maxThreadGroupSize, 1, 1);

    _pComputeCommandEncoder->dispatchThreadgroups(gridSize, threadGroupSize);

    _pComputeCommandEncoder->endEncoding();

    auto start = std::chrono::system_clock::now();
    _pCommandBuffer->commit();

    _pCommandBuffer->waitUntilCompleted();
    auto end = std::chrono::system_clock::now();
    auto dur = (end - start).count();
    std::cout << dur << std::endl;


    auto *dataPtrA = static_cast<float *>(_pBufferA->contents());
    auto *dataPtrB = static_cast<float *>(_pBufferB->contents());
    auto *dataPtrResult = static_cast<float *>(_pBufferResult->contents());
    auto _start = std::chrono::system_clock::now();
    for (u_long index = 0; index < arrayLength; index++) {
        (dataPtrResult[index]) = (dataPtrA[index]) + (dataPtrB[index]);
    }
    auto _end = std::chrono::system_clock::now();
    auto _dur = (_end - _start).count();
    std::cout << _dur << std::endl;
}

void Compute::verifyResult(unsigned long arrayLength) {
    auto *a = static_cast<float *>(_pBufferA->contents());
    auto *b = static_cast<float *>(_pBufferB->contents());
    auto *result = static_cast<float *>(_pBufferResult->contents());

    for (unsigned long index = 0; index < arrayLength; index++) {
        if (result[index] != (a[index] + b[index])) {
            printf("Compute ERROR: index=%lu result=%g vs %g=a+b\n",
                   index, result[index], a[index] + b[index]);
        }
    }
    printf("Compute results as expected\n");
}