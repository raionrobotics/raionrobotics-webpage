API Example Reference
=====================

Example code using the ``RaisinClient`` wrapper class. Modify as needed for your use case.

Include Header
--------------

.. code-block:: cpp

    #include "raisin_sdk/raisin_client.hpp"

Basic Workflow
--------------

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
----------

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
--------------------

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
---------------

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS module must be installed** on the robot
- GPS fix must be valid (``/fix`` topic)
- Without GPS, ``setWaypoints()`` succeeds but robot won't reach target
