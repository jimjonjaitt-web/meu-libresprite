#ifndef LIBRESPRITE_CORE_H
#define LIBRESPRITE_CORE_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <algorithm>
#include <queue>

namespace app {

// 32-bit RGBA Pixel Structure
struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    bool operator==(const Color& o) const {
        return r == o.r && g == o.g && b == o.b && a == o.a;
    }
    bool operator!=(const Color& o) const {
        return !(*this == o);
    }
};

// 2D Pixel Image Buffer (equivalent to app::Image)
class Image {
public:
    Image(int width, int height);
    
    int width() const { return m_width; }
    int height() const { return m_height; }

    Color getPixel(int x, int y) const;
    void putPixel(int x, int y, const Color& color);

    const std::vector<Color>& pixels() const { return m_pixels; }
    std::vector<Color>& pixels() { return m_pixels; }

private:
    int m_width;
    int m_height;
    std::vector<Color> m_pixels;
};

// Color Palette (equivalent to app::Palette)
class Palette {
public:
    Palette(int size = 256);
    void setColor(int index, const Color& color);
    Color getColor(int index) const;
private:
    std::vector<Color> m_colors;
};

// Cel (Image at specific frame/layer position)
class Cel {
public:
    Cel(int width, int height, int frameIndex);
    Image& image() { return m_image; }
    const Image& image() const { return m_image; }
    int frameIndex() const { return m_frameIndex; }
private:
    Image m_image;
    int m_frameIndex;
};

// Layer
class Layer {
public:
    Layer(const std::string& name);
    const std::string& name() const { return m_name; }
    void addCel(std::unique_ptr<Cel> cel);
    Cel* getCel(int frameIndex);
private:
    std::string m_name;
    std::vector<std::unique_ptr<Cel>> m_cels;
};

// Document / Sprite Model (equivalent to app::Sprite / app::Doc)
class SpriteDoc {
public:
    SpriteDoc(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

    Layer* addLayer(const std::string& name);
    void setActiveLayer(int index);
    void setActiveFrame(int frameIndex);

    // Drawing Operations (Tools)
    void drawPencil(int x, int y, const Color& color);
    void drawEraser(int x, int y);
    void floodFill(int x, int y, const Color& fillColor);

    Image& activeImage();

private:
    int m_width;
    int m_height;
    int m_activeLayer = 0;
    int m_activeFrame = 0;
    std::vector<std::unique_ptr<Layer>> m_layers;
};

} // namespace app

#endif // LIBRESPRITE_CORE_H
