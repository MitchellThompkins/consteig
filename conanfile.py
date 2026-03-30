from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy
import os


class ConsteigConan(ConanFile):
    name = "consteig"
    description = (
        "Header-only C++17 constexpr library for compile-time "
        "eigenvalue and eigenvector computation"
    )
    license = "Apache-2.0"
    url = "https://github.com/MitchellThompkins/consteig"
    homepage = "https://github.com/MitchellThompkins/consteig"
    topics = (
        "constexpr",
        "eigenvalues",
        "eigenvectors",
        "linear-algebra",
        "header-only",
        "freestanding",
        "compile-time",
    )
    package_type = "header-library"
    no_copy_source = True

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CONSTEIG_VERSION"] = self.version
        tc.variables["CONSTEIG_BUILD_TESTS"] = False
        tc.variables["CONSTEIG_BUILD_EXAMPLES"] = False
        tc.variables["CONSTEIG_BUILD_PROFILING"] = False
        tc.generate()

    def package(self):
        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )
        cmake = CMake(self)
        cmake.configure()
        cmake.install()

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.set_property("cmake_file_name", "consteig")
        self.cpp_info.set_property("cmake_target_name", "consteig::consteig")
