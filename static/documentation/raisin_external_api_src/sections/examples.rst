Complete Examples
=================

기본 Waypoint 제어
------------------

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

        // 클라이언트 생성 및 연결
        raisin_sdk::RaisinClient client("waypoint_example");

        std::cout << "Connecting to " << robotId << "..." << std::endl;
        if (!client.connect(robotId)) {
            std::cerr << "Connection failed!" << std::endl;
            return 1;
        }
        std::cout << "Connected!" << std::endl;

        // 맵 로드
        std::cout << "Loading map..." << std::endl;
        auto mapResult = client.setMap(pcdPath, 0.0, 0.0, 0.0, 0.0, "sdk_map");
        if (!mapResult.success) {
            std::cerr << "Map load failed: " << mapResult.message << std::endl;
            return 1;
        }
        std::cout << "Map loaded: " << mapResult.message << std::endl;

        // Waypoint 설정 (frame은 반드시 map_name과 동일!)
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

        // 진행 상황 모니터링
        while (true) {
            auto status = client.getMissionStatus();
            if (status.valid) {
                std::cout << "\rProgress: waypoint " << (int)status.current_index
                          << "/" << status.waypoints.size()
                          << " | Laps: " << (int)status.repetition << "   " << std::flush;

                // 미션 완료 확인
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

실시간 모니터링
---------------

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

        // Odometry 구독
        client.subscribeOdometry([](const raisin_sdk::RobotState& state) {
            std::cout << "Position: (" << state.x << ", " << state.y << ") "
                      << "Yaw: " << state.yaw << " rad" << std::endl;
        });

        // PointCloud 구독
        client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& cloud) {
            std::cout << "LiDAR points: " << cloud.size() << std::endl;
        });

        std::cout << "Monitoring... (Ctrl+C to stop)" << std::endl;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        return 0;
    }

무한 순찰
---------

.. code-block:: cpp

    // 무한 순찰 시작
    std::vector<raisin_sdk::Waypoint> patrol = {
        raisin_sdk::Waypoint("sdk_map", 0.0, 0.0),
        raisin_sdk::Waypoint("sdk_map", 10.0, 0.0),
        raisin_sdk::Waypoint("sdk_map", 10.0, 10.0),
        raisin_sdk::Waypoint("sdk_map", 0.0, 10.0),
    };

    client.startPatrol(patrol);  // 또는 client.setWaypoints(patrol, 0);

    // 순찰 중지
    client.stopNavigation();

로봇 상태 모니터링
------------------

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

        // 확장 로봇 상태 구독
        client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
            std::cout << "Locomotion: " << state.getLocomotionStateName()
                      << " | Battery: " << state.voltage << "V"
                      << " | Control: " << state.getJoySourceName()
                      << std::endl;

            // 모터 에러 확인
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

배터리 모니터링
---------------

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        // 배터리 잔량 계산 (선형 근사)
        double percentage = 0.0;
        if (state.max_voltage > state.min_voltage) {
            percentage = (state.voltage - state.min_voltage) /
                        (state.max_voltage - state.min_voltage) * 100.0;
        }

        std::cout << "Battery: " << state.voltage << "V (" << percentage << "%)"
                  << " | Current: " << state.current << "A"
                  << " | Temp: " << state.body_temperature << "°C"
                  << std::endl;

        // 저전압 경고
        if (percentage < 20.0) {
            std::cerr << "Warning: Low battery!" << std::endl;
        }
    });

수동/자율 제어 전환
-------------------

.. code-block:: cpp

    raisin_sdk::RaisinClient client("control_switcher");
    if (!client.connect(argv[1])) return 1;

    // 현재 제어 상태 확인
    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        std::cout << "Current control: " << state.getJoySourceName() << std::endl;
    });

    // 수동 조이스틱 제어 활성화
    auto result = client.setManualControl();
    if (result.success) {
        std::cout << "Manual control enabled" << std::endl;
    }

    // ... 수동 조작 ...

    // 자율주행 제어 활성화
    result = client.setAutonomousControl();
    if (result.success) {
        std::cout << "Autonomous control enabled" << std::endl;
    }

    // 자율주행 시작
    client.setWaypoints(waypoints, 1);

Odometry 모니터링 (현재 위치)
-----------------------------

.. code-block:: cpp

    client.subscribeOdometry([](const raisin_sdk::RobotState& state) {
        std::cout << "Position: (" << state.x << ", " << state.y << ") "
                  << "Yaw: " << (state.yaw * 180.0 / M_PI) << " deg "
                  << "Vel: (" << state.vx << ", " << state.vy << ") m/s"
                  << std::endl;
    });

    // 또는 마지막 상태 조회
    auto state = client.getRobotState();
    if (state.valid) {
        std::cout << "Current position: (" << state.x << ", " << state.y << ")" << std::endl;
    }

PointCloud 구독 (LiDAR)
-----------------------

.. code-block:: cpp

    client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& points) {
        std::cout << "Received " << points.size() << " points" << std::endl;

        // 포인트 처리 예시
        for (const auto& p : points) {
            // p.x, p.y, p.z 사용
        }
    });

    // 또는 마지막 포인트클라우드 조회
    auto cloud = client.getLatestPointCloud();

맵 로드 및 Localization
-----------------------

.. code-block:: cpp

    // PCD 맵 파일 로드 및 로봇에 전송
    auto result = client.setMap(
        "./office.pcd",  // PCD 파일 경로
        0.0, 0.0, 0.0,   // 초기 위치 (x, y, z)
        0.0,             // 초기 방향 (yaw, radians)
        "office_map"     // 맵 이름 (waypoint frame으로 사용)
    );

    if (result.success) {
        std::cout << "Map loaded: " << result.message << std::endl;
        // 이제 "office_map" 프레임으로 waypoint 설정 가능
        std::vector<raisin_sdk::Waypoint> waypoints = {
            raisin_sdk::Waypoint("office_map", 5.0, 0.0),
            raisin_sdk::Waypoint("office_map", 5.0, 5.0),
        };
        client.setWaypoints(waypoints, 1);
    }

미션 상태 조회
--------------

.. code-block:: cpp

    auto status = client.getMissionStatus();
    if (status.valid) {
        std::cout << "Active waypoints: " << status.waypoints.size() << std::endl;
        std::cout << "Current target: " << (int)status.current_index << std::endl;
        std::cout << "Remaining laps: " << (int)status.repetition << std::endl;

        // 각 waypoint 확인
        for (size_t i = 0; i < status.waypoints.size(); ++i) {
            const auto& wp = status.waypoints[i];
            std::string state = (i < status.current_index) ? "DONE" :
                               (i == status.current_index) ? "CURRENT" : "PENDING";
            std::cout << "[" << i << "] (" << wp.x << ", " << wp.y
                      << ") " << state << std::endl;
        }
    }
