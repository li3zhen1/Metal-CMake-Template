//
// Created by 李臻 on 2022/6/24.
//

#pragma once
#include <Metal/Metal.hpp>
#include <iostream>
#include <random>

class Compute {
private:
    MTL::Device *_pDevice;

    MTL::CommandQueue *_pCommandQueue;
    MTL::CommandBuffer *_pCommandBuffer;
    MTL::ComputeCommandEncoder *_pComputeCommandEncoder;

    MTL::ComputePipelineState *_pPSO;
    MTL::Function * _pFunc;
    MTL::Library * _pLibrary;

    MTL::Buffer *_pBufferA;
    MTL::Buffer *_pBufferB;
    MTL::Buffer *_pBufferResult;

public:
    explicit Compute(MTL::Device *pDevice);
    ~Compute();

    void buildShaders();

    static void generateRandomData(MTL::Buffer *buffer, unsigned long bufferSize);

    void calculate(unsigned long arrayLength);

    void verifyResult(unsigned long arrayLength);
};

