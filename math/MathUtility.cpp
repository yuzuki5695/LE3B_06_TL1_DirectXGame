#include"MathUtility.h"
#include <cmath>

namespace MathUtility {

Matrix4 Matrix4Transform(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
    Matrix4 matScale = {
        scale.x, 0.0f,    0.0f,    0.0f,
        0.0f,    scale.y, 0.0f,    0.0f,
        0.0f,    0.0f,    scale.z, 0.0f,
        0.0f,    0.0f,    0.0f,    1.0f
    };

    float cX = std::cos(rotation.x);
    float sX = std::sin(rotation.x);
    float cY = std::cos(rotation.y);
    float sY = std::sin(rotation.y);
    float cZ = std::cos(rotation.z);
    float sZ = std::sin(rotation.z);

    Matrix4 matRotX = {
        1, 0,  0, 0,
        0, cX, sX, 0,
        0, -sX, cX, 0,
        0, 0,  0, 1
    };

    Matrix4 matRotY = {
        cY, 0, -sY, 0,
        0,  1, 0,   0,
        sY, 0, cY,  0,
        0,  0, 0,   1
    };

    Matrix4 matRotZ = {
        cZ, sZ, 0, 0,
        -sZ, cZ, 0, 0,
        0,  0,  1, 0,
        0,  0,  0, 1
    };

    Matrix4 matTrans = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation.x, translation.y, translation.z, 1.0f
    };

    // SRTèáÇ…çáê¨ÅFÉXÉPÅ[Éã Å® âÒì]X Å® âÒì]Y Å® âÒì]Z Å® ïΩçsà⁄ìÆ
    Matrix4 matRot = matRotX * matRotY * matRotZ;
    Matrix4 matWorld = matScale * matRot * matTrans;

    return matWorld;
}

} // namespace MathUtility
