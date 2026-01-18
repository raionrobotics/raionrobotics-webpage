Complete Examples
=================

Basic Waypoint Control
----------------------

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
--------------------

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
---------------

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
----------------------

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
------------------

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
-----------------------------------

.. code-block:: cpp

    raisin_sdk::RaisinClient client("control_switcher");
    if (!client.connect(argv[1])) return 1;

    // Check current control state
    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        std::cout << "Current control: " << state.getJoySourceName() << std::endl;
    });

    // Enable manual joystick control
    auto result = client.setManualControl();
    if (result.success) {
        std::cout << "Manual control enabled" << std::endl;
    }

    // ... manual operation ...

    // Enable autonomous control
    result = client.setAutonomousControl();
    if (result.success) {
        std::cout << "Autonomous control enabled" << std::endl;
    }

    // Start autonomous navigation
    client.setWaypoints(waypoints, 1);

Odometry Monitoring (Current Position)
--------------------------------------

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
-------------------------------

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
----------------------------

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
--------------------

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
