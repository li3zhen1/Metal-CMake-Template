

#include "metal.h"
#include "renderer/Renderer.h"
#include "renderer/Compute.h"

int main(int argc, char *argv[]) {
    NS::AutoreleasePool *pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

//    MyAppDelegate del;
//
//    NS::Application *pSharedApplication = NS::Application::sharedApplication();
//    pSharedApplication->setDelegate(&del);
//    pSharedApplication->run();
    auto _pDevice = MTL::CreateSystemDefaultDevice();

    auto compute = new Compute(_pDevice);
    compute->buildShaders();
    compute->calculate(32);
    compute->verifyResult(32);

    pAutoreleasePool->release();

    return 0;
}
