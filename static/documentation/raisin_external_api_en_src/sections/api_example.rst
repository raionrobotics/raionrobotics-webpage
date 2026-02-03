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
        uint8_t repetition;               // Remaining laps (ignored if infinite_loop is true)
        bool infinite_loop;               // Whether patrol repeats indefinitely
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
        uint16_t status;            // CiA402 status code (see below)
        double temperature;         // Motor temperature (°C)
        double position;            // Joint position (rad)
        double velocity;            // Joint velocity (rad/s)
        double effort;              // Joint torque (Nm)
    };

**CiA402StatusWord** - Actuator status codes (EtherCAT CiA402 standard)

.. code-block:: cpp

    enum class CiA402StatusWord : uint16_t {
        NOT_READY_TO_SWITCH_ON = 0,   // Not ready (error state)
        FAULT = 8,                     // Fault (error state)
        READY_TO_SWITCH_ON = 33,       // Ready to switch on (normal standby)
        SWITCHED_ON = 35,              // Switched on (normal)
        OPERATION_ENABLED = 39,        // Operation enabled (normal operation)
        ECAT_CONN_ERROR = 99           // EtherCAT connection error
    };

.. note::
    **Status value interpretation:**

    - ``0``, ``8``, ``99`` = **Error states** (red warning required)
    - ``33``, ``35`` = **Normal standby** (motor ready/waiting)
    - ``39`` = **Normal operation** (motor running)

**ResumePatrolResult** - Resume patrol result

.. code-block:: cpp

    struct ResumePatrolResult {
        bool success;           // Success flag
        std::string message;    // Result message
        uint8_t waypoint_index; // Starting waypoint index
    };

**ListFilesResult** - File list query result

.. code-block:: cpp

    struct ListFilesResult {
        bool success;                    // Success flag
        std::string message;             // Result message
        std::vector<std::string> files;  // File name list
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
                                uint8_t start_index = 0,
                                bool infinite_loop = false);

Sets waypoint list and starts navigation.

- ``waypoints``: Waypoint list
- ``repetition``: Repeat count (ignored if ``infinite_loop`` is ``true``)

  - ``1`` = single pass (default)
  - ``N`` = repeat N times

- ``start_index``: Starting waypoint index
- ``infinite_loop``: Whether to repeat indefinitely (``true`` = infinite patrol)

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

Starts infinite patrol (equivalent to ``setWaypoints(waypoints, 1, 0, true)``).

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

Locomotion Control
------------------

**standUp()**

.. code-block:: cpp

    ServiceResult standUp();

Makes the robot stand up.

- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.standUp();
    if (result.success) {
        std::cout << "Robot standing up" << std::endl;
    }

.. warning::
    Ensure the robot is in a safe position before calling.
    Should be called when locomotion state is ``SITDOWN_MODE`` or ``STANDING_MODE``.

**sitDown()**

.. code-block:: cpp

    ServiceResult sitDown();

Makes the robot sit down.

- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.sitDown();
    if (result.success) {
        std::cout << "Robot sitting down" << std::endl;
    }

.. note::
    Can be called even when robot is in ``IN_CONTROL`` (walking) state.
    The robot will safely stop and sit down.

Patrol Route Management
-----------------------

**listWaypointsFiles()**

.. code-block:: cpp

    ListFilesResult listWaypointsFiles();

Lists saved patrol route files.

- **Returns**: File list result

.. code-block:: cpp

    auto result = client.listWaypointsFiles();
    if (result.success) {
        std::cout << "Available routes:" << std::endl;
        for (const auto& file : result.files) {
            std::cout << "  - " << file << std::endl;
        }
    }

**loadWaypointsFile()**

.. code-block:: cpp

    ServiceResult loadWaypointsFile(const std::string& filename);

Loads a saved patrol route file.

- ``filename``: File name to load (without extension)
- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.loadWaypointsFile("office_patrol");
    if (result.success) {
        std::cout << "Route loaded successfully" << std::endl;
    }

**resumePatrol()**

.. code-block:: cpp

    ResumePatrolResult resumePatrol();

Resumes patrol from the nearest waypoint in the currently loaded route.

- **Returns**: Resume patrol result (success, message, starting waypoint index)

.. code-block:: cpp

    auto result = client.resumePatrol();
    if (result.success) {
        std::cout << "Resuming from waypoint " << (int)result.waypoint_index << std::endl;
    }

.. note::
    Must load a route with ``loadWaypointsFile()`` first.
    Automatically finds the nearest waypoint from the robot's current position.

**loadAndResumePatrol()**

.. code-block:: cpp

    ResumePatrolResult loadAndResumePatrol(const std::string& filename);

Loads a patrol route and immediately resumes from the nearest waypoint.
Combines ``loadWaypointsFile()`` + ``resumePatrol()`` in one call.

- ``filename``: File name to load (without extension)
- **Returns**: Resume patrol result

.. code-block:: cpp

    auto result = client.loadAndResumePatrol("office_patrol");
    if (result.success) {
        std::cout << "Patrol started from waypoint " << (int)result.waypoint_index << std::endl;
    } else {
        std::cerr << "Failed: " << result.message << std::endl;
    }

Map Management
--------------

**listMapFiles()**

.. code-block:: cpp

    ListFilesResult listMapFiles(const std::string& directory = "");

Lists map files saved on the robot.

- ``directory``: Directory to query (default map directory if empty)
- **Returns**: File list result

.. code-block:: cpp

    auto result = client.listMapFiles();
    if (result.success) {
        std::cout << "Available maps:" << std::endl;
        for (const auto& file : result.files) {
            std::cout << "  - " << file << std::endl;
        }
    }

**saveMap()**

.. code-block:: cpp

    ServiceResult saveMap(const std::string& name);

Saves the current map to the robot.

- ``name``: Map name to save
- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.saveMap("office_floor1");
    if (result.success) {
        std::cout << "Map saved successfully" << std::endl;
    }

.. note::
    Use this to save maps created in mapping mode.

**loadMapFromRobot()**

.. code-block:: cpp

    ServiceResult loadMapFromRobot(const std::string& name);

Loads a map saved on the robot.

- ``name``: Map name to load
- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.loadMapFromRobot("office_floor1");
    if (result.success) {
        std::cout << "Map loaded successfully" << std::endl;
    }

.. note::
    Unlike ``setMap()``, this loads a map already saved on the robot.
    Initial position uses the position when the map was saved.

**startMapping()**

.. code-block:: cpp

    ServiceResult startMapping();

Starts mapping mode.

- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.startMapping();
    if (result.success) {
        std::cout << "Mapping started" << std::endl;
    }

.. warning::
    During mapping, manually operate the robot to scan the environment.
    Enable manual control with ``setManualControl()`` before using.

**stopMapping()**

.. code-block:: cpp

    ServiceResult stopMapping();

Stops mapping mode.

- **Returns**: Service call result

.. code-block:: cpp

    auto result = client.stopMapping();
    if (result.success) {
        std::cout << "Mapping stopped" << std::endl;
        // Save the map
        client.saveMap("new_map");
    }

Actuator Status Helpers
-----------------------

**isActuatorStatusError()**

.. code-block:: cpp

    bool isActuatorStatusError(uint16_t status);

Checks if an actuator status code indicates an error state.

- ``status``: CiA402 status code
- **Returns**: ``true`` if error state

.. code-block:: cpp

    for (const auto& act : state.actuators) {
        if (raisin_sdk::isActuatorStatusError(act.status)) {
            std::cerr << act.name << " has error!" << std::endl;
        }
    }

**getActuatorStatusName()**

.. code-block:: cpp

    std::string getActuatorStatusName(uint16_t status);

Converts an actuator status code to a human-readable string.

- ``status``: CiA402 status code
- **Returns**: Status name string

.. list-table::
   :header-rows: 1
   :widths: 15 35 50

   * - Code
     - Return Value
     - Description
   * - 0
     - "NOT_READY"
     - Not ready (error)
   * - 8
     - "FAULT"
     - Fault (error)
   * - 33
     - "READY"
     - Ready to switch on
   * - 35
     - "SWITCHED_ON"
     - Switched on
   * - 39
     - "OPERATIONAL"
     - Operating (normal)
   * - 99
     - "ECAT_ERROR"
     - EtherCAT error
   * - Other
     - "UNKNOWN(N)"
     - Unknown state

GPS Usage Notes
---------------

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS module must be installed** on the robot
- GPS fix must be valid (``/fix`` topic)
- Without GPS, ``setWaypoints()`` succeeds but robot won't reach target
