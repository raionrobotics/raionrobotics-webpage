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

    // 3. Load map from robot storage (auto-loads graph and default route)
    auto mapResult = client.loadMap("my_map");
    // mapResult.graphNodes, mapResult.graphEdges, mapResult.waypoints are available

    // 4. Set initial pose to start localization
    client.setInitialPose(0.0, 0.0, 0.0);  // x, y, yaw

    // 5. Subscribe to map odometry (position in map frame)
    client.subscribeMapOdometry([](const raisin_sdk::RobotState& state) {
        std::cout << "Position: " << state.x << ", " << state.y << std::endl;
    });

    // 6. Set waypoints (frame MUST match map_name!)
    std::vector<raisin_sdk::Waypoint> waypoints = {
        raisin_sdk::Waypoint("my_map", 5.0, 0.0),   // Use "my_map"
        raisin_sdk::Waypoint("my_map", 5.0, 5.0),
    };
    client.setWaypoints(waypoints, 1);

    // 7. Monitor status
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
    Waypoint::Map(x, y, z);               // Local map coordinates

.. warning::
    **The frame name MUST match the map_name specified in loadMap()!**

    .. code-block:: cpp

        // Correct example
        client.loadMap("office_map");
        Waypoint("office_map", 5.0, 0.0);  // Same name

        // Wrong example
        client.loadMap("office_map");
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

**GraphNode** - Path planning graph node

.. code-block:: cpp

    struct GraphNode {
        int32_t id;     // Node ID
        double x, y, z; // Coordinates
    };

**GraphEdge** - Path planning graph edge

.. code-block:: cpp

    struct GraphEdge {
        int32_t from_node;  // Start node ID
        int32_t to_node;    // End node ID
        double cost;        // Edge cost (distance)
    };

**LoadMapResult** - Map load result (includes graph and routes)

.. code-block:: cpp

    struct LoadMapResult {
        bool success;
        std::string message;
        std::string mapName;                      // Loaded map name
        std::vector<GraphNode> graphNodes;        // Auto-loaded graph nodes
        std::vector<GraphEdge> graphEdges;        // Auto-loaded graph edges
        std::vector<Waypoint> waypoints;          // Auto-loaded default route (route_1)
        std::vector<std::string> availableRoutes; // List of available routes
    };

**LoadGraphResult** - Graph load result

.. code-block:: cpp

    struct LoadGraphResult {
        bool success;
        std::string message;
        std::vector<GraphNode> nodes;
        std::vector<GraphEdge> edges;
    };

**RefineWaypointsResult** - Waypoint optimization result

.. code-block:: cpp

    struct RefineWaypointsResult {
        bool success;
        std::string message;
        std::vector<Waypoint> refined_waypoints;  // Optimized waypoints (follow graph)
        std::vector<int32_t> path_node_ids;       // Node IDs forming the path
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
        double temperature;         // Motor temperature (C)
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
        double body_temperature;    // Body temperature (C)

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
        std::vector<std::string> getActuatorsWithErrors() const; // Error motor list
        bool allActuatorsOperational() const;        // All motors running
    };

RaisinClient Methods
--------------------

Connection
~~~~~~~~~~

**connect()**

.. code-block:: cpp

    bool connect(const std::string& robot_id, int timeout_sec = 10,
                 std::atomic<bool>* cancel_token = nullptr);

Connects to the robot.

- ``robot_id``: Robot ID or IP address
- ``timeout_sec``: Connection timeout (seconds)
- ``cancel_token``: Optional cancellation flag
- **Returns**: Connection success

**disconnect()**

.. code-block:: cpp

    void disconnect();

Disconnects from the robot.

**isConnected()**

.. code-block:: cpp

    bool isConnected() const;

Returns the current connection status.

Map Loading
~~~~~~~~~~~

**loadMap()**

.. code-block:: cpp

    LoadMapResult loadMap(const std::string& name);

Loads a map saved on the robot and auto-loads associated graph and default route.

- ``name``: Map name (e.g., "my_map")
- **Returns**: LoadMapResult with graph, waypoints, and available routes

.. code-block:: cpp

    auto result = client.loadMap("office_map");
    if (result.success) {
        std::cout << "Graph: " << result.graphNodes.size() << " nodes" << std::endl;
        std::cout << "Route: " << result.waypoints.size() << " waypoints" << std::endl;
        std::cout << "Available routes: " << result.availableRoutes.size() << std::endl;
    }

.. note::
    ``loadMap()`` automatically:
    1. Loads the PCD map from robot storage
    2. Loads the graph file (if exists)
    3. Loads the default route "route_1" (if exists)
    4. Lists available routes for the map

**setInitialPose()**

.. code-block:: cpp

    ServiceResult setInitialPose(double x, double y, double yaw);

Sets the initial pose for localization on the loaded map.

- ``x``, ``y``: Initial position in map frame
- ``yaw``: Initial heading (radians)
- **Returns**: Service result

.. note::
    Must call ``loadMap()`` first. This starts localization at the specified position.

**getLoadedMapName()**

.. code-block:: cpp

    std::string getLoadedMapName() const;

Returns the currently loaded map name.

**listMapFiles()**

.. code-block:: cpp

    ListFilesResult listMapFiles();

Lists map files saved on the robot.

Waypoint Navigation
~~~~~~~~~~~~~~~~~~~

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

Patrol Route Management
~~~~~~~~~~~~~~~~~~~~~~~

**listWaypointsFiles()**

.. code-block:: cpp

    ListFilesResult listWaypointsFiles(const std::string& directory = "");

Lists saved patrol route files.

**loadWaypointsFile()**

.. code-block:: cpp

    ServiceResult loadWaypointsFile(const std::string& name);

Loads a saved patrol route file.

- ``name``: File name to load (without extension)

**saveWaypointsFile()**

.. code-block:: cpp

    ServiceResult saveWaypointsFile(const std::string& name);

Saves the current waypoints to a file on the robot.

- ``name``: File name to save (without extension)
- **Returns**: Service result

.. code-block:: cpp

    client.setWaypoints(myWaypoints, 1);
    client.saveWaypointsFile("my_route");

**resumePatrol()**

.. code-block:: cpp

    ResumePatrolResult resumePatrol();

Resumes patrol from the nearest waypoint in the currently loaded route.

- **Returns**: Resume patrol result (success, message, starting waypoint index)

.. note::
    Must load a route with ``loadWaypointsFile()`` first.

Graph Management
~~~~~~~~~~~~~~~~

**saveGraphFile()**

.. code-block:: cpp

    ServiceResult saveGraphFile(const std::string& name,
                                 const std::vector<GraphNode>& nodes,
                                 const std::vector<GraphEdge>& edges);

Saves a graph to a file on the robot.

- ``name``: Graph file name
- ``nodes``: Graph nodes
- ``edges``: Graph edges

.. code-block:: cpp

    std::vector<raisin_sdk::GraphNode> nodes = {
        {0, 0.0, 0.0, 0.0},
        {1, 5.0, 0.0, 0.0},
        {2, 5.0, 5.0, 0.0}
    };
    std::vector<raisin_sdk::GraphEdge> edges = {
        {0, 1, 5.0},  // Node 0 -> 1, cost 5.0
        {1, 0, 5.0},  // Bidirectional
        {1, 2, 5.0},
        {2, 1, 5.0}
    };
    client.saveGraphFile("my_map/graph", nodes, edges);

**loadGraphFile()**

.. code-block:: cpp

    LoadGraphResult loadGraphFile(const std::string& name);

Loads a graph file from the robot.

- ``name``: Graph file name
- **Returns**: LoadGraphResult with nodes and edges

**refineWaypoints()**

.. code-block:: cpp

    RefineWaypointsResult refineWaypoints(const std::vector<Waypoint>& waypoints,
                                           const std::vector<GraphNode>& nodes,
                                           const std::vector<GraphEdge>& edges);

Optimizes waypoints using A* algorithm on the graph.

- ``waypoints``: Input waypoints to refine
- ``nodes``: Graph nodes
- ``edges``: Graph edges
- **Returns**: RefineWaypointsResult with optimized waypoints and path

.. code-block:: cpp

    // Get graph from loadMap result
    auto mapResult = client.loadMap("office_map");

    // Define waypoints
    std::vector<raisin_sdk::Waypoint> waypoints = {
        {"office_map", 0.0, 0.0},
        {"office_map", 10.0, 10.0}
    };

    // Refine waypoints using graph
    auto refined = client.refineWaypoints(waypoints,
                                           mapResult.graphNodes,
                                           mapResult.graphEdges);

    if (refined.success) {
        // Use refined waypoints (follows graph edges)
        client.setWaypoints(refined.refined_waypoints, 1);
    }

Subscriptions
~~~~~~~~~~~~~

**subscribeMapOdometry()**

.. code-block:: cpp

    void subscribeMapOdometry(OdometryCallback callback);

Subscribes to robot odometry in the map frame.
Must call ``loadMap()`` and ``setInitialPose()`` first.

Topic: ``/{map_name}/{robot_id}/Odometry``

.. code-block:: cpp

    client.loadMap("office_map");
    client.setInitialPose(0.0, 0.0, 0.0);

    client.subscribeMapOdometry([](const raisin_sdk::RobotState& state) {
        std::cout << "Position in map: " << state.x << ", " << state.y << std::endl;
    });

**subscribeOdometry()**

.. code-block:: cpp

    void subscribeOdometry(OdometryCallback callback);

Subscribes to raw odometry (odom frame, Fast-LIO output).
Use ``subscribeMapOdometry()`` for map-aligned coordinates.

**subscribePointCloud()**

.. code-block:: cpp

    void subscribePointCloud(PointCloudCallback callback);

Subscribes to real-time LiDAR point cloud.

**subscribeRobotState()**

.. code-block:: cpp

    void subscribeRobotState(ExtendedRobotStateCallback callback);

Subscribes to extended robot state. Includes battery, locomotion state, actuator status.

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        std::cout << "Locomotion: " << state.getLocomotionStateName() << std::endl;
        std::cout << "Battery: " << state.voltage << "V" << std::endl;
        std::cout << "Control: " << state.getJoySourceName() << std::endl;

        if (state.hasActuatorError()) {
            std::cerr << "Warning: Actuator error detected!" << std::endl;
        }
    });

Getters (Thread-safe)
~~~~~~~~~~~~~~~~~~~~~

**getRobotState()**

.. code-block:: cpp

    RobotState getRobotState();

Returns last received odometry state.

**getExtendedRobotState()**

.. code-block:: cpp

    ExtendedRobotState getExtendedRobotState();

Returns last received extended robot state.

**getLatestPointCloud()**

.. code-block:: cpp

    std::vector<Point3D> getLatestPointCloud();

Returns last received point cloud.

Control Mode
~~~~~~~~~~~~

**setManualControl()**

.. code-block:: cpp

    ServiceResult setManualControl(const std::string& gui_network_id = "");

Enables manual joystick control (joy/gui).

**setAutonomousControl()**

.. code-block:: cpp

    ServiceResult setAutonomousControl();

Enables autonomous control (vel_cmd/autonomy).

**releaseControl()**

.. code-block:: cpp

    ServiceResult releaseControl(const std::string& source = "joy/gui");

Releases control (switches to None state).

**findGuiNetworkId()**

.. code-block:: cpp

    std::string findGuiNetworkId(const std::string& prefix = "gui");

Finds the connected GUI's network ID.

Locomotion Control
~~~~~~~~~~~~~~~~~~

**standUp()**

.. code-block:: cpp

    ServiceResult standUp();

Makes the robot stand up.

.. warning::
    Ensure the robot is in a safe position before calling.

**sitDown()**

.. code-block:: cpp

    ServiceResult sitDown();

Makes the robot sit down.

Actuator Status Helpers
~~~~~~~~~~~~~~~~~~~~~~~

**isActuatorStatusError()**

.. code-block:: cpp

    bool isActuatorStatusError(uint16_t status);

Checks if an actuator status code indicates an error state.

**getActuatorStatusName()**

.. code-block:: cpp

    std::string getActuatorStatusName(uint16_t status);

Converts an actuator status code to a human-readable string.

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
     - "ENABLED"
     - Operating (normal)
   * - 99
     - "ECAT_ERROR"
     - EtherCAT error

GPS Usage Notes
---------------

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS module must be installed** on the robot
- GPS fix must be valid (``/fix`` topic)
- Without GPS, ``setWaypoints()`` succeeds but robot won't reach target
