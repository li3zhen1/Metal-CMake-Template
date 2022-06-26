
#include "metal.h"
#include "renderer/Compute.h"

int main(int argc, char *argv[]) {
    NS::AutoreleasePool *pAutoreleasePool = NS::AutoreleasePool::alloc()->init();

    auto _pDevice = MTL::CreateSystemDefaultDevice();

    auto compute = new Compute(_pDevice);
    compute->buildShaders();

    const uint arrayLength = INT_MAX;

    compute->calculate(arrayLength);
    compute->verifyResult(arrayLength);

    compute->calculate(arrayLength);
    compute->verifyResult(arrayLength);

    compute->calculate(arrayLength);
    compute->verifyResult(arrayLength);

    compute->calculate(arrayLength);
    compute->verifyResult(arrayLength);

    pAutoreleasePool->release();

    return 0;
}
