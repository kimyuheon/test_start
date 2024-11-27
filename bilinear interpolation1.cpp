#include <iostream>
#include <vector>
#include <stdexcept>

// 이중 선형 보간 함수
double bilinearInterpolation(
    double x, double y,
    double x1, double y1, double z11,
    double x2, double y2, double z12,
    double x3, double y3, double z21,
    double x4, double y4, double z22) {
    
    // x 방향 선형 보간
    double z1 = z11 + (z12 - z11) * (x - x1) / (x2 - x1);
    double z2 = z21 + (z22 - z21) * (x - x3) / (x4 - x3);

    // y 방향 선형 보간
    return z1 + (z2 - z1) * (y - y1) / (y3 - y1);
}

int main() {
    // 4개의 기준점과 z 값
    double x1 = 0, y1 = 0, z11 = 10;
    double x2 = 1, y2 = 0, z12 = 20;
    double x3 = 0, y3 = 1, z21 = 15;
    double x4 = 1, y4 = 1, z22 = 25;

    // 보간할 점
    double x, y;
    std::cout << "Enter x and y for interpolation: ";
    std::cin >> x >> y;

    try {
        // 보간 수행
        double z = bilinearInterpolation(x, y, x1, y1, z11, x2, y2, z12, x3, y3, z21, x4, y4, z22);
        std::cout << "Interpolated z value at (" << x << ", " << y << ") is " << z << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
