#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Використання: " << argv[0] << " <шлях_до_файлу>\n";
        return 1;
    }
    std::cout << "Файл: " << argv[1] << '\n';
    return 0;
}