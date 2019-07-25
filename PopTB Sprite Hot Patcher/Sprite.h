/*
    Populous The Beginning Sprite Patcher
    Brandan Tyler Lasley 2019 (7/25/2019)
*/
#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <memory>

#define SPRITE_MAGIC_SIZE 4

struct sprite_header
{
    int8_t      magic[SPRITE_MAGIC_SIZE];
    uint32_t    frame_count;
};

struct sprite_frame_header
{
    uint16_t width;
    uint16_t height;
    uint32_t offset;
};

struct TbSprite
{
    struct sprite_frame_header* header;
    int8_t*                     data;
};

struct SpriteFile
{
    sprite_header*						header;
    std::vector<TbSprite>               sprite_data;
};

class Sprite
{
public:
    Sprite() noexcept : _raw_file(nullptr), _raw_file_size(NULL), _data({ nullptr,{} }) {}

    Sprite(const std::string & file) : _raw_file(nullptr), _raw_file_size(NULL), _data({ nullptr, {} })
    {
        loadspr(file);
    }

    ~Sprite()
    {
        clean(_data);
    }

    void loadspr(const std::string & file)
    {
        std::ifstream reader(file, std::ios::binary | std::ios::ate);

        if (reader.is_open())
        {
            // Load file in full
            uint32_t byteLoc = 0;
            _raw_file_size = reader.tellg();
            reader.seekg(std::ios::beg);
            _raw_file = new char[static_cast<size_t>(_raw_file_size)];
            reader.read(_raw_file, _raw_file_size);
            reader.close();

            // Pointer Setup //

            // Header Pointer Setup
            _data.header = reinterpret_cast<sprite_header*>(_raw_file);

            // Increase byte location to add the size of sprite header.
            // This is so that we can ignore the sprite header when adding
            // the frame headers. 
            byteLoc += sizeof(sprite_header);

            // Frame Headers 
            for (uint32_t i = 0; i < _data.header->frame_count; i++)
            {
                _data.sprite_data.push_back({ reinterpret_cast<sprite_frame_header*>((_raw_file + byteLoc) + (i * sizeof(sprite_frame_header))), nullptr });
                _data.sprite_data.back().data = reinterpret_cast<int8_t*>(_raw_file + _data.sprite_data.back().header->offset);
            }

            reader.close();
        }
    }

    void savespr(const std::string & file)
    {
        std::ofstream writer(file, std::ios::binary | std::ios::trunc);

        if (writer.is_open())
        {
            writer.write(_raw_file, _raw_file_size);
            writer.close();
        }
    }

    void reset()
    {
        clean(_data);
    }

    template<typename lambda>
    TbSprite* processSprites(lambda func)
    {
        for (auto & spr_ptr : _data.sprite_data)
        {
            if (!func(spr_ptr))
                return &spr_ptr;
        }
        return nullptr;
    }
private:
    void clean(SpriteFile & sf)
    {
        sf.header = nullptr;
        sf.sprite_data.clear();
        delete[] _raw_file;
        _raw_file = nullptr;
        _raw_file_size = 0;
    }

    char*               _raw_file;
    std::ios::pos_type  _raw_file_size;
    struct SpriteFile   _data;
};