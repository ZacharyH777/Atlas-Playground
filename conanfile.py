from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class GameDemo(ConanFile):
    name = "game-demo"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    export_source = "CMakeLists.txt", "application.cpp"

    # Putting all of your build-related dependencies here
    def build_requirements(self):
        self.tool_requires("make/4.4.1")
        self.tool_requires("cmake/3.27.1")
        self.tool_requires("engine3d-cmake-utils/3.0")

    # Putting all of your packages here
    def requirements(self):
        self.requires("glfw/3.4", transitive_headers=True)
        self.requires("spdlog/1.15.1", transitive_headers=True)
        self.requires("glm/1.0.1", transitive_headers=True)
        self.requires("yaml-cpp/0.8.0", transitive_headers=True)
        self.requires("box2d/2.4.1", transitive_headers=True)
        self.requires("opengl/system", transitive_headers=True)
        self.requires("imguidocking/2.0")

        # Vulkan-related headers and includes packages
        self.requires("vulkan-headers/1.3.290.0", transitive_headers=True)
        self.requires("flecs/4.0.0")
        self.requires("tinyobjloader/2.0.0-rc10")
        self.requires("stb/cci.20230920")

        self.requires("boost-ext-ut/2.1.0")
        self.requires("atlas/0.1@physics/testing")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def layout(self):
        cmake_layout(self)