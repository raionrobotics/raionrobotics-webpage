Raisin External API Reference
==============================

외부 시스템에서 Raibo2의 자율주행 기능을 제어하기 위한 API 문서입니다.

`English Version <../en/index.html>`_

Overview
--------

Raisin 시스템은 외부 연동을 위해 두 가지 방식을 제공합니다:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - 방식
     - 설명
     - 용도
   * - **API Example**
     - ``AutonomyClient`` 래퍼 클래스 예제 코드
     - 빠른 시작, 참고용 구현체
   * - **Direct Network**
     - ``raisin_network`` 라이브러리 직접 사용
     - 커스텀 서비스 호출, 세밀한 제어 필요 시

.. note::
    **API Example (AutonomyClient)** 는 공식 지원 SDK가 아닌 예제 코드입니다.
    외부 개발자가 Raisin 플랫폼을 연동하는 방법을 보여주는 참고용 구현체이며,
    필요에 따라 수정하여 사용하시기 바랍니다.

.. code-block:: text

    +------------------------------------------+
    |           External Application           |
    +------------------------------------------+
    |  Option A: API Example                   |
    |  +------------------------------------+  |
    |  |  #include "autonomy_client.hpp"   |  |
    |  |  AutonomyClient client;           |  |
    |  |  client.connect("ROBOT_ID");      |  |
    |  |  client.setMap(...);              |  |
    |  |  client.setWaypoints(...);        |  |
    |  +------------------------------------+  |
    +------------------------------------------+
    |  Option B: Direct Network                |
    |  +------------------------------------+  |
    |  |  raisin::Network network;         |  |
    |  |  raisin::Node node;               |  |
    |  |  auto client = node.createClient  |  |
    |  |      <SetWaypoints>(...);         |  |
    |  +------------------------------------+  |
    +------------------------------------------+
                      |
                      | WebSocket
                      v
    +------------------------------------------+
    |                 Raibo2                   |
    |  +----------------+  +----------------+  |
    |  | Autonomy Plugin|  | Fast-LIO Plugin|  |
    |  | - Waypoints    |  | - Localization |  |
    |  | - Navigation   |  | - Map Loading  |  |
    |  +----------------+  +----------------+  |
    +------------------------------------------+

Integration Package
-------------------

획득 방법
^^^^^^^^^

연동 패키지는 Raion Robotics에서 제공합니다. 다음 방법으로 요청하세요:

- **이메일**: support@raionrobotics.com
- **웹사이트**: https://raionrobotics.com/support

패키지 구성
^^^^^^^^^^^

``raisin_sdk_install/`` 폴더에는 다음이 포함됩니다:

.. code-block:: text

    raisin_sdk_install/
    ├── include/                    # 헤더 파일
    │   ├── raisin_network/         # 네트워크 라이브러리
    │   ├── raisin_interfaces/      # 메시지/서비스 정의
    │   └── ...
    └── lib/                        # 컴파일된 라이브러리
        ├── libraisin_network.so
        ├── libraisin_interfaces.so
        └── ...

시스템 요구사항
^^^^^^^^^^^^^^^

.. code-block:: bash

    # Ubuntu 22.04
    sudo apt-get install libeigen3-dev libssl-dev libpcl-dev libopencv-dev

네트워크 요구사항
^^^^^^^^^^^^^^^^^

- 로봇과 클라이언트가 **동일 네트워크**에 있어야 합니다
- 멀티캐스트 트래픽이 허용되어야 합니다 (Robot ID 자동 발견용)
- 방화벽: TCP/UDP 포트 8080, 38371 등 허용 필요

Robot ID 확인
^^^^^^^^^^^^^

Robot ID는 ``raisin_master`` 실행 시 콘솔에 출력됩니다:

.. code-block:: text

    [raisin_master] Robot ID: railab_raibo-3614732983840
    [raisin_master] Listening on port 38371

또는 raisin_gui에서 확인할 수 있습니다.

----

API Example Reference
---------------------

``AutonomyClient`` 래퍼 클래스를 사용한 예제 코드입니다. 필요에 따라 수정하여 사용하세요.

헤더 포함
^^^^^^^^^

.. code-block:: cpp

    #include "raisin_sdk/autonomy_client.hpp"

기본 워크플로우
^^^^^^^^^^^^^^^

.. code-block:: cpp

    // 1. 클라이언트 생성
    raisin_sdk::AutonomyClient client("my_app");

    // 2. 로봇 연결
    client.connect("ROBOT_ID");

    // 3. 맵 로드 (Localization 활성화)
    client.setMap("/path/to/map.pcd", 0.0, 0.0, 0.0, 0.0, "my_map");

    // 4. Waypoint 설정 (frame은 반드시 map_name과 동일해야 함!)
    std::vector<raisin_sdk::Waypoint> waypoints = {
        raisin_sdk::Waypoint("my_map", 5.0, 0.0),   // "my_map" 사용
        raisin_sdk::Waypoint("my_map", 5.0, 5.0),
    };
    client.setWaypoints(waypoints, 1);

    // 5. 상태 모니터링
    auto status = client.getMissionStatus();

Data Types
^^^^^^^^^^

**Waypoint** - 네비게이션 목표 지점

.. code-block:: cpp

    struct Waypoint {
        std::string frame;  // 좌표 프레임 (중요!)
        double x, y, z;     // 좌표
        bool use_z;         // Z 좌표 사용 여부
    };

    // 생성 방법
    Waypoint("map_name", x, y);           // 맵 좌표
    Waypoint::GPS(latitude, longitude);   // GPS 좌표 (GPS 모듈 필요)

.. warning::
    **frame 이름은 setMap()에서 지정한 map_name과 반드시 일치해야 합니다!**

    .. code-block:: cpp

        // 올바른 예시
        client.setMap("/path/map.pcd", 0, 0, 0, 0, "office_map");
        Waypoint("office_map", 5.0, 0.0);  // ✓ 동일한 이름

        // 잘못된 예시
        client.setMap("/path/map.pcd", 0, 0, 0, 0, "office_map");
        Waypoint("map", 5.0, 0.0);  // ✗ 이름 불일치 - 동작 안함!

**MissionStatus** - 미션 상태

.. code-block:: cpp

    struct MissionStatus {
        std::vector<Waypoint> waypoints;  // 현재 waypoint 목록
        uint8_t current_index;            // 현재 목표 인덱스
        uint8_t repetition;               // 남은 반복 횟수 (0=무한)
        bool valid;                       // 데이터 유효성
    };

**ServiceResult** - 서비스 호출 결과

.. code-block:: cpp

    struct ServiceResult {
        bool success;         // 성공 여부
        std::string message;  // 결과 메시지
    };

**RobotState** - 로봇 상태 (Odometry)

.. code-block:: cpp

    struct RobotState {
        double x, y, z;    // 위치 (meters)
        double yaw;        // 방향 (radians)
        double vx, vy;     // 선속도 (m/s)
        double omega;      // 각속도 (rad/s)
        bool valid;
    };

**Point3D** - 포인트 클라우드 점

.. code-block:: cpp

    struct Point3D { float x, y, z; };

AutonomyClient Methods
^^^^^^^^^^^^^^^^^^^^^^

**connect()**

.. code-block:: cpp

    bool connect(const std::string& robot_id, int timeout_sec = 10);

로봇에 연결합니다.

- ``robot_id``: Robot ID 또는 IP 주소
- ``timeout_sec``: 연결 타임아웃 (초)
- **반환**: 연결 성공 여부

**setMap()**

.. code-block:: cpp

    ServiceResult setMap(const std::string& pcd_path,
                         double initial_x, double initial_y,
                         double initial_z, double initial_yaw,
                         const std::string& map_name);

PCD 맵 파일을 로드하고 Localization을 초기화합니다.

- ``pcd_path``: PCD 파일 경로 (**클라이언트 PC에 있는 파일**, 로봇으로 전송됨)
- ``initial_x/y/z``: 맵 내 로봇 초기 위치 (로봇이 물리적으로 있는 위치를 맵 좌표로 입력)
- ``initial_yaw``: 초기 방향 (radians)
- ``map_name``: 맵 프레임 이름 (**Waypoint frame과 일치해야 함**)

.. note::
    PCD 맵은 사전에 SLAM으로 생성해야 합니다. raisin_gui의 Mapping 기능을 사용하거나,
    Fast-LIO 플러그인으로 직접 생성할 수 있습니다.

**setWaypoints()**

.. code-block:: cpp

    ServiceResult setWaypoints(const std::vector<Waypoint>& waypoints,
                                uint8_t repetition = 1,
                                uint8_t start_index = 0);

Waypoint 목록을 설정하고 네비게이션을 시작합니다.

- ``waypoints``: Waypoint 목록
- ``repetition``: 반복 횟수

  - ``0`` = 무한 순찰
  - ``1`` = 1회 통과 (기본값)
  - ``N`` = N회 반복

- ``start_index``: 시작 waypoint 인덱스

**getMissionStatus()**

.. code-block:: cpp

    MissionStatus getMissionStatus();

현재 미션 상태를 조회합니다.

**appendWaypoint()**

.. code-block:: cpp

    ServiceResult appendWaypoint(const Waypoint& waypoint);

현재 미션 큐에 waypoint를 추가합니다.

**stopNavigation()**

.. code-block:: cpp

    ServiceResult stopNavigation();

자율주행을 중지합니다 (빈 waypoint 목록 설정).

**startPatrol()**

.. code-block:: cpp

    ServiceResult startPatrol(const std::vector<Waypoint>& waypoints);

무한 순찰을 시작합니다 (``setWaypoints(waypoints, 0)``과 동일).

**subscribeOdometry()**

.. code-block:: cpp

    void subscribeOdometry(std::function<void(const RobotState&)> callback);

Odometry 데이터를 실시간 구독합니다.

**subscribePointCloud()**

.. code-block:: cpp

    void subscribePointCloud(std::function<void(const std::vector<Point3D>&)> callback);

LiDAR 포인트 클라우드를 실시간 구독합니다.

**getRobotState() / getLatestPointCloud()**

.. code-block:: cpp

    RobotState getRobotState();
    std::vector<Point3D> getLatestPointCloud();

마지막으로 수신한 데이터를 반환합니다 (thread-safe).

**loadPCD() (static)**

.. code-block:: cpp

    static std::vector<Point3D> loadPCD(const std::string& pcd_path);

PCD 파일을 로드합니다 (로봇 전송 없이 시각화용).

GPS 사용 시 주의사항
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS 모듈이 로봇에 장착**되어 있어야 합니다
- GPS fix가 유효해야 합니다 (``/fix`` 토픽)
- GPS가 없으면 ``setWaypoints()``는 성공하지만 로봇이 목표에 도달하지 못합니다

----

Direct Network API
------------------

고급 사용자를 위한 ``raisin_network`` 직접 사용 방법입니다.

헤더 포함
^^^^^^^^^

.. code-block:: cpp

    #include "raisin_network/raisin.hpp"
    #include "raisin_network/network.hpp"
    #include "raisin_network/node.hpp"
    #include "raisin_interfaces/srv/set_waypoints.hpp"
    #include "raisin_interfaces/srv/get_waypoints.hpp"
    #include "raisin_interfaces/srv/append_waypoint.hpp"
    #include "raisin_interfaces/srv/set_laser_map.hpp"

연결 패턴
^^^^^^^^^

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
^^^^^^^^^^^^^^^^^^^^^^

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

    // 서비스 대기
    if (!setClient->waitForService(std::chrono::seconds(10))) {
        std::cerr << "Service not available" << std::endl;
    }

서비스 호출 예시
^^^^^^^^^^^^^^^^

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
^^^^^^^^^^^

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

----

Service Interface Reference
----------------------------

사용 가능한 서비스 인터페이스 정의입니다.

planning/set_waypoints
^^^^^^^^^^^^^^^^^^^^^^

Waypoint 목록을 설정합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - waypoints
     - Waypoint[]
     - Waypoint 배열
   * - repetition
     - uint8
     - 반복 횟수 (0=무한)
   * - current_index
     - uint8
     - 시작 인덱스

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - 성공 여부
   * - message
     - string
     - 결과 메시지

**가능한 Response 메시지:**

- ``"set waypoint success"`` - 성공
- ``"waypoint list is empty"`` - waypoint가 비어있음
- ``"service timeout"`` - 서비스 응답 없음

planning/get_waypoints
^^^^^^^^^^^^^^^^^^^^^^

현재 미션 상태를 조회합니다.

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
     - 성공 여부
   * - message
     - string
     - 결과 메시지
   * - waypoints
     - Waypoint[]
     - 현재 waypoint 목록
   * - repetition
     - uint8
     - 남은 반복 횟수
   * - current_index
     - uint8
     - 현재 목표 인덱스

planning/append_waypoint
^^^^^^^^^^^^^^^^^^^^^^^^

Waypoint를 큐에 추가합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - waypoint
     - Waypoint
     - 추가할 waypoint

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - 성공 여부
   * - message
     - string
     - 결과 메시지

set_map (SetLaserMap)
^^^^^^^^^^^^^^^^^^^^^

PCD 맵을 로드하고 Localization을 초기화합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 맵 프레임 이름
   * - pc
     - PointCloud2
     - 포인트 클라우드 데이터
   * - initial_pose
     - Pose
     - 초기 로봇 자세

**Response:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - success
     - bool
     - 성공 여부
   * - message
     - string
     - 결과 메시지

**가능한 Response 메시지:**

- ``"Map saved."`` - 성공
- ``"service timeout"`` - Fast-LIO 플러그인 미로드

Waypoint Message
^^^^^^^^^^^^^^^^

.. code-block:: text

    string frame      # 좌표 프레임: "map_name", "gps", "odom"
    float64 x         # X 좌표 (GPS: latitude)
    float64 y         # Y 좌표 (GPS: longitude)
    float64 z         # Z 좌표 (GPS: altitude)
    bool use_z        # Z 좌표 검사 여부

----

Complete Examples
-----------------

기본 Waypoint 제어
^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <thread>
    #include <chrono>
    #include "raisin_sdk/autonomy_client.hpp"

    int main(int argc, char* argv[]) {
        if (argc < 2) {
            std::cout << "Usage: " << argv[0] << " <robot_id> [pcd_path]" << std::endl;
            return 1;
        }

        std::string robotId = argv[1];
        std::string pcdPath = (argc >= 3) ? argv[2] : "../maps/office1_example.pcd";

        // 클라이언트 생성 및 연결
        raisin_sdk::AutonomyClient client("waypoint_example");

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
^^^^^^^^^^^^^^^

.. code-block:: cpp

    #include <iostream>
    #include <atomic>
    #include <csignal>
    #include "raisin_sdk/autonomy_client.hpp"

    std::atomic<bool> running{true};
    void signalHandler(int) { running = false; }

    int main(int argc, char* argv[]) {
        std::signal(SIGINT, signalHandler);

        raisin_sdk::AutonomyClient client("monitor");
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
^^^^^^^^^

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

----

GUI Demo (simple_gui)
---------------------

OpenCV 기반의 시각화 데모 프로그램입니다. 맵과 로봇 위치를 시각화하고, 마우스로 Waypoint를 설정할 수 있습니다.

실행 방법
^^^^^^^^^

.. code-block:: bash

    ./simple_gui <robot_id> [pcd_path]

    # 예시
    ./simple_gui railab_raibo-3614732983840
    ./simple_gui railab_raibo-3614732983840 ../maps/office1_example.pcd

워크플로우
^^^^^^^^^^

1. ``O`` 키로 PCD 맵 파일 열기
2. 맵에서 클릭하여 로봇 초기 위치 설정
3. 로봇이 바라보는 방향으로 클릭하여 방향(Yaw) 설정
4. ``S`` 키로 맵 전송 및 Localization 시작
5. 맵에서 클릭하여 Waypoint 추가
6. ``W`` 키로 Waypoint 전송 및 자율주행 시작

키보드 단축키
^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - 키
     - 기능
   * - ``O``
     - PCD 맵 파일 열기
   * - ``S``
     - 맵 전송 (Localization 시작)
   * - ``W``
     - Waypoint 전송 (자율주행 시작)
   * - ``C``
     - Waypoint 초기화
   * - ``R``
     - 반복 횟수 변경 (1 → 5 → ∞)
   * - ``X``
     - Initial pose 리셋
   * - ``+/-``
     - 줌 인/아웃
   * - ``Q``
     - 종료

마우스 조작
^^^^^^^^^^^

- **좌클릭**: 워크플로우 단계별 동작 (위치/방향/Waypoint)
- **우클릭 드래그**: 화면 이동 (Pan)
- **스크롤**: 줌

상태 표시
^^^^^^^^^

- 화면 하단에 현재 단계 및 다음 동작 안내 표시
- Localization 활성화 전에는 Waypoint 추가 불가
- 이미 Localization이 활성화된 상태에서 ``S`` 를 누르면 리셋 안내

.. note::
    simple_gui는 ``zenity`` 패키지를 사용하여 파일 선택 다이얼로그를 표시합니다.
    Ubuntu에서 ``sudo apt-get install zenity`` 로 설치할 수 있습니다.

----

CMake Configuration
-------------------

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.16)
    project(my_controller)

    set(CMAKE_CXX_STANDARD 17)

    # SDK 경로
    set(RAISIN_SDK_PATH "/path/to/raisin_sdk_install")

    # 의존성
    find_package(Eigen3 REQUIRED)
    find_package(OpenSSL REQUIRED)

    set(PCL_INCLUDE_DIRS "/usr/include/pcl-1.12")
    set(PCL_LIBRARIES pcl_common pcl_io)

    # 실행 파일
    add_executable(my_controller main.cpp)

    target_include_directories(my_controller PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${RAISIN_SDK_PATH}/include
        ${PCL_INCLUDE_DIRS}
    )

    target_link_libraries(my_controller PRIVATE
        ${RAISIN_SDK_PATH}/lib/libraisin_network.so
        ${RAISIN_SDK_PATH}/lib/libraisin_thread_pool.so
        ${RAISIN_SDK_PATH}/lib/libraisin_parameter.so
        ${RAISIN_SDK_PATH}/lib/libraisin_encryption.so
        ${RAISIN_SDK_PATH}/lib/libraisin_data_logger.so
        ${RAISIN_SDK_PATH}/lib/libraisin_util.so
        ${RAISIN_SDK_PATH}/lib/libraisim.so
        ${RAISIN_SDK_PATH}/lib/libzstd.so
        Eigen3::Eigen
        OpenSSL::SSL OpenSSL::Crypto
        ${PCL_LIBRARIES}
        pthread dl rt
    )

    set_target_properties(my_controller PROPERTIES
        BUILD_RPATH "${RAISIN_SDK_PATH}/lib"
    )

----

Troubleshooting
---------------

연결 실패
^^^^^^^^^

**증상**: ``connect()`` 반환값이 false

**확인 사항**:

1. ``raisin_master``가 로봇에서 실행 중인지 확인
2. 동일 네트워크에 있는지 확인
3. Robot ID가 정확한지 확인 (raisin_master 콘솔 출력 참조)

맵 로드 실패
^^^^^^^^^^^^

**증상**: ``setMap()`` 실패 또는 타임아웃

**확인 사항**:

1. **Fast-LIO 플러그인**이 로드되어 있는지 확인 (미로드 시: raisin_plugin 문서의 FastLIO 섹션 참조)
2. PCD 파일 경로가 올바른지 확인
3. PCD 파일이 손상되지 않았는지 확인 (``loadPCD()``로 테스트)

로봇이 움직이지 않음
^^^^^^^^^^^^^^^^^^^^

**증상**: ``setWaypoints()`` 성공했지만 로봇이 정지

**확인 사항**:

1. **Autonomy 플러그인**이 로드되어 있는지 확인 (미로드 시: raisin_plugin 문서의 Autonomy 섹션 참조)
2. **frame 이름이 map_name과 일치하는지 확인** (가장 흔한 원인!)
3. Waypoint 좌표가 맵 범위 내에 있는지 확인
4. 조이스틱 오버라이드 상태가 아닌지 확인

.. code-block:: cpp

    // 흔한 실수
    client.setMap("...", 0, 0, 0, 0, "office_map");
    client.setWaypoints({Waypoint("map", 5.0, 0.0)}, 1);  // ✗ "map" != "office_map"

    // 올바른 사용
    client.setMap("...", 0, 0, 0, 0, "office_map");
    client.setWaypoints({Waypoint("office_map", 5.0, 0.0)}, 1);  // ✓

서비스 응답 메시지 목록
^^^^^^^^^^^^^^^^^^^^^^^

**SetWaypoints:**

- ``"set waypoint success"`` - 성공
- ``"waypoint list is empty"`` - 빈 목록

**SetMap (SetLaserMap):**

- ``"Map saved."`` - 성공
- 타임아웃 - Fast-LIO 플러그인 미로드

**GetWaypoints:**

- ``"get waypoint success"`` - 성공

----

관련 문서
---------

- **raisin_master 문서**: 로봇 설정 및 플러그인 로드 방법
- **raisin_plugin 문서**: Autonomy, Fast-LIO 플러그인 상세

----

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
