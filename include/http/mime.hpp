#include <filesystem>
#include <magic.h>

namespace espresso {

inline static std::string mime_type(const std::filesystem::path& file)
{
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    magic_load(magic, nullptr);
    const char* mime = magic_file(magic, file.c_str());
    std::string mime_str = mime;
    magic_close(magic);
    return mime_str;
}

}// namespace espresso