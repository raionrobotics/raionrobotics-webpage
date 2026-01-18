FAQ
===

Frequently asked questions.

General
-------

Q: What's the difference between RaisinClient and Direct Network?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``RaisinClient`` is an example wrapper class for common use cases.
Direct Network uses the ``raisin_network`` library directly for more fine-grained control.

* **RaisinClient**: Quick start, simple API
* **Direct Network**: Custom services, advanced features

Q: Can I use it without GPS?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Yes. PCD map-based autonomous navigation works without GPS.

.. code-block:: cpp

    // PCD map based (no GPS required)
    client.setMap("office.pcd", 0, 0, 0, 0, "office");
    Waypoint("office", 5.0, 3.0);  // Map coordinates

GPS requires a GPS module installed on the robot.

Q: Can I control multiple robots simultaneously?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Yes. Create separate ``RaisinClient`` instances for each robot.

.. code-block:: cpp

    raisin_sdk::RaisinClient client1("controller1");
    raisin_sdk::RaisinClient client2("controller2");

    client1.connect("robot1_id");
    client2.connect("robot2_id");

Waypoints
---------

Q: Why is the Waypoint frame important?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** The frame specifies the coordinate system. The ``map_name`` set in ``setMap()``
must match the Waypoint ``frame`` for the robot to navigate correctly.

.. code-block:: cpp

    // If map_name = "floor1"
    client.setMap("map.pcd", 0, 0, 0, 0, "floor1");

    // Waypoint must also use "floor1" frame
    Waypoint("floor1", 10.0, 5.0);  // ✓

    // Using different frame won't work
    Waypoint("map", 10.0, 5.0);     // ✗

Q: How do I set up infinite patrol?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Set ``repetition`` to 0 or use ``startPatrol()``.

.. code-block:: cpp

    // Method 1
    client.setWaypoints(waypoints, 0);  // repetition = 0

    // Method 2
    client.startPatrol(waypoints);

Q: Can I add waypoints during patrol?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Yes. Use ``appendWaypoint()``.

.. code-block:: cpp

    // Add new waypoint to existing mission
    client.appendWaypoint(raisin_sdk::Waypoint("my_map", 15.0, 10.0));

Control
-------

Q: How do I switch between manual and autonomous control?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Use ``setManualControl()`` and ``setAutonomousControl()``.

.. code-block:: cpp

    // Manual control (joystick)
    client.setManualControl();

    // Autonomous control
    client.setAutonomousControl();
    client.setWaypoints(waypoints, 1);

Q: How do I emergency stop?
^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Use ``stopNavigation()`` to stop autonomous navigation.

.. code-block:: cpp

    // Stop autonomous navigation
    client.stopNavigation();

For actual robot emergency stop, use the emergency stop button on the joystick.

Data
----

Q: What coordinate system is Odometry data in?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Map-based coordinates if a map is loaded, otherwise odom coordinates relative to start position.

Q: What is the PointCloud data format?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Provided as a vector of ``Point3D`` structs.

.. code-block:: cpp

    struct Point3D { float x, y, z; };

    client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& points) {
        for (const auto& p : points) {
            // Use p.x, p.y, p.z
        }
    });

Q: How do I check battery level?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Subscribe to battery information using ``subscribeRobotState()``.

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        double percentage = 0.0;
        if (state.max_voltage > state.min_voltage) {
            percentage = (state.voltage - state.min_voltage) /
                        (state.max_voltage - state.min_voltage) * 100.0;
        }
        std::cout << "Battery: " << percentage << "%" << std::endl;
    });

Compatibility
-------------

Q: Which robots are supported?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Supports the Raibo2 platform. Autonomy and Fast-LIO plugins must be loaded.

Q: Can I use it on Windows?
^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Currently only Ubuntu Linux is officially supported. Windows support is under consideration.

Q: Can I use it on ARM64 (Jetson, etc.)?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Yes. Both x86_64 and arm64 architectures are supported.
