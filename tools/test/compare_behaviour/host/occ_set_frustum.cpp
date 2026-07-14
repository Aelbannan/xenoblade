#include <cmath>
#include <cstdio>
#include <cstdlib>

namespace {

const unsigned int kFlags01 = 3;
const float kEpsilon = 0.0001f;

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Plane {
    Vec3 normal;
    float distance;
};

struct Frustum {
    Vec3 pos;
    Vec3 rot;
    Vec3 scale;
    Vec3 direction;
    Vec3 corners[4];
    Plane planes[5];
    float nearValue;
    float radius;
    unsigned int flags;
};

static Vec3 subtract(const Vec3& a, const Vec3& b) {
    Vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

static float dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static Vec3 cross(const Vec3& a, const Vec3& b) {
    Vec3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
    };
    return result;
}

static Vec3 normalize(const Vec3& v) {
    float lengthSq = dot(v, v);
    if (lengthSq == 0.0f) {
        Vec3 zero = {0.0f, 0.0f, 0.0f};
        return zero;
    }
    float reciprocalLength = 1.0f / std::sqrt(lengthSq);
    Vec3 result = {v.x * reciprocalLength, v.y * reciprocalLength, v.z * reciprocalLength};
    return result;
}

static Vec3 rotateXYZ(const Vec3& v, const Vec3& angles) {
    float sx = std::sin(angles.x);
    float cx = std::cos(angles.x);
    float sy = std::sin(angles.y);
    float cy = std::cos(angles.y);
    float sz = std::sin(angles.z);
    float cz = std::cos(angles.z);

    Vec3 xRotated = {v.x, v.y * cx - v.z * sx, v.y * sx + v.z * cx};
    Vec3 yRotated = {
        xRotated.x * cy + xRotated.z * sy,
        xRotated.y,
        -xRotated.x * sy + xRotated.z * cy,
    };
    Vec3 result = {
        yRotated.x * cz - yRotated.y * sz,
        yRotated.x * sz + yRotated.y * cz,
        yRotated.z,
    };
    return result;
}

static Vec3 transformPoint(const Frustum& frustum, const Vec3& point) {
    Vec3 scaled = {point.x * frustum.scale.x, point.y * frustum.scale.y, point.z * frustum.scale.z};
    Vec3 rotated = rotateXYZ(scaled, frustum.rot);
    Vec3 result = {rotated.x + frustum.pos.x, rotated.y + frustum.pos.y, rotated.z + frustum.pos.z};
    return result;
}

static Plane makeEdgePlane(const Vec3& from, const Vec3& to) {
    Plane plane;
    plane.normal = normalize(subtract(to, from));
    plane.distance = -dot(from, plane.normal);
    return plane;
}

static Plane makeFacePlane(const Vec3& p0, const Vec3& p1, const Vec3& p2) {
    Plane plane;
    plane.normal = normalize(cross(subtract(p1, p0), subtract(p2, p0)));
    plane.distance = -dot(p0, plane.normal);
    return plane;
}

// Retail oracle reconstructed from setFrustum__11COccCullingFP12CCullFrustum:
// scale -> rotate -> translate, transform the four static corners, track the
// farthest squared point distance, then build plane 0 only for FLAGS_01.
static void retail_set_frustum(Frustum& frustum) {
    static const Vec3 kPlaneCoords[4] = {
        {-0.5f, 0.0f, 0.0f},
        {0.5f, 0.0f, 0.0f},
        {0.5f, 1.0f, 0.0f},
        {-0.5f, 1.0f, 0.0f},
    };

    frustum.direction = normalize(rotateXYZ({0.0f, 0.0f, 1.0f}, frustum.rot));
    frustum.nearValue = 0.0f;
    frustum.radius = 0.0f;
    for (int i = 0; i < 4; ++i) {
        frustum.corners[i] = transformPoint(frustum, kPlaneCoords[i]);
        Vec3 delta = subtract(frustum.pos, frustum.corners[i]);
        float magnitude = dot(delta, delta);
        if (frustum.radius < magnitude) {
            frustum.radius = magnitude;
        }
    }
    frustum.radius = std::sqrt(frustum.radius);

    if (frustum.flags & kFlags01) {
        frustum.planes[0] = makeFacePlane(frustum.corners[0], frustum.corners[1], frustum.corners[2]);
    }
    frustum.planes[1] = makeEdgePlane(frustum.corners[0], frustum.corners[1]);
    frustum.planes[2] = makeEdgePlane(frustum.corners[1], frustum.corners[2]);
    frustum.planes[3] = makeEdgePlane(frustum.corners[2], frustum.corners[3]);
    frustum.planes[4] = makeEdgePlane(frustum.corners[3], frustum.corners[0]);
}

// High-level mirror of COccCulling::setFrustum.
static void decomp_set_frustum(Frustum& frustum) {
    static const Vec3 kPlaneCoords[4] = {
        {-0.5f, 0.0f, 0.0f},
        {0.5f, 0.0f, 0.0f},
        {0.5f, 1.0f, 0.0f},
        {-0.5f, 1.0f, 0.0f},
    };

    frustum.direction = normalize(rotateXYZ({0.0f, 0.0f, 1.0f}, frustum.rot));
    frustum.nearValue = 0.0f;
    frustum.radius = 0.0f;
    for (int i = 0; i < 4; ++i) {
        frustum.corners[i] = transformPoint(frustum, kPlaneCoords[i]);
        float magnitude = dot(subtract(frustum.pos, frustum.corners[i]), subtract(frustum.pos, frustum.corners[i]));
        if (frustum.radius < magnitude) {
            frustum.radius = magnitude;
        }
    }
    frustum.radius = std::sqrt(frustum.radius);

    if (frustum.flags & kFlags01) {
        frustum.planes[0] = makeFacePlane(frustum.corners[0], frustum.corners[1], frustum.corners[2]);
    }
    frustum.planes[1] = makeEdgePlane(frustum.corners[0], frustum.corners[1]);
    frustum.planes[2] = makeEdgePlane(frustum.corners[1], frustum.corners[2]);
    frustum.planes[3] = makeEdgePlane(frustum.corners[2], frustum.corners[3]);
    frustum.planes[4] = makeEdgePlane(frustum.corners[3], frustum.corners[0]);
}

static bool closeEnough(float a, float b) {
    return std::fabs(a - b) <= kEpsilon;
}

static bool sameVec(const Vec3& a, const Vec3& b) {
    return closeEnough(a.x, b.x) && closeEnough(a.y, b.y) && closeEnough(a.z, b.z);
}

static bool samePlane(const Plane& a, const Plane& b) {
    return sameVec(a.normal, b.normal) && closeEnough(a.distance, b.distance);
}

static bool sameFrustum(const Frustum& retail, const Frustum& decomp, bool expectsPlane0) {
    if (!sameVec(retail.direction, decomp.direction) || !closeEnough(retail.nearValue, decomp.nearValue) ||
        !closeEnough(retail.radius, decomp.radius)) {
        return false;
    }
    for (int i = 0; i < 4; ++i) {
        if (!sameVec(retail.corners[i], decomp.corners[i])) {
            return false;
        }
    }
    for (int i = expectsPlane0 ? 0 : 1; i < 5; ++i) {
        if (!samePlane(retail.planes[i], decomp.planes[i])) {
            return false;
        }
    }
    return true;
}

static void run_scenario(const char* name, Vec3 pos, Vec3 rot, Vec3 scale, unsigned int flags) {
    Frustum retail = {};
    Frustum decomp = {};
    retail.pos = decomp.pos = pos;
    retail.rot = decomp.rot = rot;
    retail.scale = decomp.scale = scale;
    retail.flags = decomp.flags = flags;

    retail_set_frustum(retail);
    decomp_set_frustum(decomp);
    if (!sameFrustum(retail, decomp, (flags & kFlags01) != 0)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

} // namespace

int main() {
    run_scenario("identity_flags_zero", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 0);
    run_scenario("identity_flag_0", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 1);
    run_scenario("identity_flag_1", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 2);
    run_scenario("identity_flags_01", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 3);
    run_scenario("unrelated_flag", {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, 4);
    run_scenario("translated_positive", {3, 4, 5}, {0, 0, 0}, {1, 1, 1}, 0);
    run_scenario("translated_negative", {-7, 2, -9}, {0, 0, 0}, {1, 1, 1}, 3);
    run_scenario("x_rotation", {0, 0, 0}, {0.7f, 0, 0}, {1, 1, 1}, 0);
    run_scenario("y_rotation", {0, 0, 0}, {0, -1.1f, 0}, {1, 1, 1}, 1);
    run_scenario("z_rotation", {0, 0, 0}, {0, 0, 2.2f}, {1, 1, 1}, 2);
    run_scenario("combined_rotation", {3, -4, 5}, {0.4f, -0.8f, 1.2f}, {1, 1, 1}, 3);
    run_scenario("wide_scale", {0, 0, 0}, {0, 0, 0}, {10, 1, 1}, 0);
    run_scenario("tall_scale", {0, 0, 0}, {0, 0, 0}, {1, 9, 1}, 3);
    run_scenario("small_scale", {2, 3, 4}, {0.1f, 0.2f, 0.3f}, {0.125f, 0.25f, 1}, 0);
    run_scenario("negative_x_scale", {1, -2, 3}, {0.3f, 0.6f, -0.9f}, {-2, 1, 1}, 1);
    run_scenario("negative_y_scale", {-2, 1, -3}, {-0.3f, 0.6f, 0.9f}, {1, -2, 1}, 2);
    run_scenario("zero_x_scale", {0, 0, 0}, {0, 0, 0}, {0, 1, 1}, 0);
    run_scenario("zero_y_scale", {0, 0, 0}, {0, 0, 0}, {1, 0, 1}, 3);
    run_scenario("large_coordinates", {10000, -20000, 30000}, {1.5f, -2.0f, 0.5f}, {32, 64, 1}, 0);
    run_scenario("mixed_flags_and_transform", {-13, 17, -19}, {-1.2f, 2.4f, -0.6f}, {3.5f, -4.5f, 1}, 0xFFFFFFFFu);
    std::printf("host occ_set_frustum: all scenarios passed\n");
    return 0;
}
