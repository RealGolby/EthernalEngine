#pragma once


class Texture
{
public:
    int width{}, height{}, nrChannels{}, textureIndex;
    unsigned char *data;
    unsigned int TextureID{};
public:
    explicit Texture(const char* imagePath, int textureIndex);
    ~Texture();
    void Bind() const;
};