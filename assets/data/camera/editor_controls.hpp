#include <stdint.h>
#include <core/event/key_codes.hpp>
// #include <glm/glm.hpp>
#define GLM_EXPERIMENTATION
// #include <glm/ext.h>
// #include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

// Whatever controls works for me (within reason)
struct editor_controls {
    uint32_t down = key_left_shift; // q
    uint32_t up = key_space; // e
    uint32_t left = key_a;
    uint32_t right = key_d;
    uint32_t forward = key_w;
    uint32_t backward = key_s;
    glm::vec2 last_cursor_pos = glm::zero<glm::vec2>();
    bool on_click_check = false;
    uint32_t player = 0;
};