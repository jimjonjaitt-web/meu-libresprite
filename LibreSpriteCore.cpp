#include "LibreSpriteCore.h"
#include <stdexcept>

namespace app {

// --- Image Implementation ---
Image::Image(int width, int height)
    : m_width(width), m_height(height), m_pixels(width * height, Color{0, 0, 0, 0}) {}

Color Image::getPixel(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return Color{0, 0, 0, 0};
    return m_pixels[y * m_width + x];
}

void Image::putPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_pixels[y * m_width + x] = color;
    }
}

// --- Palette Implementation ---
Palette::Palette(int size) : m_colors(size, Color{0, 0, 0, 255}) {}

void Palette::setColor(int index, const Color& color) {
    if (index >= 0 && index < (int)m_colors.size()) {
        m_colors[index] = color;
    }
}

Color Palette::getColor(int index) const {
    if (index >= 0 && index < (int)m_colors.size()) {
        return m_colors[index];
    }
    return Color{0, 0, 0, 255};
}

// --- Cel Implementation ---
Cel::Cel(int width, int height, int frameIndex)
    : m_image(width, height), m_frameIndex(frameIndex) {}

// --- Layer Implementation ---
Layer::Layer(const std::string& name) : m_name(name) {}

void Layer::addCel(std::unique_ptr<Cel> cel) {
    m_cels.push_back(std::move(cel));
}

Cel* Layer::getCel(int frameIndex) {
    for (auto& cel : m_cels) {
        if (cel->frameIndex() == frameIndex) {
            return cel.get();
        }
    }
    return nullptr;
}

// --- SpriteDoc Implementation ---
SpriteDoc::SpriteDoc(int width, int height) : m_width(width), m_height(height) {
    // Create default background layer and cel for frame 0
    auto layer = addLayer("Background");
    auto cel = std::make_unique<Cel>(width, height, 0);
    layer->addCel(std::move(cel));
}

Layer* SpriteDoc::addLayer(const std::string& name) {
    auto layer = std::make_unique<Layer>(name);
    m_layers.push_back(std::move(layer));
    return m_layers.back().get();
}

void SpriteDoc::setActiveLayer(int index) {
    if (index >= 0 && index < (int)m_layers.size()) {
        m_activeLayer = index;
    }
}

void SpriteDoc::setActiveFrame(int frameIndex) {
    m_activeFrame = frameIndex;
}

Image& SpriteDoc::activeImage() {
    if (m_layers.empty()) {
        throw std::runtime_error("No layers in SpriteDoc");
    }
    Layer* layer = m_layers[m_activeLayer].get();
    Cel* cel = layer->getCel(m_activeFrame);
    if (!cel) {
        // Automatically create cel if missing for this frame
        auto newCel = std::make_unique<Cel>(m_width, m_height, m_activeFrame);
        cel = newCel.get();
        layer->addCel(std::move(newCel));
    }
    return cel->image();
}

void SpriteDoc::drawPencil(int x, int y, const Color& color) {
    activeImage().putPixel(x, y, color);
}

void SpriteDoc::drawEraser(int x, int y) {
    activeImage().putPixel(x, y, Color{0, 0, 0, 0});
}

void SpriteDoc::floodFill(int x, int y, const Color& fillColor) {
    Image& img = activeImage();
    if (x < 0 || x >= img.width() || y < 0 || y >= img.height())
        return;

    Color targetColor = img.getPixel(x, y);
    if (targetColor == fillColor)
        return;

    std::queue<std::pair<int, int>> q;
    q.push({x, y});

    std::vector<bool> visited(img.width() * img.height(), false);

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        if (cx < 0 || cx >= img.width() || cy < 0 || cy >= img.height())
            continue;

        int idx = cy * img.width() + cx;
        if (visited[idx])
            continue;
        visited[idx] = true;

        if (img.getPixel(cx, cy) == targetColor) {
            img.putPixel(cx, cy, fillColor);
            q.push({cx + 1, cy});
            q.push({cx - 1, cy});
            q.push({cx, cy + 1});
            q.push({cx, cy - 1});
        }
    }
}

} // namespace app
