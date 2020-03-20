#define KAKERA_DISABLE_COPY(name) name(const name&) = delete; name& operator=(const name&) = delete
#define KAKERA_DISABLE_MOVE(name) name(name&&) = delete; name& operator=(name&&) = delete