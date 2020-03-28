#ifndef KAKERA_ENGINE_GRAPHIC_PAGE
#define KAKERA_ENGINE_GRAPHIC_PAGE

#include "glad/glad.h"
#include "../graphic/render_object.h"
#include <memory>
#include <climits>
#include <string>
#include <cstring>
#include <iterator>
#include <stack>
#include <vector>
#include "../graphic/texture_manager.h"
#include "component_base.hpp"
#include "../graphic/shader_normal.hpp"
#include "../copy_and_move.inc"
#if (!defined(_DEBUG) || defined(NDEBUG))
#   include "msgpack.hpp"
#else
#   include "pugixml.hpp"
#include <iostream>
#endif

#include "image.hpp"

enum class PageDisplayMode
{
    Unique,
    Shared
};

PageDisplayMode int_to_display_mode(int mode_int);

class Page
{
private:
#if (defined(_DEBUG) || !defined(NDEBUG))
    struct XMLTreeWalker : pugi::xml_tree_walker
    {
        Page* page = nullptr;
        bool for_each(pugi::xml_node& node) override;
    };
#endif
    std::string uuid;

    GLuint fbo = UINT_MAX, rbo = UINT_MAX;
    std::unique_ptr<RenderObject> render_obj;

    PageDisplayMode mode;
    Component root;

    std::list<Component*> component_tree_cache;

    void refresh_cache();
public:
    Page(PageDisplayMode mode = PageDisplayMode::Unique);
    Page(std::string src);
    ~Page();

    KAKERA_DISABLE_COPY(Page);
    KAKERA_DISABLE_MOVE(Page);

    PageDisplayMode get_display_mode();

    void resize(int width, int height);
    void draw();
    void render();
};

#endif // !KAKERA_ENGINE_GRAPHIC_PAGE
