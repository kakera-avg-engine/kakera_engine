#ifndef KAKERA_ENGINE_COMPONENT_BASE
#define KAKERA_ENGINE_COMPONENT_BASE

class Component
{
public:
    virtual ~Component() = 0;
    virtual void render() = 0;
};

#endif // !KAKERA_ENGINE_COMPONENT_BASE
