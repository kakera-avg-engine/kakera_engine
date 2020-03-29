#include "render_object.h"

TextureView RenderObject::normal_view = GLVertices({
    0.0f, 1.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 0.0f
});

TextureView RenderObject::upside_down_view = GLVertices({
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 1.0f
});

GLVertices RenderObject::method_to_vertices(TextureApplyMethod method)
{
    // TODO
    return GLVertices({
        0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    });
}

RenderObject::RenderObject()
{
    GLVertices vertices = {
        0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 12, vertices.data());

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)(sizeof(GLfloat) * 12));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

RenderObject::RenderObject(RenderObject&& other)
{
    vao = other.vao;
    vbo = other.vbo;
    texture = other.texture;
    shader = other.shader;
    width = other.width;
    height = other.height;
    x = other.x;
    y = other.y;
    rotation_angle = other.rotation_angle;
    opacity = other.opacity;
    is_attach_color = other.is_attach_color;
    attached_color = other.attached_color;

    other.vao = UINT_MAX;
    other.vbo = UINT_MAX;
    other.texture = nullptr;
    other.shader = nullptr;
    other.width = 0;
    other.height = 0;
    other.x = 0;
    other.y = 0;
    other.rotation_angle = 0.0f;
    other.opacity = 255;
    other.is_attach_color = false;
    other.attached_color = { 0, 0, 0, 0 };
}

RenderObject::~RenderObject()
{
    if (glIsBuffer(vbo)) glDeleteBuffers(1, &vbo);
    if (glIsVertexArray(vao)) glDeleteVertexArrays(1, &vao);
}

RenderObject& RenderObject::operator=(RenderObject&& other)
{
    if (&other != this) {
        vao = other.vao;
        vbo = other.vbo;
        texture = other.texture;
        shader = other.shader;
        width = other.width;
        height = other.height;
        x = other.x;
        y = other.y;
        rotation_angle = other.rotation_angle;
        opacity = other.opacity;
        is_attach_color = other.is_attach_color;
        attached_color = other.attached_color;

        other.vao = UINT_MAX;
        other.vbo = UINT_MAX;
        other.texture = nullptr;
        other.shader = nullptr;
        other.width = 0;
        other.height = 0;
        other.x = 0;
        other.y = 0;
        other.rotation_angle = 0.0f;
        other.opacity = 255;
        other.is_attach_color = false;
        other.attached_color = { 0, 0, 0, 0 };
    }

    return *this;
}

void RenderObject::set_texture(Texture* texture, TextureView texture_view)
{
    this->texture = texture;
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    if (auto vertices = std::get_if<GLVertices>(&texture_view)) 
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, vertices->data());
    else {
        TextureApplyMethod method = std::get<TextureApplyMethod>(texture_view);
        GLVertices tex_coord = method_to_vertices(method);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, tex_coord.data());
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderObject::set_shader(ShaderBase* shader)
{
    this->shader = shader;
}

std::pair<int, int> RenderObject::get_size()
{
    return std::make_pair(width, height);
}

void RenderObject::set_size(int width, int height)
{
    this->width = width;
    this->height = height;
}

std::pair<int, int> RenderObject::get_position()
{
    return std::make_pair(x, y);
}

void RenderObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}

float RenderObject::get_rotation_angle()
{
    return rotation_angle;
}

void RenderObject::set_rotation_angle(float angle)
{
    rotation_angle = angle;
}

uint8_t RenderObject::get_opacity()
{
    return opacity;
}

void RenderObject::set_opacity(uint8_t opacity)
{
    this->opacity = opacity;
}

void RenderObject::attach_color(SDL_Color color)
{
    if (!is_attach_color) is_attach_color = true;
    attached_color = color;
}

void RenderObject::cancel_attach_color()
{
    if (is_attach_color) is_attach_color = false;
}

Texture* RenderObject::get_texture()
{
    return texture;
}

void RenderObject::render()
{
    if (!texture || !shader) return;

    shader->use();

    glm::mat4 model = glm::mat4(1.0f);

    float fx = static_cast<float>(x), fy = static_cast<float>(y);
    float fw = static_cast<float>(width), fh = static_cast<float>(height);

    model = glm::translate(model, glm::vec3(fx, fy, 0.0f));

    if (rotation_angle != 0.0f) {
        model = glm::translate(model, glm::vec3(0.5f * fw, 0.5f * fh, 0.0f));
        model = glm::rotate(model, glm::radians(rotation_angle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * fw, -0.5f * fh, 0.0f));
    }

    model = glm::scale(model, glm::vec3(fw, fh, 1.0f));

    shader->set_model(model);

    if (is_attach_color) shader->attach_color(attached_color);

    shader->set_opacity(opacity);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
