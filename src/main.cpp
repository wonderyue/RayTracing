#include "Renderer.h"
#include "Scene.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Vector.h"
#include "global.h"

int main(int argc, char** argv) {
    bool bvhEnable = true;
    int sceneIdx = 0;
    int sampleCount = 1;
    if (argc > 1 && atoi(argv[1]) == 0)
        bvhEnable = false;
    if (argc > 2)
        sceneIdx = atoi(argv[2]);
    if (argc > 3)
        sampleCount = atoi(argv[3]);
    
    Scene scene;
    scene.bvhEnable = bvhEnable;
    if (sceneIdx == 0) {
        scene.width = 1200;
        scene.height = 1200;
        scene.camera = Camera(Vec3(0, 2, 9), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, scene.width / scene.height);
        scene.Add(new Sphere(Vec3(-2, 2.5, 2.8), 2.5, METAL, Vec3(0.5)));
        scene.Add(new Sphere(Vec3(5, 3, 1), 3, TRANSPARENT, Vec3(1)));
        scene.Add(new Sphere(Vec3(-2.3, 0.5, 3), 0.5, LAMBERTIAN, Vec3(0.2, 0.3, 0.3)));
        scene.Add(new Sphere(Vec3(2.5, 0.5, 2.5), 0.5, LAMBERTIAN, Vec3(1,0,0)));
        scene.Add(new Sphere(Vec3(3, 2.5, -1.5), 2.5, LAMBERTIAN, Vec3(0, 0, 0.8)));
        scene.Add(new Sphere(Vec3(-3, 0.3, 5), 0.3, LAMBERTIAN, Vec3(0.8, 0.0, 0.3)));
        scene.Add(new Sphere(Vec3(3, 0.5, 4), 0.5, LAMBERTIAN, Vec3(0.5, 0.9, 0.9)));
        scene.Add(new Sphere(Vec3(-4.5, 0.5, 4), 0.5, LAMBERTIAN, Vec3(0, 0.9, 0.3)));
        scene.Add(new MeshTriangle("models/plane.obj", new Material(LAMBERTIAN, Vec3(1)), bvhEnable));
        Sphere* l1 = new Sphere(Vec3(-5, 25, 30), 5, LIGHT, Vec3(1));
        l1->material->kd = 1.0f;
        scene.Add(l1);
        Sphere* l2 = new Sphere(Vec3(5, 30, 40), 3, LIGHT, Vec3(1));
        l2->material->kd = 0.8f;
        scene.Add(l2);
    } else if (sceneIdx == 1) {
        scene.width = 800;
        scene.height = 800;
        scene.camera = Camera(Vec3(-0.2, 2.66, 2.3), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, scene.width / scene.height);

        Material* red = new Material(LAMBERTIAN, Vec3(0.63f, 0.065f, 0.05f));
        red->kd = 1.5 * Vec3(0.63f, 0.065f, 0.05f);
        Material* green = new Material(LAMBERTIAN, Vec3(0.14f, 0.45f, 0.091f));
        green->kd = 1.5 * Vec3(0.14f, 0.45f, 0.091f);
        Material* white = new Material(LAMBERTIAN, Vec3(0.725f, 0.71f, 0.68f));
        white->kd = 1.1 * Vec3(0.725f, 0.71f, 0.68f);
        Material* whiteLight = new Material(LIGHT, Vec3(1));
        whiteLight->kd = Vec3(158.5f);
        Material* box = new Material(LAMBERTIAN, Vec3(1));
        box->kd = Vec3(0.6f);

        MeshTriangle* back = new MeshTriangle("models/back.obj", white, bvhEnable);
        MeshTriangle* ceiling = new MeshTriangle("models/ceiling.obj", white, bvhEnable);
        MeshTriangle* floor = new MeshTriangle("models/floor.obj", white, bvhEnable);
        MeshTriangle* shortbox = new MeshTriangle("models/shortbox.obj", box, bvhEnable);
        MeshTriangle* tallbox = new MeshTriangle("models/tallbox.obj", box, bvhEnable);
        MeshTriangle* left = new MeshTriangle("models/left.obj", red, bvhEnable);
        MeshTriangle* right = new MeshTriangle("models/right.obj", green, bvhEnable);
        MeshTriangle* light = new MeshTriangle("models/light.obj", whiteLight, bvhEnable);
        scene.Add(back);
        scene.Add(ceiling);
        scene.Add(floor);
        scene.Add(shortbox);
        scene.Add(tallbox);
        scene.Add(left);
        scene.Add(right);
        scene.Add(light);
        scene.Add(new Sphere(Vec3(0.5, 2.6, -1.3), 0.8, TRANSPARENT, Vec3(1)));
    } else if (sceneIdx == 2) {
        scene.width = 600;
        scene.height = 600;
        scene.camera = Camera(Vec3(0, 2, 2), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, scene.width / scene.height);
        scene.Add(new MeshTriangle("models/rock.obj", new Material(LAMBERTIAN, Vec3(1)), bvhEnable));
        Sphere* l1 = new Sphere(Vec3(-5, 25, 30), 5, LIGHT, Vec3(1));
        l1->material->kd = 1.0f;
        scene.Add(l1);
        Sphere* l2 = new Sphere(Vec3(5, 30, 40), 3, LIGHT, Vec3(1));
        l2->material->kd = 0.8f;
        scene.Add(l2);
    }
    scene.buildBVH();
    Renderer r;
    time_t startTime = time(NULL);
    r.Render(scene, sampleCount);
    time_t stopTime = time(NULL);
    long seconds = stopTime - startTime;
    long minutes = seconds / 60;
    seconds %= 60;
    std::cout << "\nComplete " << (bvhEnable ? "with" : "without") << " BVH! Time token: " << std::setw(2)
    << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds <<std::endl;
    return 0;
}
