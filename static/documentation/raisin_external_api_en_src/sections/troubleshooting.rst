Troubleshooting
===============

Common issues and solutions.

Connection Issues
-----------------

Cannot Connect to Robot
^^^^^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* ``connect()`` returns false
* "Connection failed" error

**Solutions:**

1. **Check Network**

   .. code-block:: bash

       # Ping test to robot IP
       ping 192.168.1.xxx

2. **Verify Robot ID**

   * Check Robot ID in raisin_master console or raisin_gui
   * Ensure exact match

3. **Check Multicast**

   * Robot and client must be on same subnet
   * Verify router/switch is not blocking multicast

4. **Check Firewall**

   .. code-block:: bash

       # Check firewall status on Ubuntu
       sudo ufw status

       # Disable if necessary
       sudo ufw disable

Service Timeout
^^^^^^^^^^^^^^^

**Symptoms:**

* ``setWaypoints()`` etc. return "service timeout"
* Service calls fail

**Solutions:**

1. **Check Autonomy Plugin**

   * Verify Autonomy plugin is loaded in raisin_master
   * Fast-LIO plugin must also be loaded (for map operations)

2. **Check Connection Status**

   .. code-block:: cpp

       if (!client.isConnected()) {
           std::cerr << "Not connected!" << std::endl;
       }

3. **Increase Timeout** (if needed)

   Service responses may be delayed in high-latency network environments.

Map Loading Issues
------------------

"service timeout" Instead of "Map saved."
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Cause:**

* Fast-LIO plugin is not loaded

**Solutions:**

1. Verify Fast-LIO plugin is loaded in raisin_master
2. Check that LiDAR is working properly
3. Verify ``/cloud_registered`` topic is being published on robot

Cannot Find PCD File
^^^^^^^^^^^^^^^^^^^^

**Cause:**

* File path is incorrect

**Solutions:**

1. **Use Absolute Path (Recommended)**

   .. code-block:: cpp

       // Use absolute path instead of relative
       client.setMap("/home/user/maps/office.pcd", ...);

2. **Verify File Exists**

   .. code-block:: bash

       ls -la /path/to/map.pcd

Waypoint Issues
---------------

Robot Does Not Move
^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* Waypoint setting succeeds but robot doesn't move
* No path generated

**Solutions:**

1. **Check frame Name** (Most Common Cause)

   .. code-block:: cpp

       // frame from setMap's map_name must match Waypoint frame!
       client.setMap("...", 0, 0, 0, 0, "my_map");
       Waypoint("my_map", 5.0, 0.0);  // ✓ Match

       // Wrong example
       client.setMap("...", 0, 0, 0, 0, "my_map");
       Waypoint("map", 5.0, 0.0);     // ✗ Mismatch!

2. **Check Localization Status**

   * Verify map was loaded properly
   * Verify robot position is correctly set within the map

3. **Check Autonomy Mode**

   * Verify ``setAutonomousControl()`` was called

4. **Check Mission Status**

   .. code-block:: cpp

       auto status = client.getMissionStatus();
       std::cout << "Waypoints: " << status.waypoints.size() << std::endl;

Coordinates Don't Match
^^^^^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* Robot moves to unexpected location
* Waypoint coordinates don't match the map

**Solutions:**

1. **Verify Initial Position**

   * Check that initial position set in ``setMap()`` matches actual robot position
   * Initial yaw (direction) must also be accurate

2. **Check Coordinate System**

   * Waypoint coordinates are map-based (relative to map origin)
   * Uses meters

Subscription Issues
-------------------

Callback Not Being Called
^^^^^^^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* ``subscribeOdometry()`` etc. callbacks don't execute

**Solutions:**

1. **Check Connection Status**

   * Verify properly connected to robot

2. **Maintain Main Loop**

   .. code-block:: cpp

       // Keep main loop running to prevent program exit
       while (running) {
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
       }

3. **Verify Topic Publishing**

   * Use raisin_cli to check if topic is being published

Build Issues
------------

Cannot Find Library
^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* Link error: "cannot find -lraisin_network"

**Solutions:**

1. **Verify Path**

   .. code-block:: cmake

       # Check if RAISIN_SDK_PATH is correct
       message(STATUS "SDK Path: ${RAISIN_SDK_PATH}")

2. **Verify Library Exists**

   .. code-block:: bash

       ls -la /path/to/raisin_sdk_install/lib/

Library Load Failure at Runtime
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Symptoms:**

* "error while loading shared libraries"

**Solutions:**

.. code-block:: bash

    # Set LD_LIBRARY_PATH
    export LD_LIBRARY_PATH=/path/to/raisin_sdk_install/lib:$LD_LIBRARY_PATH

    # Or use ldconfig
    sudo ldconfig /path/to/raisin_sdk_install/lib

Getting Help
------------

If issues persist:

1. Record full error message content
2. Prepare code snippet being used
3. Check network configuration information
4. Check raisin_master console logs
