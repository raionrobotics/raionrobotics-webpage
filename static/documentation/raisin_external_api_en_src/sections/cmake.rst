CMake Integration
=================

How to use raisin_sdk in CMake-based projects.

CMakeLists.txt Example
----------------------

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.10)
    project(my_robot_controller)

    set(CMAKE_CXX_STANDARD 17)

    # Set raisin_sdk_install path
    set(RAISIN_SDK_PATH "${CMAKE_SOURCE_DIR}/../raisin_sdk_install")

    # Include paths
    include_directories(
        ${RAISIN_SDK_PATH}/include
    )

    # Library paths
    link_directories(
        ${RAISIN_SDK_PATH}/lib
    )

    # System dependencies
    find_package(Eigen3 REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(PCL REQUIRED)

    # Create executable
    add_executable(my_controller src/main.cpp)

    target_link_libraries(my_controller
        # raisin libraries
        raisin_network
        raisin_interfaces
        nav_msgs
        sensor_msgs
        geometry_msgs
        builtin_interfaces
        std_msgs
        # System libraries
        Eigen3::Eigen
        OpenSSL::SSL
        OpenSSL::Crypto
        ${PCL_LIBRARIES}
        pthread
    )

    # PCL include paths
    target_include_directories(my_controller PRIVATE ${PCL_INCLUDE_DIRS})
    target_link_directories(my_controller PRIVATE ${PCL_LIBRARY_DIRS})
    target_compile_definitions(my_controller PRIVATE ${PCL_DEFINITIONS})

Build Instructions
------------------

.. code-block:: bash

    mkdir build && cd build
    cmake ..
    make -j$(nproc)

RPATH Configuration (Optional)
------------------------------

If the executable cannot find libraries at runtime:

**Method 1: Set LD_LIBRARY_PATH**

.. code-block:: bash

    export LD_LIBRARY_PATH=/path/to/raisin_sdk_install/lib:$LD_LIBRARY_PATH
    ./my_controller

**Method 2: CMake RPATH Setting**

.. code-block:: cmake

    # Add to CMakeLists.txt
    set(CMAKE_INSTALL_RPATH "${RAISIN_SDK_PATH}/lib")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

**Method 3: Add to System Library Path**

.. code-block:: bash

    sudo echo "/path/to/raisin_sdk_install/lib" > /etc/ld.so.conf.d/raisin.conf
    sudo ldconfig

Example Project Structure
-------------------------

.. code-block:: text

    my_project/
    ├── CMakeLists.txt
    ├── src/
    │   └── main.cpp
    ├── include/
    │   └── my_controller.hpp
    └── ../raisin_sdk_install/      # SDK installation directory
        ├── include/
        └── lib/

.. note::
    Adjust the raisin_sdk_install folder location to match your project structure.
    You can also set ``RAISIN_SDK_PATH`` to an absolute path.
