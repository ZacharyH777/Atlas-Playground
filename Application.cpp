#include <atlas/core/application.hpp>
#include <atlas/renderer/renderer.hpp>
#include "editor_world.hpp"

class test_application : public atlas::application{
public:
    test_application(const atlas::application_settings& p_settings) 
    : application(p_settings) {
        m_world = atlas::create_ref<editor_world>("Editor World");
    }

private:
    atlas::ref<editor_world> m_world;
};


namespace atlas {
    ref<application> initialize_application() {
        application_settings settings = {
            .Name = "Editor",
            .Width = 1200,
            .Height = 800,
        };

        return create_ref<test_application>(settings);
    }

} 