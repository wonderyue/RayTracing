#include <fstream>

#include "Renderer.h"
#include "Scene.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void Renderer::Render(const Scene& scene, int sampleCount) {
    unsigned char data[scene.width * scene.height * 3];
    int m = 0;
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
            Vec3 color = Vec3();
            for (int _ = 0; _ < sampleCount; _++) {
                float y = 1 - (j + rand_n1_1()) / (float)scene.height;
                float x = (i + rand_n1_1()) / (float)scene.width;
                color += scene.rayCastColor(scene.camera.getRay(x, y), 0);
            }
            for (int k = 0; k < 3; k++) {
                data[m++] = (unsigned char)(255 * clamp(0, 1, color[k] / sampleCount));
            }
        }
        updateProgress(j / (float)scene.height);
    }
    updateProgress(1.f);
    stbi_write_png("output.png", scene.width, scene.height, 3, data, 0);
}
