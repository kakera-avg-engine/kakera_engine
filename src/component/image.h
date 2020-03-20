#ifndef KAKERA_ENGINE_COMPONENT_IMAGE
#define KAKERA_ENGINE_COMPONENT_IMAGE

#include "component_base.h"
#include "../copy_and_move.inc"

class Image : public Component
{
public:
    Image();
    ~Image();

    KAKERA_DISABLE_COPY(Image);
    KAKERA_ENABLE_MOVE(Image);

    void render() override;
};

#endif // !KAKERA_ENGINE_COMPONENT_IMAGE
