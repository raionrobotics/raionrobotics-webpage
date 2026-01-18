Direct Network API
==================

Direct ``raisin_network`` usage for advanced users.

Include Headers
---------------

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
------------------

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
------------------------

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
--------------------

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
--------------------

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
------------------------

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
