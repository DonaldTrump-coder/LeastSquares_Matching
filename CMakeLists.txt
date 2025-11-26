cmake_minimum_required(VERSION 3.12)
project(LEASTSQUARES_MATCHING)

# ----------------- 编译选项 -----------------
set(CMAKE_CXX_STANDARD 14)

if(MINGW)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWIN32 -D_WINDOWS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DWIN32 -D_WINDOWS")
endif()

if(WIN32)
    set(PARALLEL_JOBS "")
else()
    set(PARALLEL_JOBS "-- -j8")   # Linux 使用 8 线程
endif()

# ----------------- OpenCV 安装目录 -----------------
# 在项目内部安装 OpenCV
set(OPENCV_INSTALL_DIR "${CMAKE_BINARY_DIR}/opencv_install")
set(OpenCV_DIR "${OPENCV_INSTALL_DIR}")

# ----------------- 检测 OpenCV 是否已经安装 -----------------
if(NOT EXISTS "${OPENCV_INSTALL_DIR}/OpenCVConfig.cmake")
    message(STATUS "OpenCV not found, building from source...")

    # 设置 OpenCV 构建目录
    set(OPENCV_BUILD_DIR "${CMAKE_BINARY_DIR}/opencv_build")

    # 添加 OpenCV 源码子目录并编译（只在第一次执行）
    execute_process(
    COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/third_party/src/opencv
            -B ${OPENCV_BUILD_DIR}
            -G "MinGW Makefiles"
            -D CMAKE_BUILD_TYPE=Release
            -D CMAKE_INSTALL_PREFIX=${OPENCV_INSTALL_DIR}
            -D BUILD_EXAMPLES=OFF
            -D BUILD_TESTS=OFF
            -D BUILD_DOCS=OFF
            -D BUILD_opencv_videoio=OFF
            -D WITH_FFMPEG=OFF
    RESULT_VARIABLE ret
)
    if(NOT ret EQUAL 0)
        message(FATAL_ERROR "Failed to configure OpenCV")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${OPENCV_BUILD_DIR} --target install ${PARALLEL_JOBS}
        RESULT_VARIABLE ret
    )
    if(NOT ret EQUAL 0)
        message(FATAL_ERROR "Failed to build OpenCV")
    endif()
endif()

# ----------------- 查找 OpenCV -----------------
find_package(OpenCV REQUIRED)

# ----------------- 创建可执行文件 -----------------
add_executable(leastsquares_matching
    core/src/matching.cpp
    core/src/test.cpp
)

# ----------------- 链接 OpenCV -----------------
target_link_libraries(leastsquares_matching PRIVATE ${OpenCV_LIBS})

# ----------------- 项目头文件 -----------------
target_include_directories(leastsquares_matching PRIVATE
    ${CMAKE_SOURCE_DIR}/core/include
)