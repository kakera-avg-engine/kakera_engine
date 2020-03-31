#include "page.h"

#define node_name_is(str) strcmp(node.name(),str)==0

PageDisplayMode int_to_display_mode(int mode_int)
{
    if (mode_int == 0) return PageDisplayMode::Unique;
    else return PageDisplayMode::Shared;
}

#if (defined(_DEBUG) || !defined(NDEBUG))
bool Page::XMLTreeWalker::for_each(pugi::xml_node& node)
{
    if (node_name_is("DisplayMode")) {
        if (strcmp(node.child_value(), "unique") == 0) page->mode = PageDisplayMode::Unique;
        else if (strcmp(node.child_value(), "shared") == 0) page->mode = PageDisplayMode::Shared;
    }

    if (depth() >= 2 && (!node_name_is("DisplayMode") && !node_name_is("Include"))) {
        Component* parent = nullptr;
        std::string parent_uuid = node.parent().attribute("uuid").as_string();

        if (depth() == 2) parent = &page->root;
        else {
            for (auto com : page->component_tree_cache) {
                if (com->is(parent_uuid)) {
                    parent = com;
                    break;
                }
            }
        }

        if (!parent) return true;

        if (node_name_is("Image")) {
            parent->children.emplace_back(std::make_unique<Image>());
            parent->children.back()->parent = parent;

            Image* image = (Image*)parent->children.back().get();
            image->set_uuid(node.attribute("uuid").as_string());
            int w, h, x, y;
            w = node.attribute("width").as_int();
            h = node.attribute("height").as_int();
            x = node.attribute("x").as_int();
            y = node.attribute("y").as_int();
            image->set_size(w, h);
            image->set_position(x, y);
            image->set_src(node.attribute("src").as_string());
        }

        if (node_name_is("SingleLineText")) {
            parent->children.emplace_back(std::make_unique<SingleLineText>());
            parent->children.back()->parent = parent;

            SingleLineText* sl_text = (SingleLineText*)parent->children.back().get();
            sl_text->set_uuid(node.attribute("uuid").as_string());
            int x = node.attribute("x").as_int();
            int y = node.attribute("y").as_int();
            sl_text->set_position(x, y);
            sl_text->set_font(node.attribute("font").as_string());
            sl_text->set_fontsize(node.attribute("font-size").as_int());
            sl_text->set_color(node.attribute("color").as_string());
            std::string str = node.child_value();
            StringTrim::trim(str);
            sl_text->set_string(str);
            sl_text->set_spacing(node.attribute("spacing").as_int());
        }
    }

    return true;
}
#endif

void Page::refresh_cache()
{
    if (!component_tree_cache.empty()) component_tree_cache.clear();

    std::stack<Component*> temp_stack;
    temp_stack.push(&root);
    while (!temp_stack.empty()) {
        Component* component = temp_stack.top();
        temp_stack.pop();

        component_tree_cache.push_back(component);

        if (!component->children.empty()) {
            for (auto iter = component->children.rbegin(); iter != component->children.rend(); iter++) {
                temp_stack.push((*iter).get());
            }
        }
    }
}

Page::Page(PageDisplayMode mode)
{
    this->mode = mode;
}

Page::Page(std::string src, std::string id)
{
    this->id = id;
#if (!defined(_DEBUG) || defined(NDEBUG))
#else
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(src.c_str());
    if (result.status != pugi::status_ok) return;

    XMLTreeWalker walker;
    walker.page = this;
    doc.traverse(walker);

    refresh_cache();
#endif
}

Page::~Page()
{
    if (glIsRenderbuffer(rbo)) glDeleteRenderbuffers(1, &rbo);
    if (glIsFramebuffer(fbo)) glDeleteFramebuffers(1, &fbo);
}

PageDisplayMode Page::get_display_mode()
{
    return mode;
}

void Page::resize(int width, int height)
{
    root.set_size(width, height);

    render_obj.reset(new RenderObject());
    render_obj->set_size(width, height);
    render_obj->set_shader(&KAKERA_SHADER_NORMAL);

    if (glIsFramebuffer(fbo)) glDeleteFramebuffers(1, &fbo);
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    KAKERA_TEXTURE_MANAGER.destroy_texture(id);
    KAKERA_TEXTURE_MANAGER.set_texture(id, Texture(width, height));
    Texture* color_attachment = KAKERA_TEXTURE_MANAGER[id];
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment->get_id(), 0);
    render_obj->set_texture(color_attachment, RenderObject::upside_down_view);

    if (glIsRenderbuffer(rbo)) glDeleteRenderbuffers(1, &rbo);
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Page::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto com : component_tree_cache) {
        if (com->parent) {
            com->render();
        }
    }
}

void Page::render()
{
    render_obj->render();
}
