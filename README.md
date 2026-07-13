# GLPI Framework Intro — Cross-Platform CI

A Model + Renderer architecture rendering a rotating Triangle and a static
Square, running on Desktop (Windows/GLFW), Android, and Web (Emscripten/WebGL).

## Project Structure

- `Scene/` — shared source, used by all three platforms
  - `Model.h` — abstract lifecycle interface (InitModel/Render/Resize)
  - `Renderer.h/.cpp` — singleton controller owning the Model collection
  - `ShaderHelper.h/.cpp` — shader compile/link helper (inline-string and file-based loading)
  - `Triangle.h/.cpp`, `Square.h/.cpp` — concrete Model implementations
  - `main.cpp` — entry point for Desktop and Web
- `android/` — Android Studio / Gradle project (uses `NativeTemplate.cpp` as its JNI entry point instead of `main.cpp`)
- `CMakeLists.txt` — Desktop build configuration
- `build_web.bat` / `build_web.sh` — Web (Emscripten) build scripts
- `script_build_and_run.bat` — convenience wrapper for the Desktop build

## Building

### Desktop (Windows)

## Screenshots

   See `screenshots/` for Desktop, Android, and Web builds each showing
   both the Triangle and Square rendering together.