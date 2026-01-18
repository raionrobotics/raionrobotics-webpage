Direct Network API
==================

고급 사용자를 위한 ``raisin_network`` 직접 사용 방법입니다.

헤더 포함
---------

.. code-block:: cpp

    #include "raisin_network/raisin.hpp"
    #include "raisin_network/network.hpp"
    #include "raisin_network/node.hpp"
    #include "raisin_interfaces/srv/set_waypoints.hpp"
    #include "raisin_interfaces/srv/get_waypoints.hpp"
    #include "raisin_interfaces/srv/append_waypoint.hpp"
    #include "raisin_interfaces/srv/set_laser_map.hpp"
    #include "raisin_interfaces/srv/string.hpp"       // Joy 제어용
    #include "raisin_interfaces/msg/robot_state.hpp"  // 로봇 상태 토픽

연결 패턴
---------

.. code-block:: cpp

    // 1. 초기화
    raisin::raisinInit();

    // 2. 네트워크 생성
    std::vector<std::vector<std::string>> threads = {{"main"}};
    auto network = std::make_shared<raisin::Network>(
        "my_client",      // 클라이언트 ID
        "external",       // 디바이스 타입
        threads
    );

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 3. 로봇 연결
    auto connection = network->connect("ROBOT_ID");
    if (!connection) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 4. Node 생성
    raisin::Node node(network);

서비스 클라이언트 생성
----------------------

.. code-block:: cpp

    // Waypoint 설정 클라이언트
    auto setClient = node.createClient<raisin::raisin_interfaces::srv::SetWaypoints>(
        "planning/set_waypoints", connection);

    // Waypoint 조회 클라이언트
    auto getClient = node.createClient<raisin::raisin_interfaces::srv::GetWaypoints>(
        "planning/get_waypoints", connection);

    // Waypoint 추가 클라이언트
    auto appendClient = node.createClient<raisin::raisin_interfaces::srv::AppendWaypoint>(
        "planning/append_waypoint", connection);

    // 맵 설정 클라이언트
    auto mapClient = node.createClient<raisin::raisin_interfaces::srv::SetLaserMap>(
        "set_map", connection);

    // Joy 제어 클라이언트
    auto joyListenClient = node.createClient<raisin::raisin_interfaces::srv::String>(
        "set_listen", connection);

    // 서비스 대기
    if (!setClient->waitForService(std::chrono::seconds(10))) {
        std::cerr << "Service not available" << std::endl;
    }

서비스 호출 예시
----------------

.. code-block:: cpp

    // SetWaypoints 요청 생성
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

    // 비동기 호출
    auto future = setClient->asyncSendRequest(request);

    if (future.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future.get();
        std::cout << "Success: " << response->success << std::endl;
        std::cout << "Message: " << response->message << std::endl;
    }

구독자 생성
-----------

.. code-block:: cpp

    // Odometry 구독
    auto odomSub = node.createSubscriber<raisin::nav_msgs::msg::Odometry>(
        "/Odometry", connection,
        [](const raisin::nav_msgs::msg::Odometry::SharedPtr& msg) {
            std::cout << "Position: " << msg->pose.pose.position.x
                      << ", " << msg->pose.pose.position.y << std::endl;
        });

    // PointCloud 구독
    auto cloudSub = node.createSubscriber<raisin::sensor_msgs::msg::PointCloud2>(
        "/cloud_registered", connection,
        [](const raisin::sensor_msgs::msg::PointCloud2::SharedPtr& msg) {
            std::cout << "Points: " << msg->width * msg->height << std::endl;
        });

    // RobotState 구독 (배터리, locomotion 상태, 액추에이터 등)
    auto stateSub = node.createSubscriber<raisin::raisin_interfaces::msg::RobotState>(
        "robot_state", connection,
        [](const raisin::raisin_interfaces::msg::RobotState::SharedPtr& msg) {
            std::cout << "Locomotion state: " << msg->state << std::endl;
            std::cout << "Battery: " << msg->voltage << "V / " << msg->current << "A" << std::endl;
            std::cout << "Joy source: " << msg->joy_listen_type << std::endl;
            std::cout << "Actuators: " << msg->actuator_states.size() << std::endl;
        });

Joy 제어 서비스 호출
--------------------

.. code-block:: cpp

    // 수동 조이스틱 제어 활성화
    auto enableRequest = std::make_shared<raisin::raisin_interfaces::srv::String::Request>();
    enableRequest->data = "joy<&>my_client_id";  // "토픽명<&>클라이언트ID"

    auto future = joyListenClient->asyncSendRequest(enableRequest);
    if (future.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future.get();
        std::cout << "Enable joy: " << response->success << std::endl;
    }

    // 조이스틱 제어 비활성화 (잠금)
    auto disableRequest = std::make_shared<raisin::raisin_interfaces::srv::String::Request>();
    disableRequest->data = "joy<&><CLOSE>";  // "토픽명<&><CLOSE>"

    auto future2 = joyListenClient->asyncSendRequest(disableRequest);
    if (future2.wait_for(std::chrono::seconds(5)) == std::future_status::ready) {
        auto response = future2.get();
        std::cout << "Disable joy: " << response->success << std::endl;
    }
