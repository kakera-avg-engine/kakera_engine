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
#include <regex>
#include "../graphic/texture_manager.h"
#include "component_base.hpp"
#include "../graphic/shader_normal.hpp"
#include "../string_tools.hpp"
#include "../copy_and_move.inc"
#if (!defined(_DEBUG) || defined(NDEBUG))
#   include "msgpack.hpp"
#else
#   include "pugixml.hpp"
#include <iostream>
#endif

#include "image.hpp"
#include "text.hpp"
#include "video.hpp"

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
    std::string id;

    GLuint fbo = UINT_MAX, rbo = UINT_MAX;
    std::unique_ptr<RenderObject> render_obj;

    PageDisplayMode mode;
    Component root;

    void refresh_cache();
public:
    std::list<Component*> component_tree_cache;

    Page(PageDisplayMode mode = PageDisplayMode::Unique);
    Page(std::string src, std::string id);
    ~Page();

    KAKERA_DISABLE_COPY(Page);
    KAKERA_DISABLE_MOVE(Page);

    PageDisplayMode get_display_mode();

    void resize(int width, int height);
    void draw();
    void render();

    void on_active();
    void on_suspend();
};

#endif // !KAKERA_ENGINE_GRAPHIC_PAGE
