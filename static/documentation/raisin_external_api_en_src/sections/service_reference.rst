Service Interface Reference
===========================

Available service interface definitions.

planning/set_waypoints
----------------------

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
     - Repeat count (ignored if infinite_loop is true)
   * - current_index
     - uint8
     - Starting index
   * - infinite_loop
     - bool
     - Whether to repeat indefinitely

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
----------------------

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
     - Remaining laps (ignored if infinite_loop is true)
   * - current_index
     - uint8
     - Current target index
   * - infinite_loop
     - bool
     - Whether patrol repeats indefinitely

planning/append_waypoint
------------------------

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
---------------------

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
----------------

.. code-block:: text

    string frame      # Coordinate frame: "map_name", "gps", "odom"
    float64 x         # X coordinate (GPS: latitude)
    float64 y         # Y coordinate (GPS: longitude)
    float64 z         # Z coordinate (GPS: altitude)
    bool use_z        # Whether to check Z coordinate

set_listen (Joy Control)
------------------------

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
-------------------

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
     - CiA402 status code (see below)
   * - temperature
     - double
     - Motor temperature (°C)

**Actuator Status Codes (CiA402 EtherCAT Standard):**

.. list-table::
   :header-rows: 1
   :widths: 15 30 55

   * - Value
     - State
     - Description
   * - 0
     - NOT_READY_TO_SWITCH_ON
     - Not ready (**error**)
   * - 8
     - FAULT
     - Fault (**error**)
   * - 33
     - READY_TO_SWITCH_ON
     - Ready to switch on (normal standby)
   * - 35
     - SWITCHED_ON
     - Switched on (normal)
   * - 39
     - OPERATION_ENABLED
     - Operation enabled (normal operation)
   * - 99
     - ECAT_CONN_ERROR
     - EtherCAT connection error (**error**)

.. warning::
    **Status interpretation:**

    - ``status == 0`` is **NOT** "normal"! It's a NOT_READY error state.
    - Normal operation state is ``status == 39`` (OPERATION_ENABLED).
    - Error detection: ``status == 0 || status == 8 || status == 99``

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

stand_up
--------

Makes the robot stand up.

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

**Possible Response Messages:**

- ``"Trigger: started"`` - Success
- ``"service timeout"`` - No service response

sit_down
--------

Makes the robot sit down.

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

**Possible Response Messages:**

- ``"Trigger: started"`` - Success
- ``"service timeout"`` - No service response

planning/list_waypoints_files
-----------------------------

Lists saved patrol route files.

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
   * - files
     - string[]
     - File name list

**Possible Response Messages:**

- ``"Found N files"`` - Success (N files found)
- ``"service timeout"`` - No service response

planning/load_waypoints_file
----------------------------

Loads a saved patrol route file.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - filename
     - string
     - File name to load (without extension)

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

- ``"Waypoints loaded from: filename"`` - Success
- ``"File not found: filename"`` - File not found
- ``"service timeout"`` - No service response

planning/resume_patrol
----------------------

Resumes patrol from the nearest waypoint in the currently loaded route.

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
   * - waypoint_index
     - uint8
     - Starting waypoint index

**Possible Response Messages:**

- ``"Resuming from waypoint N"`` - Success (starting from waypoint N)
- ``"No waypoints loaded"`` - No route loaded
- ``"service timeout"`` - No service response

.. note::
    Must load a route with ``planning/load_waypoints_file`` first.

list_map_files
--------------

Lists map files saved on the robot.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - directory
     - string
     - Directory to query (default directory if empty)

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
   * - files
     - string[]
     - Map file name list

save_laser_map
--------------

Saves the current map to the robot.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - Map name to save

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
- ``"service timeout"`` - No service response

load_laser_map
--------------

Loads a map saved on the robot.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - Map name to load

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

- ``"Map loaded."`` - Success
- ``"Map not found"`` - Map file not found
- ``"service timeout"`` - No service response

start_mapping
-------------

Starts mapping mode.

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

**Possible Response Messages:**

- ``"Trigger: started"`` - Success
- ``"service timeout"`` - No service response

.. note::
    During mapping, manually operate the robot to scan the environment.

stop_mapping
------------

Stops mapping mode.

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

**Possible Response Messages:**

- ``"Trigger: started"`` - Success
- ``"service timeout"`` - No service response

.. note::
    After stopping mapping, save the map with ``save_laser_map``.
