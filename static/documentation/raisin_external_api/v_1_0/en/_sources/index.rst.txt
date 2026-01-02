Raisin External API Reference
==============================

API documentation for controlling Raibo2 autonomous navigation from external systems.

`한국어 버전 <../ko/index.html>`_

Overview
--------

Raisin provides two integration methods for external systems:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - Method
     - Description
     - Use Case
   * - **API Example**
     - ``AutonomyClient`` wrapper class example code
     - Quick start, reference implementation
   * - **Direct Network**
     - Direct use of ``raisin_network`` library
     - Custom service calls, fine-grained control

.. note::
    **API Example (AutonomyClient)** is example code, not an officially supported SDK.
    It is a reference implementation showing how to integrate with the Raisin platform.
    Modify it as needed for your use case.

.. code-block:: text

    +------------------------------------------+
    |           External Application           |
    +------------------------------------------+
    |  Option A: API Example                   |
    |  +------------------------------------+  |
    |  |  #include "autonomy_client.hpp"   |  |
    |  |  AutonomyClient client;           |  |
    |  |  client.connect("ROBOT_ID");      |  |
    |  |  client.setMap(...);              |  |
    |  |  client.setWaypoints(...);        |  |
    |  +------------------------------------+  |
    +------------------------------------------+
    |  Option B: Direct Network                |
    |  +------------------------------------+  |
    |  |  raisin::Network network;         |  |
    |  |  raisin::Node node;               |  |
    |  |  auto client = node.createClient  |  |
    |  |      <SetWaypoints>(...);         |  |
    |  +------------------------------------+  |
    +------------------------------------------+
                      |
                      | WebSocket
                      v
    +------------------------------------------+
    |                 Raibo2                   |
    |  +----------------+  +----------------+  |
    |  | Autonomy Plugin|  | Fast-LIO Plugin|  |
    |  | - Waypoints    |  | - Localization |  |
    |  | - Navigation   |  | - Map Loading  |  |
    |  +----------------+  +----------------+  |
    +------------------------------------------+

Integration Package
-------------------

How to Obtain
^^^^^^^^^^^^^

The integration package is provided by Raion Robotics. Request via:

- **Email**: support@raionrobotics.com
- **Website**: https://raionrobotics.com/support

Package Contents
^^^^^^^^^^^^^^^^

The ``raisin_sdk_install/`` folder contains:

.. code-block:: text

    raisin_sdk_install/
    ├── include/                    # Header files
    │   ├── raisin_network/         # Network library
    │   ├── raisin_interfaces/      # Message/service definitions
    │   └── ...
    └── lib/                        # Compiled libraries
        ├── libraisin_network.so
        ├── libraisin_interfaces.so
        └── ...

System Requirements
^^^^^^^^^^^^^^^^^^^

.. code-block:: bash

    # Ubuntu 22.04
    sudo apt-get install libeigen3-dev libssl-dev libpcl-dev libopencv-dev

Network Requirements
^^^^^^^^^^^^^^^^^^^^

- Robot and client must be on the **same network**
- Multicast traffic must be allowed (for Robot ID discovery)
- Firewall: Allow TCP/UDP ports 8080, 38371, etc.

Finding Robot ID
^^^^^^^^^^^^^^^^

Robot ID is printed in the ``raisin_master`` console on startup:

.. code-block:: text

    [raisin_master] Robot ID: railab_raibo-3614732983840
    [raisin_master] Listening on port 38371

It can also be found in raisin_gui.

----

API Example Reference
---------------------

Example code using the ``AutonomyClient`` wrapper class. Modify as needed for your use case.

Include Header
^^^^^^^^^^^^^^

.. code-block:: cpp

    #include "raisin_sdk/autonomy_client.hpp"

Basic Workflow
^^^^^^^^^^^^^^

.. code-block:: cpp

    // 1. Create client
    raisin_sdk::AutonomyClient client("my_app");

    // 2. Connect to robot
    client.connect("ROBOT_ID");

    // 3. Load map (enables localization)
    client.setMap("/path/to/map.pcd", 0.0, 0.0, 0.0, 0.0, "my_map");

    // 4. Set waypoints (frame MUST match map_name!)
    std::vector<raisin_sdk::Waypoint> waypoints = {
        raisin_sdk::Waypoint("my_map", 5.0, 0.0),   // Use "my_map"
        raisin_sdk::Waypoint("my_map", 5.0, 5.0),
    };
    client.setWaypoints(waypoints, 1);

    // 5. Monitor status
    auto status = client.getMissionStatus();

Data Types
^^^^^^^^^^

**Waypoint** - Navigation target point

.. code-block:: cpp

    struct Waypoint {
        std::string frame;  // Coordinate frame (important!)
        double x, y, z;     // Coordinates
        bool use_z;         // Whether to use Z coordinate
    };

    // Creation methods
    Waypoint("map_name", x, y);           // Map coordinates
    Waypoint::GPS(latitude, longitude);   // GPS coordinates (requires GPS module)

.. warning::
    **The frame name MUST match the map_name specified in setMap()!**

    .. code-block:: cpp

        // Correct example
        client.setMap("/path/map.pcd", 0, 0, 0, 0, "office_map");
        Waypoint("office_map", 5.0, 0.0);  // Same name

        // Wrong example
        client.setMap("/path/map.pcd", 0, 0, 0, 0, "office_map");
        Waypoint("map", 5.0, 0.0);  // Name mismatch - won't work!

**MissionStatus** - Mission state

.. code-block:: cpp

    struct MissionStatus {
        std::vector<Waypoint> waypoints;  // Current waypoint list
        uint8_t current_index;            // Current target index
        uint8_t repetition;               // Remaining laps (0=infinite)
        bool valid;                       // Data validity
    };

**ServiceResult** - Service call result

.. code-block:: cpp

    struct ServiceResult {
        bool success;         // Success flag
        std::string message;  // Result message
    };

**RobotState** - Robot state (Odometry)

.. code-block:: cpp

    struct RobotState {
        double x, y, z;    // Position (meters)
        double yaw;        // Heading (radians)
        double vx, vy;     // Linear velocity (m/s)
        double omega;      // Angular velocity (rad/s)
        bool valid;
    };

**Point3D** - Point cloud point

.. code-block:: cpp

    struct Point3D { float x, y, z; };

AutonomyClient Methods
^^^^^^^^^^^^^^^^^^^^^^

**connect()**

.. code-block:: cpp

    bool connect(const std::string& robot_id, int timeout_sec = 10);

Connects to the robot.

- ``robot_id``: Robot ID or IP address
- ``timeout_sec``: Connection timeout (seconds)
- **Returns**: Connection success

**setMap()**

.. code-block:: cpp

    ServiceResult setMap(const std::string& pcd_path,
                         double initial_x, double initial_y,
                         double initial_z, double initial_yaw,
                         const std::string& map_name);

Loads a PCD map file and initializes localization.

- ``pcd_path``: PCD file path (**file on client PC**, transferred to robot)
- ``initial_x/y/z``: Robot initial position in map (enter map coordinates where the robot is physically located)
- ``initial_yaw``: Initial heading (radians)
- ``map_name``: Map frame name (**must match Waypoint frame**)

.. note::
    PCD maps must be pre-generated using SLAM. Use raisin_gui's Mapping feature or
    generate directly with the Fast-LIO plugin.

**setWaypoints()**

.. code-block:: cpp

    ServiceResult setWaypoints(const std::vector<Waypoint>& waypoints,
                                uint8_t repetition = 1,
                                uint8_t start_index = 0);

Sets waypoint list and starts navigation.

- ``waypoints``: Waypoint list
- ``repetition``: Repeat count

  - ``0`` = infinite patrol
  - ``1`` = single pass (default)
  - ``N`` = repeat N times

- ``start_index``: Starting waypoint index

**getMissionStatus()**

.. code-block:: cpp

    MissionStatus getMissionStatus();

Queries current mission status.

**appendWaypoint()**

.. code-block:: cpp

    ServiceResult appendWaypoint(const Waypoint& waypoint);

Appends a waypoint to the current mission queue.

**stopNavigation()**

.. code-block:: cpp

    ServiceResult stopNavigation();

Stops autonomous navigation (sets empty waypoint list).

**startPatrol()**

.. code-block:: cpp

    ServiceResult startPatrol(const std::vector<Waypoint>& waypoints);

Starts infinite patrol (equivalent to ``setWaypoints(waypoints, 0)``).

**subscribeOdometry()**

.. code-block:: cpp

    void subscribeOdometry(std::function<void(const RobotState&)> callback);

Subscribes to real-time odometry data.

**subscribePointCloud()**

.. code-block:: cpp

    void subscribePointCloud(std::function<void(const std::vector<Point3D>&)> callback);

Subscribes to real-time LiDAR point cloud.

**getRobotState() / getLatestPointCloud()**

.. code-block:: cpp

    RobotState getRobotState();
    std::vector<Point3D> getLatestPointCloud();

Returns last received data (thread-safe).

**loadPCD() (static)**

.. code-block:: cpp

    static std::vector<Point3D> loadPCD(const std::string& pcd_path);

Loads a PCD file (for visualization without sending to robot).

GPS Usage Notes
^^^^^^^^^^^^^^^

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS module must be installed** on the robot
- GPS fix must be valid (``/fix`` topic)
- Without GPS, ``setWaypoints()`` succeeds but robot won't reach target

----

Direct Network API
------------------

Direct ``raisin_network`` usage for advanced users.

Include Headers
^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include "raisin_network/raisin.hpp"
    #include "raisin_network/network.hpp"
    #include "raisin_network/node.hpp"
    #include "raisin_interfaces/srv/set_waypoints.hpp"
    #include "raisin_interfaces/srv/get_waypoints.hpp"
    #include "raisin_interfaces/srv/append_waypoint.hpp"
    #include "raisin_interfaces/srv/set_laser_map.hpp"

Connection Pattern
^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // 1. Initialize
    raisin::raisinInit();

    // 2. Create network
    std::vector<std::vector<std::string>> threads = {{"main"}};
    auto network = std::make_shared<raisin::Network>(
        "my_client",      // Client ID
        "external",       // Device type
        threads
    );

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 3. Connect to robot
    auto connection = network->connect("ROBOT_ID");
    if (!connection) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 4. Create Node
    raisin::Node node(network);

Creating Service Clients
^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // SetWaypoints client
    auto setClient = node.createClient<raisin::raisin_interfaces::srv::SetWaypoints>(
        "planning/set_waypoints", connection);

    // GetWaypoints client
    auto getClient = node.createClient<raisin::raisin_interfaces::srv::GetWaypoints>(
        "planning/get_waypoints", connection);

    // AppendWaypoint client
    auto appendClient = node.createClient<raisin::raisin_interfaces::srv::AppendWaypoint>(
        "planning/append_waypoint", connection);

    // SetMap client
    auto mapClient = node.createClient<raisin::raisin_interfaces::srv::SetLaserMap>(
        "set_map", connection);

    // Wait for service
    if (!setClient->waitForService(std::chrono::seconds(10))) {
        std::cerr << "Service not available" << std::endl;
    }

Service Call Example
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // Create SetWaypoints request
    auto request = std::make_shared<raisin::raisin_interfaces::srv::SetWaypoints::Request>();

    raisin::raisin_interfaces::msg::Waypoint wp;
    wp.frame = "my_map";
    wp.x = 5.0;
    wp.y = 0.0;
    wp.z = 0.0;
    wp.use_z = false;
    request->waypoints.push_back(wp);

    request->repetition = 1;
    request->current_index = 0;

    // Async call
    auto future = setClient->asyncSendRequest(request);

    if (future.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future.get();
        std::cout << "Success: " << response->success << std::endl;
        std::cout << "Message: " << response->message << std::endl;
    }

Creating Subscribers
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // Odometry subscription
    auto odomSub = node.createSubscriber<raisin::nav_msgs::msg::Odometry>(
        "/Odometry", connection,
        [](const raisin::nav_msgs::msg::Odometry::SharedPtr& msg) {
            std::cout << "Position: " << msg->pose.pose.position.x
                      << ", " << msg->pose.pose.position.y << std::endl;
        });

    // PointCloud subscription
    auto cloudSub = node.createSubscriber<raisin::sensor_msgs::msg::PointCloud2>(
        "/cloud_registered", connection,
        [](const raisin::sensor_msgs::msg::PointCloud2::SharedPtr& msg) {
            std::cout << "Points: " << msg->width * msg->height << std::endl;
        });

----

Service Interface Reference
----------------------------

Available service interface definitions.

planning/set_waypoints
^^^^^^^^^^^^^^^^^^^^^^

Sets the waypoint list.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - waypoints
     - Waypoint[]
     - Waypoint array
   * - repetition
     - uint8
     - Repeat count (0=infinite)
   * - current_index
     - uint8
     - Starting index

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - Success flag
   * - message
     - string
     - Result message

**Possible Response Messages:**

- ``"set waypoint success"`` - Success
- ``"waypoint list is empty"`` - Empty waypoint list
- ``"service timeout"`` - No service response

planning/get_waypoints
^^^^^^^^^^^^^^^^^^^^^^

Queries current mission status.

**Request:** (empty)

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - Success flag
   * - message
     - string
     - Result message
   * - waypoints
     - Waypoint[]
     - Current waypoint list
   * - repetition
     - uint8
     - Remaining laps
   * - current_index
     - uint8
     - Current target index

planning/append_waypoint
^^^^^^^^^^^^^^^^^^^^^^^^

Appends a waypoint to the queue.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - waypoint
     - Waypoint
     - Waypoint to append

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - Success flag
   * - message
     - string
     - Result message

set_map (SetLaserMap)
^^^^^^^^^^^^^^^^^^^^^

Loads PCD map and initializes localization.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - Map frame name
   * - pc
     - PointCloud2
     - Point cloud data
   * - initial_pose
     - Pose
     - Initial robot pose

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - Success flag
   * - message
     - string
     - Result message

**Possible Response Messages:**

- ``"Map saved."`` - Success
- ``"service timeout"`` - Fast-LIO plugin not loaded

Waypoint Message
^^^^^^^^^^^^^^^^

.. code-block:: text

    string frame      # Coordinate frame: "map_name", "gps", "odom"
    float64 x         # X coordinate (GPS: latitude)
    float64 y         # Y coordinate (GPS: longitude)
    float64 z         # Z coordinate (GPS: altitude)
    bool use_z        # Whether to check Z coordinate

----

Complete Examples
-----------------

Basic Waypoint Control
^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <thread>
    #include <chrono>
    #include "raisin_sdk/autonomy_client.hpp"

    int main(int argc, char* argv[]) {
        if (argc < 2) {
            std::cout << "Usage: " << argv[0] << " <robot_id> [pcd_path]" << std::endl;
            return 1;
        }

        std::string robotId = argv[1];
        std::string pcdPath = (argc >= 3) ? argv[2] : "../maps/office1_example.pcd";

        // Create and connect client
        raisin_sdk::AutonomyClient client("waypoint_example");

        std::cout << "Connecting to " << robotId << "..." << std::endl;
        if (!client.connect(robotId)) {
            std::cerr << "Connection failed!" << std::endl;
            return 1;
        }
        std::cout << "Connected!" << std::endl;

        // Load map
        std::cout << "Loading map..." << std::endl;
        auto mapResult = client.setMap(pcdPath, 0.0, 0.0, 0.0, 0.0, "sdk_map");
        if (!mapResult.success) {
            std::cerr << "Map load failed: " << mapResult.message << std::endl;
            return 1;
        }
        std::cout << "Map loaded: " << mapResult.message << std::endl;

        // Set waypoints (frame MUST match map_name!)
        std::vector<raisin_sdk::Waypoint> waypoints = {
            raisin_sdk::Waypoint("sdk_map", 0.0, 0.0),
            raisin_sdk::Waypoint("sdk_map", 5.0, 0.0),
            raisin_sdk::Waypoint("sdk_map", 5.0, 5.0),
            raisin_sdk::Waypoint("sdk_map", 0.0, 5.0),
            raisin_sdk::Waypoint("sdk_map", 0.0, 0.0),
        };

        std::cout << "Setting " << waypoints.size() << " waypoints..." << std::endl;
        auto result = client.setWaypoints(waypoints, 1);
        if (!result.success) {
            std::cerr << "Failed: " << result.message << std::endl;
            return 1;
        }
        std::cout << "Navigation started!" << std::endl;

        // Monitor progress
        while (true) {
            auto status = client.getMissionStatus();
            if (status.valid) {
                std::cout << "\rProgress: waypoint " << (int)status.current_index
                          << "/" << status.waypoints.size()
                          << " | Laps: " << (int)status.repetition << "   " << std::flush;

                // Check mission complete
                if (status.current_index >= status.waypoints.size() - 1 &&
                    status.repetition == 0) {
                    std::cout << "\nMission complete!" << std::endl;
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        client.disconnect();
        return 0;
    }

Real-time Monitoring
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <atomic>
    #include <csignal>
    #include "raisin_sdk/autonomy_client.hpp"

    std::atomic<bool> running{true};
    void signalHandler(int) { running = false; }

    int main(int argc, char* argv[]) {
        std::signal(SIGINT, signalHandler);

        raisin_sdk::AutonomyClient client("monitor");
        if (!client.connect(argv[1])) return 1;

        // Subscribe to odometry
        client.subscribeOdometry([](const raisin_sdk::RobotState& state) {
            std::cout << "Position: (" << state.x << ", " << state.y << ") "
                      << "Yaw: " << state.yaw << " rad" << std::endl;
        });

        // Subscribe to point cloud
        client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& cloud) {
            std::cout << "LiDAR points: " << cloud.size() << std::endl;
        });

        std::cout << "Monitoring... (Ctrl+C to stop)" << std::endl;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return 0;
    }

Infinite Patrol
^^^^^^^^^^^^^^^

.. code-block:: cpp

    // Start infinite patrol
    std::vector<raisin_sdk::Waypoint> patrol = {
        raisin_sdk::Waypoint("sdk_map", 0.0, 0.0),
        raisin_sdk::Waypoint("sdk_map", 10.0, 0.0),
        raisin_sdk::Waypoint("sdk_map", 10.0, 10.0),
        raisin_sdk::Waypoint("sdk_map", 0.0, 10.0),
    };

    client.startPatrol(patrol);  // or client.setWaypoints(patrol, 0);

    // Stop patrol
    client.stopNavigation();

----

GUI Demo (simple_gui)
---------------------

An OpenCV-based visualization demo program. It visualizes maps and robot positions, allowing you to set waypoints with mouse clicks.

Usage
^^^^^

.. code-block:: bash

    ./simple_gui <robot_id> [pcd_path]

    # Examples
    ./simple_gui railab_raibo-3614732983840
    ./simple_gui railab_raibo-3614732983840 ../maps/office1_example.pcd

Workflow
^^^^^^^^

1. Press ``O`` to open a PCD map file
2. Click on map to set robot initial position
3. Click in the direction the robot is facing to set yaw
4. Press ``S`` to send map and start localization
5. Click on map to add waypoints
6. Press ``W`` to send waypoints and start navigation

Keyboard Shortcuts
^^^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Key
     - Function
   * - ``O``
     - Open PCD map file
   * - ``S``
     - Send map (start localization)
   * - ``W``
     - Send waypoints (start navigation)
   * - ``C``
     - Clear waypoints
   * - ``R``
     - Toggle repetition (1 → 5 → ∞)
   * - ``X``
     - Reset initial pose
   * - ``+/-``
     - Zoom in/out
   * - ``Q``
     - Quit

Mouse Controls
^^^^^^^^^^^^^^

- **Left-click**: Step-by-step workflow action (position/direction/waypoint)
- **Right-drag**: Pan view
- **Scroll**: Zoom

Status Display
^^^^^^^^^^^^^^

- Current step and next action guide displayed at bottom of screen
- Waypoints cannot be added before localization is active
- Pressing ``S`` when localization is already active shows reset instructions

.. note::
    simple_gui uses the ``zenity`` package for file selection dialogs.
    Install with ``sudo apt-get install zenity`` on Ubuntu.

----

CMake Configuration
-------------------

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.16)
    project(my_controller)

    set(CMAKE_CXX_STANDARD 17)

    # SDK path
    set(RAISIN_SDK_PATH "/path/to/raisin_sdk_install")

    # Dependencies
    find_package(Eigen3 REQUIRED)
    find_package(OpenSSL REQUIRED)

    set(PCL_INCLUDE_DIRS "/usr/include/pcl-1.12")
    set(PCL_LIBRARIES pcl_common pcl_io)

    # Executable
    add_executable(my_controller main.cpp)

    target_include_directories(my_controller PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${RAISIN_SDK_PATH}/include
        ${PCL_INCLUDE_DIRS}
    )

    target_link_libraries(my_controller PRIVATE
        ${RAISIN_SDK_PATH}/lib/libraisin_network.so
        ${RAISIN_SDK_PATH}/lib/libraisin_thread_pool.so
        ${RAISIN_SDK_PATH}/lib/libraisin_parameter.so
        ${RAISIN_SDK_PATH}/lib/libraisin_encryption.so
        ${RAISIN_SDK_PATH}/lib/libraisin_data_logger.so
        ${RAISIN_SDK_PATH}/lib/libraisin_util.so
        ${RAISIN_SDK_PATH}/lib/libraisim.so
        ${RAISIN_SDK_PATH}/lib/libzstd.so
        Eigen3::Eigen
        OpenSSL::SSL OpenSSL::Crypto
        ${PCL_LIBRARIES}
        pthread dl rt
    )

    set_target_properties(my_controller PROPERTIES
        BUILD_RPATH "${RAISIN_SDK_PATH}/lib"
    )

----

Troubleshooting
---------------

Connection Failed
^^^^^^^^^^^^^^^^^

**Symptom**: ``connect()`` returns false

**Check:**

1. Verify ``raisin_master`` is running on robot
2. Verify same network connectivity
3. Verify correct Robot ID (see raisin_master console output)

Map Load Failed
^^^^^^^^^^^^^^^

**Symptom**: ``setMap()`` fails or times out

**Check:**

1. Verify **Fast-LIO plugin** is loaded (if not: see FastLIO section in raisin_plugin documentation)
2. Verify PCD file path is correct
3. Verify PCD file is not corrupted (test with ``loadPCD()``)

Robot Not Moving
^^^^^^^^^^^^^^^^

**Symptom**: ``setWaypoints()`` succeeds but robot stays still

**Check:**

1. Verify **Autonomy plugin** is loaded (if not: see Autonomy section in raisin_plugin documentation)
2. **Verify frame name matches map_name** (most common cause!)
3. Verify waypoint coordinates are within map bounds
4. Verify not in joystick override mode

.. code-block:: cpp

    // Common mistake
    client.setMap("...", 0, 0, 0, 0, "office_map");
    client.setWaypoints({Waypoint("map", 5.0, 0.0)}, 1);  // "map" != "office_map"

    // Correct usage
    client.setMap("...", 0, 0, 0, 0, "office_map");
    client.setWaypoints({Waypoint("office_map", 5.0, 0.0)}, 1);

Service Response Messages
^^^^^^^^^^^^^^^^^^^^^^^^^

**SetWaypoints:**

- ``"set waypoint success"`` - Success
- ``"waypoint list is empty"`` - Empty list

**SetMap (SetLaserMap):**

- ``"Map saved."`` - Success
- Timeout - Fast-LIO plugin not loaded

**GetWaypoints:**

- ``"get waypoint success"`` - Success

----

Related Documentation
---------------------

- **raisin_master documentation**: Robot setup and plugin loading
- **raisin_plugin documentation**: Autonomy, Fast-LIO plugin details

----

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
