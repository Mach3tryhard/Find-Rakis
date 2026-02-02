#include "code/GameManager.h"

int main() {
    try {
        GameManager::getInstance().Run();
    }
    catch (const ResourceLoadException& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    catch (const GenerationException& e) {
        std::cerr << e.what() << "\n";
        return 2;
    }
    catch (const PhysicsException& e) {
        std::cerr << e.what() << "\n";
        return 3;
    }
    catch (const std::exception& e) {
        std::cerr << "Nu se stie error: " << e.what() << "\n";
        return -1;
    }
    return 0;
}
