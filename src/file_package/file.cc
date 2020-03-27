#include "file.h"

File::File(File&& other)
{
    _size = other._size;
    other._size = 0;
    file_ptr = std::move(other.file_ptr);
}

File& File::operator=(File&& other)
{
    if (this != &other) {
        _size = other._size;
        other._size = 0;
        file_ptr = std::move(other.file_ptr);
    }

    return *this;
}

File::File(size_t size, char* file_ptr)
{
    this->size = size;
    this->file_ptr = std::unique_ptr<char>(file_ptr);
}

bool File::empty()
{
    return _size == 0 || file_ptr == nullptr;
}

File::operator bool()
{
    return _size == 0 || file_ptr == nullptr;
}

size_t File::size()
{
    return _size;
}

char* File::get()
{
    return file_ptr.get();
}

char* File::operator*()
{
    return file_ptr.get();
}
