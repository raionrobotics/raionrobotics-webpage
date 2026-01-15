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
     - ``RaisinClient`` wrapper class example code
     - Quick start, reference implementation
   * - **Direct Network**
     - Direct use of ``raisin_network`` library
     - Custom service calls, fine-grained control

.. note::
    **API Example (RaisinClient)** is example code, not an officially supported SDK.
    It is a reference implementation showing how to integrate with the Raisin platform.
    Modify it as needed for your use case.

.. code-block:: text

    +------------------------------------------+
    |           External Application           |
    +------------------------------------------+
    |  Option A: API Example                   |
    |  +------------------------------------+  |
    |  |  #include "raisin_client.hpp"   |  |
    |  |  RaisinClient client;           |  |
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

    [raisin_master] Robot ID: 10.42.0.1
    [raisin_master] Listening on port 38371

It can also be found in raisin_gui.

----

API Example Reference
---------------------

Example code using the ``RaisinClient`` wrapper class. Modify as needed for your use case.

Include Header
^^^^^^^^^^^^^^

.. code-block:: cpp

    #include "raisin_sdk/raisin_client.hpp"

Basic Workflow
^^^^^^^^^^^^^^

.. code-block:: cpp

    // 1. Create client
    raisin_sdk::RaisinClient client("my_app");

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

.. _data-types-en:

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

**ActuatorInfo** - Actuator (motor) information

.. code-block:: cpp

    struct ActuatorInfo {
        std::string name;           // Motor name (e.g., "FR_hip", "FL_thigh")
        uint16_t status;            // Status code (0 = normal)
        double temperature;         // Motor temperature (°C)
        double position;            // Joint position (rad)
        double velocity;            // Joint velocity (rad/s)
        double effort;              // Joint torque (Nm)
    };

**LocomotionState** - Robot locomotion state enum

.. code-block:: cpp

    enum class LocomotionState : int32_t {
        COMM_DISABLED = 0,      // Communication disabled
        COMM_ENABLED = 1,       // Communication enabled
        MOTOR_READY = 2,        // Motor ready
        MOTOR_COMMUTATION = 3,  // Motor commutation in progress
        MOTOR_ENABLED = 4,      // Motor enabled
        IN_TEST_MODE = 5,       // Test mode
        STANDING_MODE = 6,      // Standing
        IN_CONTROL = 7,         // In control (walking)
        SITDOWN_MODE = 8,       // Sitting
        MOTOR_DISABLED = 9      // Motor disabled
    };

**JoySourceType** - Joystick control source type

.. code-block:: cpp

    enum class JoySourceType : int32_t {
        JOY = 0,           // Manual joystick control
        VEL_CMD = 1,       // Autonomous velocity command
        NUM_SOURCES = 2    // No control source
    };

**ExtendedRobotState** - Extended robot state (includes battery, motor status)

.. code-block:: cpp

    struct ExtendedRobotState {
        // Position and velocity
        double x, y, z;             // Position (meters)
        double yaw;                 // Heading (radians)
        double vx, vy;              // Linear velocity (m/s)
        double omega;               // Angular velocity (rad/s)

        // Locomotion state
        int32_t locomotion_state;   // LocomotionState enum value (0-9)

        // Battery information
        double voltage;             // Current voltage (V)
        double current;             // Current (A)
        double max_voltage;         // Maximum voltage
        double min_voltage;         // Minimum voltage

        // Temperature
        double body_temperature;    // Body temperature (°C)

        // Joystick control state
        int32_t joy_listen_type;    // JoySourceType enum value

        // Actuator status
        std::vector<ActuatorInfo> actuators;

        bool valid;

        // Utility methods
        std::string getLocomotionStateName() const;  // State name string
        std::string getJoySourceName() const;        // Control source name string
        bool isOperational() const;                  // Whether standing or walking
        bool hasActuatorError() const;               // Whether any motor has error
    };

RaisinClient Methods
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

**subscribeRobotState()**

.. code-block:: cpp

    void subscribeRobotState(std::function<void(const ExtendedRobotState&)> callback);

Subscribes to extended robot state in real-time. Includes battery information, locomotion state, actuator status, etc.

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        std::cout << "Locomotion: " << state.getLocomotionStateName() << std::endl;
        std::cout << "Battery: " << state.voltage << "V" << std::endl;
        std::cout << "Control: " << state.getJoySourceName() << std::endl;

        if (state.hasActuatorError()) {
            std::cerr << "Warning: Actuator error detected!" << std::endl;
        }
    });

**getExtendedRobotState()**

.. code-block:: cpp

    ExtendedRobotState getExtendedRobotState();

Returns last received extended robot state (thread-safe).
Must call ``subscribeRobotState()`` first to get valid data.

**findGuiNetworkId()**

.. code-block:: cpp

    std::string findGuiNetworkId(const std::string& prefix = "gui");

Finds the connected GUI's network ID.

- ``prefix``: GUI ID prefix (default: "gui")
- **Returns**: GUI network ID (e.g., "gui53-230486654196"), empty string if not found

.. code-block:: cpp

    std::string guiId = client.findGuiNetworkId();
    std::cout << "Connected GUI: " << guiId << std::endl;

**setManualControl()**

.. code-block:: cpp

    ServiceResult setManualControl(const std::string& gui_network_id = "");

Enables manual joystick control (joy/gui).
Auto-detects the GUI network ID to receive joystick commands from that GUI.

- ``gui_network_id``: GUI network ID (auto-detected if empty, keeps existing ID if detection fails)
- **Returns**: Service call result

.. note::
    Even if GUI network ID auto-detection fails, the service call will succeed.
    In this case, the existing network_id configured on the robot is preserved.

.. code-block:: cpp

    auto result = client.setManualControl();
    if (result.success) {
        std::cout << "Manual control enabled" << std::endl;
    }

**setAutonomousControl()**

.. code-block:: cpp

    ServiceResult setAutonomousControl();

Enables autonomous control (vel_cmd/autonomy).

- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.setAutonomousControl();
    if (result.success) {
        std::cout << "Autonomous control enabled" << std::endl;
    }

**releaseControl()**

.. code-block:: cpp

    ServiceResult releaseControl(const std::string& source = "joy/gui");

Releases control (switches to None state).

- ``source``: Control source to release ("joy/gui" or "vel_cmd/autonomy")
- **Returns**: Service call result

.. code-block:: cpp

    client.releaseControl("joy/gui");
    client.releaseControl("vel_cmd/autonomy");

.. note::
    Control state can be checked via ``ExtendedRobotState.joy_listen_type``:

    - ``JOY (0)``: Manual joystick control active (joy/gui)
    - ``VEL_CMD (1)``: Receiving autonomous velocity commands (vel_cmd/autonomy)
    - ``NONE (2)``: No control source

    When ``setManualControl()`` is called, the GUI's wifi icon turns green.

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
    #include "raisin_interfaces/srv/string.hpp"       // For Joy control
    #include "raisin_interfaces/msg/robot_state.hpp"  // For robot state topic

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

    // Joy control client
    auto joyListenClient = node.createClient<raisin::raisin_interfaces::srv::String>(
        "set_listen", connection);

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

    // RobotState subscription (battery, locomotion state, actuators, etc.)
    auto stateSub = node.createSubscriber<raisin::raisin_interfaces::msg::RobotState>(
        "robot_state", connection,
        [](const raisin::raisin_interfaces::msg::RobotState::SharedPtr& msg) {
            std::cout << "Locomotion state: " << msg->state << std::endl;
            std::cout << "Battery: " << msg->voltage << "V / " << msg->current << "A" << std::endl;
            std::cout << "Joy source: " << msg->joy_listen_type << std::endl;
            std::cout << "Actuators: " << msg->actuator_states.size() << std::endl;
        });

Joy Control Service Call
^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // Enable manual joystick control
    auto enableRequest = std::make_shared<raisin::raisin_interfaces::srv::String::Request>();
    enableRequest->data = "joy<&>my_client_id";  // "topic_name<&>client_id"

    auto future = joyListenClient->asyncSendRequest(enableRequest);
    if (future.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future.get();
        std::cout << "Enable joy: " << response->success << std::endl;
    }

    // Disable joystick control (lock)
    auto disableRequest = std::make_shared<raisin::raisin_interfaces::srv::String::Request>();
    disableRequest->data = "joy<&><CLOSE>";  // "topic_name<&><CLOSE>"

    auto future2 = joyListenClient->asyncSendRequest(disableRequest);
    if (future2.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future2.get();
        std::cout << "Disable joy: " << response->success << std::endl;
    }

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

set_listen (Joy Control)
^^^^^^^^^^^^^^^^^^^^^^^^

Enables/disables manual joystick control.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - data
     - string
     - Control command string

**Request data format:**

- Enable: ``"topic_name<&>client_id"`` (e.g., ``"joy<&>my_app"``)
- Disable: ``"topic_name<&><CLOSE>"`` (e.g., ``"joy<&><CLOSE>"``)

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

robot_state (Topic)
^^^^^^^^^^^^^^^^^^^

Topic that publishes robot state information.

**Topic name:** ``robot_state``

**Message fields:**

.. list-table::
   :header-rows: 1
   :widths: 25 20 55

   * - Field
     - Type
     - Description
   * - actuator_states
     - ActuatorState[]
     - Actuator (motor) status array
   * - base_pos
     - double[3]
     - Base position [x, y, z]
   * - base_quat
     - double[4]
     - Base quaternion [x, y, z, w]
   * - base_lin_vel
     - double[3]
     - Linear velocity [vx, vy, vz]
   * - base_ang_vel
     - double[3]
     - Angular velocity [wx, wy, wz]
   * - voltage
     - double
     - Current battery voltage (V)
   * - current
     - double
     - Current (A)
   * - max_voltage
     - double
     - Maximum voltage
   * - min_voltage
     - double
     - Minimum voltage
   * - body_temperature
     - double
     - Body temperature (°C)
   * - state
     - int32
     - Locomotion state (0-9)
   * - joy_listen_type
     - int32
     - Joy source type (0=JOY, 1=VEL_CMD, 2=NONE)

**ActuatorState message:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - Motor name (e.g., "FR_hip")
   * - position
     - double
     - Joint position (rad)
   * - velocity
     - double
     - Joint velocity (rad/s)
   * - effort
     - double
     - Joint torque (Nm)
   * - status
     - uint16
     - Status code (0 = normal)
   * - temperature
     - double
     - Motor temperature (°C)

**Locomotion State values:**

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Value
     - State
   * - 0
     - COMM_DISABLED (Communication disabled)
   * - 1
     - COMM_ENABLED (Communication enabled)
   * - 2
     - MOTOR_READY (Motor ready)
   * - 3
     - MOTOR_COMMUTATION (Motor commutation in progress)
   * - 4
     - MOTOR_ENABLED (Motor enabled)
   * - 5
     - IN_TEST_MODE (Test mode)
   * - 6
     - STANDING_MODE (Standing)
   * - 7
     - IN_CONTROL (In control/walking)
   * - 8
     - SITDOWN_MODE (Sitting)
   * - 9
     - MOTOR_DISABLED (Motor disabled)

----

Complete Examples
-----------------

Basic Waypoint Control
^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <thread>
    #include <chrono>
    #include "raisin_sdk/raisin_client.hpp"

    int main(int argc, char* argv[]) {
        if (argc < 2) {
            std::cout << "Usage: " << argv[0] << " <robot_id> [pcd_path]" << std::endl;
            return 1;
        }

        std::string robotId = argv[1];
        std::string pcdPath = (argc >= 3) ? argv[2] : "../maps/office1_example.pcd";

        // Create and connect client
        raisin_sdk::RaisinClient client("waypoint_example");

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
    #include "raisin_sdk/raisin_client.hpp"

    std::atomic<bool> running{true};
    void signalHandler(int) { running = false; }

    int main(int argc, char* argv[]) {
        std::signal(SIGINT, signalHandler);

        raisin_sdk::RaisinClient client("monitor");
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

Robot State Monitoring
^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <atomic>
    #include <csignal>
    #include "raisin_sdk/raisin_client.hpp"

    std::atomic<bool> running{true};
    void signalHandler(int) { running = false; }

    int main(int argc, char* argv[]) {
        std::signal(SIGINT, signalHandler);

        raisin_sdk::RaisinClient client("state_monitor");
        if (!client.connect(argv[1])) return 1;

        // Subscribe to extended robot state
        client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
            std::cout << "Locomotion: " << state.getLocomotionStateName()
                      << " | Battery: " << state.voltage << "V"
                      << " | Control: " << state.getJoySourceName()
                      << std::endl;

            // Check for motor errors
            if (state.hasActuatorError()) {
                for (const auto& act : state.actuators) {
                    if (act.status != 0) {
                        std::cerr << "Motor error: " << act.name
                                  << " (status=" << act.status << ")" << std::endl;
                    }
                }
            }
        });

        std::cout << "Monitoring... (Ctrl+C to stop)" << std::endl;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        return 0;
    }

Battery Monitoring
^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        // Calculate battery percentage (linear approximation)
        double percentage = 0.0;
        if (state.max_voltage > state.min_voltage) {
            percentage = (state.voltage - state.min_voltage) /
                        (state.max_voltage - state.min_voltage) * 100.0;
        }

        std::cout << "Battery: " << state.voltage << "V (" << percentage << "%)"
                  << " | Current: " << state.current << "A"
                  << " | Temp: " << state.body_temperature << "°C"
                  << std::endl;

        // Low battery warning
        if (percentage < 20.0) {
            std::cerr << "Warning: Low battery!" << std::endl;
        }
    });

Manual/Autonomous Control Switching
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    raisin_sdk::RaisinClient client("control_switcher");
    if (!client.connect(argv[1])) return 1;

    // Check current control state
    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        std::cout << "Current control: " << state.getJoySourceName() << std::endl;
    });

    // Enable manual joystick control
    auto result = client.enableJoyControl();
    if (result.success) {
        std::cout << "Manual control enabled" << std::endl;
    }

    // ... manual operation ...

    // Disable manual control (before switching to autonomous)
    result = client.disableJoyControl();
    if (result.success) {
        std::cout << "Manual control disabled" << std::endl;
    }

    // Start autonomous navigation
    client.setWaypoints(waypoints, 1);

Odometry Monitoring (Current Position)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    client.subscribeOdometry([](const raisin_sdk::RobotState& state) {
        std::cout << "Position: (" << state.x << ", " << state.y << ") "
                  << "Yaw: " << (state.yaw * 180.0 / M_PI) << " deg "
                  << "Vel: (" << state.vx << ", " << state.vy << ") m/s"
                  << std::endl;
    });

    // Or get last state
    auto state = client.getRobotState();
    if (state.valid) {
        std::cout << "Current position: (" << state.x << ", " << state.y << ")" << std::endl;
    }

PointCloud Subscription (LiDAR)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& points) {
        std::cout << "Received " << points.size() << " points" << std::endl;

        // Process points
        for (const auto& p : points) {
            // Use p.x, p.y, p.z
        }
    });

    // Or get last pointcloud
    auto cloud = client.getLatestPointCloud();

Map Loading and Localization
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // Load PCD map file and send to robot
    auto result = client.setMap(
        "./office.pcd",  // PCD file path
        0.0, 0.0, 0.0,   // Initial position (x, y, z)
        0.0,             // Initial heading (yaw, radians)
        "office_map"     // Map name (used as waypoint frame)
    );

    if (result.success) {
        std::cout << "Map loaded: " << result.message << std::endl;
        // Now set waypoints using "office_map" frame
        std::vector<raisin_sdk::Waypoint> waypoints = {
            raisin_sdk::Waypoint("office_map", 5.0, 0.0),
            raisin_sdk::Waypoint("office_map", 5.0, 5.0),
        };
        client.setWaypoints(waypoints, 1);
    }

Mission Status Query
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    auto status = client.getMissionStatus();
    if (status.valid) {
        std::cout << "Active waypoints: " << status.waypoints.size() << std::endl;
        std::cout << "Current target: " << (int)status.current_index << std::endl;
        std::cout << "Remaining laps: " << (int)status.repetition << std::endl;

        // Check each waypoint
        for (size_t i = 0; i < status.waypoints.size(); ++i) {
            const auto& wp = status.waypoints[i];
            std::string state = (i < status.current_index) ? "DONE" :
                               (i == status.current_index) ? "CURRENT" : "PENDING";
            std::cout << "[" << i << "] (" << wp.x << ", " << wp.y
                      << ") " << state << std::endl;
        }
    }

----

GUI Demo (simple_gui)
---------------------

An OpenCV-based visualization demo program. It visualizes maps and robot positions, allowing you to set waypoints with mouse clicks.

Usage
^^^^^

.. code-block:: bash

    ./simple_gui <robot_id> [pcd_path]

    # Examples
    ./simple_gui 10.42.0.1
    ./simple_gui 10.42.0.1 ../maps/office1_example.pcd

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

FAQ (Frequently Asked Questions)
--------------------------------

This section is based on actual user inquiries.

.. _faq-valid-field-en:

Q: What is the valid field in MissionStatus and RobotState?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Related documentation**: See ``MissionStatus`` and ``RobotState`` in :ref:`Data Types <data-types-en>` section

**Question**:

    What does the ``valid`` field in MissionStatus/RobotState structures indicate,
    and when does it return false?

**Answer**:

The ``valid`` field indicates **whether asynchronous data reception or service call was successful**.

**RobotState.valid**

- ``true``: Odometry data has been received successfully
- ``false``: No odometry data received yet

.. code-block:: cpp

    // valid may be false immediately after connection
    client.connect("ROBOT_ID");
    client.subscribeOdometry([](const RobotState& s) { /* ... */ });

    // Query after a moment
    auto state = client.getRobotState();
    if (!state.valid) {
        // First odometry message not yet received
        std::cout << "Waiting for odometry..." << std::endl;
    }

**MissionStatus.valid**

- ``true``: ``getMissionStatus()`` service call succeeded
- ``false``: Service call failed (timeout, etc.)

.. code-block:: cpp

    auto status = client.getMissionStatus();
    if (!status.valid) {
        // No response received (timeout or disconnection)
        std::cerr << "Failed to get mission status" << std::endl;
    }

**Common causes when valid is false**:

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Situation
     - Description
   * - Before robot connection
     - Querying data before calling ``connect()``
   * - Right after connection
     - Subscriber hasn't received the first message yet (wait 1-2 seconds)
   * - Service timeout
     - No response within 5 seconds
   * - Network disconnection
     - Connection to robot is lost
   * - Plugin not loaded
     - The plugin providing the service is not loaded

**Recommended pattern**:

.. code-block:: cpp

    // Always check valid before using data
    auto state = client.getRobotState();
    if (state.valid) {
        std::cout << "Position: " << state.x << ", " << state.y << std::endl;
    } else {
        std::cout << "Data not available yet" << std::endl;
    }

    // Or use callback approach (callback is only called when data is valid)
    client.subscribeOdometry([](const RobotState& state) {
        // state.valid is always true here
        std::cout << "Position: " << state.x << ", " << state.y << std::endl;
    });

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
