#ifndef KAKERA_ENGINE_GRAPHIC_PAGE
#define KAKERA_ENGINE_GRAPHIC_PAGE

#include "glad/glad.h"
#include "../graphic/texture.h"
#include <memory>
#include "../copy_and_move.inc"

class Page
{
private:
    unsigned int fbo, rbo;
    std::unique_ptr<Texture> render_target;
public:
    Page();
    ~Page();

    KAKERA_DISABLE_COPY(Page);
};

#endif // !KAKERA_ENGINE_GRAPHIC_PAGE
