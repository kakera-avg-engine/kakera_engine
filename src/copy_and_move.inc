#define KAKERA_DISABLE_COPY(name) name(const name&) = delete; name& operator=(const name&) = delete
#define KAKERA_DISABLE_MOVE(name) name(name&&) = delete; name& operator=(name&&) = delete
#define KAKERA_ENABLE_COPY(name) name(const name& other); name& operator=(const name& other)
#define KAKERA_ENABLE_MOVE(name) name(name&& other); name& operator=(name&& other)