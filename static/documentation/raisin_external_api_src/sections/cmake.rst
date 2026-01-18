CMake Integration
=================

CMake 기반 프로젝트에서 raisin_sdk를 사용하는 방법입니다.

CMakeLists.txt 예시
-------------------

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.10)
    project(my_robot_controller)

    set(CMAKE_CXX_STANDARD 17)

    # raisin_sdk_install 경로 설정
    set(RAISIN_SDK_PATH "${CMAKE_SOURCE_DIR}/../raisin_sdk_install")

    # Include 경로
    include_directories(
        ${RAISIN_SDK_PATH}/include
    )

    # 라이브러리 경로
    link_directories(
        ${RAISIN_SDK_PATH}/lib
    )

    # 의존성 패키지 (시스템)
    find_package(Eigen3 REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(PCL REQUIRED)
    find_package(OpenCV REQUIRED)

    # 실행 파일 생성
    add_executable(my_controller src/main.cpp)

    target_link_libraries(my_controller
        # raisin 라이브러리
        raisin_network
        raisin_interfaces
        nav_msgs
        sensor_msgs
        geometry_msgs
        builtin_interfaces
        std_msgs
        # 시스템 라이브러리
        Eigen3::Eigen
        OpenSSL::SSL
        OpenSSL::Crypto
        ${PCL_LIBRARIES}
        ${OpenCV_LIBS}
        pthread
    )

    # PCL include 경로
    target_include_directories(my_controller PRIVATE ${PCL_INCLUDE_DIRS})
    target_link_directories(my_controller PRIVATE ${PCL_LIBRARY_DIRS})
    target_compile_definitions(my_controller PRIVATE ${PCL_DEFINITIONS})

빌드 방법
---------

.. code-block:: bash

    mkdir build && cd build
    cmake ..
    make -j$(nproc)

RPATH 설정 (선택)
-----------------

실행 파일이 라이브러리를 찾지 못하는 경우:

**방법 1: LD_LIBRARY_PATH 설정**

.. code-block:: bash

    export LD_LIBRARY_PATH=/path/to/raisin_sdk_install/lib:$LD_LIBRARY_PATH
    ./my_controller

**방법 2: CMake RPATH 설정**

.. code-block:: cmake

    # CMakeLists.txt에 추가
    set(CMAKE_INSTALL_RPATH "${RAISIN_SDK_PATH}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

**방법 3: 시스템 라이브러리 경로 추가**

.. code-block:: bash

    sudo echo "/path/to/raisin_sdk_install/lib" > /etc/ld.so.conf.d/raisin.conf
    sudo ldconfig

예제 프로젝트 구조
------------------

.. code-block:: text

    my_project/
    ├── CMakeLists.txt
    ├── src/
    │   └── main.cpp
    ├── include/
    │   └── my_controller.hpp
    └── ../raisin_sdk_install/      # SDK 설치 디렉토리
        ├── include/
        └── lib/

.. note::
    raisin_sdk_install 폴더의 위치는 프로젝트 구조에 맞게 조정하세요.
    ``RAISIN_SDK_PATH`` 변수를 절대 경로로 설정해도 됩니다.
