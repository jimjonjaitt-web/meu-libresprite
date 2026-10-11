#include "LibreSpriteCore.h"
#include <iostream>

int main() {
    std::cout << "Initializing LibreSprite C++ Core Engine...\n";

    // Create a 32x32 sprite document (matching app::Sprite / app::Doc)
    app::SpriteDoc doc(32, 32);

    // Test drawing with pencil tool
    app::Color red{255, 0, 0, 255};
    app::Color blue{0, 0, 255, 255};

    doc.drawPencil(10, 10, red);
    doc.drawPencil(11, 10, red);
    doc.drawPencil(10, 11, red);
    doc.drawPencil(11, 11, red);

    auto pixel = doc.activeImage().getPixel(10, 10);
    std::cout << "Pixel at (10, 10) R:" << (int)pixel.r << " G:" << (int)pixel.g << " B:" << (int)pixel.b << "\n";

    // Test Flood Fill
    doc.floodFill(0, 0, blue);
    auto fillPixel = doc.activeImage().getPixel(0, 0);
    std::cout << "Floodfill pixel at (0, 0) B:" << (int)fillPixel.b << "\n";

    std::cout << "LibreSprite C++ Core Engine verified successfully!\n";
    return 0;
}
