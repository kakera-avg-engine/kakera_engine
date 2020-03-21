#ifndef KAKERA_ENGINE_COMPONENT_BASE
#define KAKERA_ENGINE_COMPONENT_BASE

#include "../graphic/render_object.h"
#include "../graphic/texture.h"
#include "../copy_and_move.inc"
#include <list>
#include <utility>
#include <string>

class Component
{
protected:
    int width = 0, height = 0;
    int x = 0, y = 0;
    std::string uuid;
public:
    Component* parent = nullptr;
    std::list<std::unique_ptr<Component>> children;

    bool is(std::string uuid)
    {
        return uuid == this->uuid;
    }

    void set_uuid(std::string uuid)
    {
        this->uuid = uuid;
    }

    std::pair<int, int> get_size()
    {
        return std::make_pair(width, height);
    }

    virtual void set_size(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    std::pair<int, int> get_position()
    {
        return std::make_pair(x, y);
    }

    virtual void set_position(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    virtual ~Component() {};
    virtual void render() {};
};

using Root = Component;

// Root type id = 0

#endif // !KAKERA_ENGINE_COMPONENT_BASE
