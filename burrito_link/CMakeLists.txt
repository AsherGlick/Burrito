cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)

project (burrito_link)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# remove `-rdynamic` default link library flag set in the linux `Platform/*.cmake` file
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

# Set the compiler to mingw so we can build a windows executable on linux
SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

# Create the standalone executable
set(STANDALONE burrito_link.exe)
add_executable (${STANDALONE} "burrito_link.c")
target_link_libraries(${STANDALONE} PRIVATE "ws2_32")

target_compile_options(${STANDALONE} PRIVATE -Wall -Os -g)

# An option specifically for creating console application and not a windowed application
target_compile_options(${STANDALONE} PRIVATE -mconsole)


# Create the D3D11.dll file
set(CMAKE_SYSTEM_NAME Windows)

SET(DX11LIB d3d11.dll)

# Will this build a library
add_library(${DX11LIB} SHARED dllmain.c burrito_link.c)

target_compile_options(${DX11LIB} PRIVATE -mwindows)
target_compile_options(${DX11LIB} PRIVATE -static-libgcc -static-libstdc++)

target_link_libraries(${DX11LIB} PRIVATE "ws2_32")

set_target_properties(${DX11LIB} PROPERTIES
    PREFIX ""
    SUFFIX ""
    LINK_FLAGS "../deffile.def -Wl,--allow-shlib-undefined,--no-insert-timestamp -Wl,-O1 -shared -static -static-libgcc -static-libstdc++ -Wl,--file-alignment=4096 -lm -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32"
)
