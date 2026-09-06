cmake_minimum_required(VERSION 3.20)

project(
    minitensor
    VERSION 0.1.0
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)#不允许降低版本
set(CMAKE_CXX_EXTENSIONS OFF)#不允许GNU++


function(enable_strict_warnings target)
    if (MSVC)
        # Windows msvc: /W4最高告警等级，/WX 告警=编译错误
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        # Linux/macOS(gcc/g++)
        target_compile_options(${target} PRIVATE
            -Wall        # 基础告警
            -Wextra      # 额外扩展告警
            -Wpedantic   # 严格遵守C++标准，拒绝非标准扩展
            -Werror      # ⭐重点：所有警告直接升级为编译错误，不允许带警告编译通过
        )
    endif()
endfunction()

option(MINITENSOR_ENABLE_SANITIZERS
    "Enable ASan and UBSan"
    OFF)

function(enable_sanitizers target)
    if (MINITENSOR_ENABLE_SANITIZERS
        AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined#野指针/未定义
            -fno-omit-frame-pointer#保留栈帧，**崩溃时打印完整调用栈**，方便定位源码
            -g#保留调试信息,**保存代码文件名和行号**
            
        )

        target_link_options(${target} PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endfunction()

enable_sanitizers(minitensor)
enable_sanitizers(test_demo_tensor)
enable_sanitizers(test_bound_tensor)
enable_sanitizers(test_storage)
enable_sanitizers(test_tensor_view)
enable_sanitizers(test_operators)


file(GLOB SRC_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(minitensor STATIC ${SRC_LIST})
target_include_directories(minitensor PUBLIC ${PROJECT_SOURCE_DIR}/include)
set_target_properties(minitensor PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)
enable_strict_warnings(minitensor)


add_executable(minitensor_demo
    ./examples/demo.cpp
)
set_target_properties(minitensor_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(minitensor_demo PRIVATE minitensor)
enable_strict_warnings(minitensor_demo)


add_executable(storage_demo
    ./examples/storage_move_demo.cpp
)
set_target_properties(storage_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(storage_demo PRIVATE minitensor)
enable_strict_warnings(storage_demo)


add_executable(matmul_bench
    ./benchmarks/matmul_bench.cpp
)
set_target_properties(matmul_bench PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(matmul_bench PRIVATE minitensor)
enable_strict_warnings(matmul_bench)


include (CTest) #定义BUILD_TESTING,开启enable_testing

if (BUILD_TESTING)
    include (FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )

    FetchContent_MakeAvailable(googletest)

    add_executable(minitensor_test
        tests/test_tensor.cpp
    )
    set_target_properties(minitensor_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    target_link_libraries(minitensor_test
        PRIVATE
            minitensor
            GTest::gtest_main
    )
    enable_strict_warnings(minitensor_test)

    add_executable(test_storage tests/test_storage.cpp)
    set_target_properties(test_storage PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    target_link_libraries(test_storage
        PRIVATE
            minitensor
            GTest::gtest_main
    )
    enable_strict_warnings(test_storage)

    add_executable(minitensor_test_view
        tests/test_tensor_view.cpp
    )
    set_target_properties(minitensor_test_view PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    target_link_libraries(minitensor_test_view
        PRIVATE
            minitensor
            GTest::gtest_main
    )
    enable_strict_warnings(minitensor_test_view)

    add_executable(operators_test_view
        tests/test_operators.cpp
    )
    set_target_properties(operators_test_view PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    target_link_libraries(operators_test_view
        PRIVATE
            minitensor
            GTest::gtest_main
    )
    enable_strict_warnings(operators_test_view)

    include(GoogleTest)
    gtest_discover_tests(minitensor_test)
    gtest_discover_tests(test_storage)
    gtest_discover_tests(minitensor_test_view)
    gtest_discover_tests(operators_test_view)
endif()