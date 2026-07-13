#include <cstdio>
#include <cstdlib>

struct Vec3 {
    float x;
    float y;
    float z;
};

struct Plane {
    Vec3 normal;
    float distance;

    float point_distance(const Vec3& point) const {
        return point.x * normal.x + point.y * normal.y + point.z * normal.z + distance;
    }
};

struct Result {
    bool in_first_list;
    float depth;
    bool reverse_planes;
};

static Result retail_func(const Plane planes[5], const Vec3 corners[4], const Vec3& direction,
                          const Vec3& camera, const Vec3& transformed_position) {
    for (int i = 0; i < 5; ++i) {
        bool all_negative = true;
        for (int j = 0; j < 4; ++j) {
            if (planes[i].point_distance(corners[j]) >= 0.0f) {
                all_negative = false;
                break;
            }
        }
        if (all_negative) {
            return {true, 0.0f, false};
        }
    }

    const Vec3 delta = {camera.x - corners[0].x, camera.y - corners[0].y, camera.z - corners[0].z};
    const float dot = direction.x * delta.x + direction.y * delta.y + direction.z * delta.z;
    return {false, -transformed_position.z, dot >= 0.0f};
}

static Result decomp_func(const Plane planes[5], const Vec3 corners[4], const Vec3& direction,
                          const Vec3& camera, const Vec3& transformed_position) {
    for (int i = 0; i < 5; ++i) {
        bool all_negative = true;
        for (int j = 0; j < 4; ++j) {
            if (planes[i].point_distance(corners[j]) >= 0.0f) {
                all_negative = false;
                break;
            }
        }
        if (all_negative) {
            return {true, 0.0f, false};
        }
    }

    const Vec3 delta = {camera.x - corners[0].x, camera.y - corners[0].y, camera.z - corners[0].z};
    const float dot = direction.x * delta.x + direction.y * delta.y + direction.z * delta.z;
    return {false, -transformed_position.z, dot >= 0.0f};
}

static void run_scenario(const char* name, const Plane planes[5], const Vec3 corners[4],
                         const Vec3& direction, const Vec3& camera, const Vec3& transformed_position) {
    const Result retail = retail_func(planes, corners, direction, camera, transformed_position);
    const Result decomp = decomp_func(planes, corners, direction, camera, transformed_position);
    if (retail.in_first_list != decomp.in_first_list || retail.depth != decomp.depth ||
        retail.reverse_planes != decomp.reverse_planes) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    const Vec3 corners[4] = {{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
    Plane visible[5] = {{{1, 0, 0}, 2}, {{-1, 0, 0}, 2}, {{0, 1, 0}, 2}, {{0, -1, 0}, 2}, {{0, 0, 1}, 1}};
    Plane culled[5] = {{{1, 0, 0}, -2}, {{-1, 0, 0}, 2}, {{0, 1, 0}, 2}, {{0, -1, 0}, 2}, {{0, 0, 1}, 1}};
    const Vec3 forward = {0, 0, 1};
    const Vec3 backward = {0, 0, -1};
    const Vec3 camera_front = {0, 0, 2};
    const Vec3 camera_back = {0, 0, -2};

    run_scenario("plane0_early_out", culled, corners, forward, camera_front, {0, 0, 3});
    run_scenario("front_direction", visible, corners, forward, camera_front, {0, 0, 3});
    run_scenario("back_direction", visible, corners, forward, camera_back, {0, 0, -3});
    run_scenario("negative_depth", visible, corners, backward, camera_front, {0, 0, -4});
    run_scenario("zero_dot", visible, corners, forward, {0, 0, 0}, {0, 0, 0});
    run_scenario("x_direction", visible, corners, {1, 0, 0}, {2, 0, 0}, {0, 0, 5});
    run_scenario("y_direction", visible, corners, {0, 1, 0}, {0, -2, 0}, {0, 0, 6});
    run_scenario("last_plane_visible", visible, corners, backward, camera_front, {0, 0, 7});
}
