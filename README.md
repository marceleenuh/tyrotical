<div align="center">
  <img src="readme/title_img.png" alt="readme/title_img.png" height="250">
  <h1></h1>
</div>
<div align="center">

    Tyrotical
    My extremely barebones Vulkan rasterized renderer

</div>
<div align="center">

    How to Build Tyrotical
</div>

1. Configure CMake in ./build and set build type to Release. Also set VULKAN_PATH to whereever your Vulkan SDK is installed." `$ cmake -B build -DVULKAN_PATH="C:/VulkanSDK/1.3.211.0"`
2. Build `$ cmake --build build --config Release`
3. For MSVC the built binary will be in `./build/Release/tyrotical.exe`