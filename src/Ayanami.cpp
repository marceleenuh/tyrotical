#include "Ayanami.hpp"
#include "tyrotical.hpp"

int main(void) {
    Tyrotical::TyroticalApp app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        printf("%s\n", e.what());
        return 1;
    }

    return 0;
}