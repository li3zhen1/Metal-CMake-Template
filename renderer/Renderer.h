//
// Created by 李臻 on 2022/6/24.
//

#pragma once
#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include <simd/simd.h>

class Renderer {
public:
    Renderer(MTL::Device *pDevice);

    ~Renderer();

    void buildShaders();

    void buildBuffers();

    void buildFrameData();

    void draw(MTK::View *pView);

private:
    MTL::Device *_pDevice;
    MTL::CommandQueue *_pCommandQueue;
    MTL::Library *_pShaderLibrary;
    MTL::RenderPipelineState *_pPSO;
    MTL::Buffer *_pArgBuffer;
    MTL::Buffer *_pVertexPositionsBuffer;
    MTL::Buffer *_pVertexColorsBuffer;
    MTL::Buffer *_pFrameData[3];
    float _angle;
    int _frame;
    dispatch_semaphore_t _semaphore;
    static const int kMaxFramesInFlight;
};

class MyMTKViewDelegate : public MTK::ViewDelegate {
public:
    MyMTKViewDelegate(MTL::Device *pDevice);

    virtual ~MyMTKViewDelegate() override;

    virtual void drawInMTKView(MTK::View *pView) override;

private:
    Renderer *_pRenderer;
};

class MyAppDelegate : public NS::ApplicationDelegate {
public:
    ~MyAppDelegate();

    NS::Menu *createMenuBar();

    virtual void applicationWillFinishLaunching(NS::Notification *pNotification) override;

    virtual void applicationDidFinishLaunching(NS::Notification *pNotification) override;

    virtual bool applicationShouldTerminateAfterLastWindowClosed(NS::Application *pSender) override;

private:
    NS::Window *_pWindow;
    MTK::View *_pMtkView;
    MTL::Device *_pDevice;
    MyMTKViewDelegate *_pViewDelegate = nullptr;
};
