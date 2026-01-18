API Example Reference
=====================

``RaisinClient`` 래퍼 클래스를 사용한 예제 코드입니다. 필요에 따라 수정하여 사용하세요.

헤더 포함
---------

.. code-block:: cpp

    #include "raisin_sdk/raisin_client.hpp"

기본 워크플로우
---------------

.. code-block:: cpp

    // 1. 클라이언트 생성
    raisin_sdk::RaisinClient client("my_app");

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

.. _data-types:

Data Types
----------

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

**ActuatorInfo** - 액추에이터(모터) 정보

.. code-block:: cpp

    struct ActuatorInfo {
        std::string name;           // 모터 이름 (e.g., "FR_hip", "FL_thigh")
        uint16_t status;            // 상태 코드 (0 = 정상)
        double temperature;         // 모터 온도 (°C)
        double position;            // 관절 위치 (rad)
        double velocity;            // 관절 속도 (rad/s)
        double effort;              // 관절 토크 (Nm)
    };

**LocomotionState** - 로봇 locomotion 상태 열거형

.. code-block:: cpp

    enum class LocomotionState : int32_t {
        COMM_DISABLED = 0,      // 통신 비활성
        COMM_ENABLED = 1,       // 통신 활성
        MOTOR_READY = 2,        // 모터 준비
        MOTOR_COMMUTATION = 3,  // 모터 정류 중
        MOTOR_ENABLED = 4,      // 모터 활성
        IN_TEST_MODE = 5,       // 테스트 모드
        STANDING_MODE = 6,      // 서있는 상태
        IN_CONTROL = 7,         // 제어 중 (걷기)
        SITDOWN_MODE = 8,       // 앉은 상태
        MOTOR_DISABLED = 9      // 모터 비활성
    };

**JoySourceType** - 조이스틱 제어 소스 타입

.. code-block:: cpp

    enum class JoySourceType : int32_t {
        JOY = 0,           // 수동 조이스틱 제어
        VEL_CMD = 1,       // 자율주행 속도 명령
        NUM_SOURCES = 2    // 제어 소스 없음
    };

**ExtendedRobotState** - 확장 로봇 상태 (배터리, 모터 상태 포함)

.. code-block:: cpp

    struct ExtendedRobotState {
        // 위치 및 속도
        double x, y, z;             // 위치 (meters)
        double yaw;                 // 방향 (radians)
        double vx, vy;              // 선속도 (m/s)
        double omega;               // 각속도 (rad/s)

        // Locomotion 상태
        int32_t locomotion_state;   // LocomotionState 열거형 값 (0-9)

        // 배터리 정보
        double voltage;             // 현재 전압 (V)
        double current;             // 전류 (A)
        double max_voltage;         // 최대 전압
        double min_voltage;         // 최소 전압

        // 온도
        double body_temperature;    // 본체 온도 (°C)

        // 조이스틱 제어 상태
        int32_t joy_listen_type;    // JoySourceType 열거형 값

        // 액추에이터 상태
        std::vector<ActuatorInfo> actuators;

        bool valid;

        // 유틸리티 메서드
        std::string getLocomotionStateName() const;  // 상태명 문자열
        std::string getJoySourceName() const;        // 제어 소스명 문자열
        bool isOperational() const;                  // 서있거나 걷는 중인지
        bool hasActuatorError() const;               // 모터 에러 여부
    };

RaisinClient Methods
--------------------

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

무한 순찰을 시작합니다 ( ``setWaypoints(waypoints, 0)`` 과 동일).

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

**subscribeRobotState()**

.. code-block:: cpp

    void subscribeRobotState(std::function<void(const ExtendedRobotState&)> callback);

확장 로봇 상태를 실시간 구독합니다. 배터리 정보, locomotion 상태, 액추에이터 상태 등을 포함합니다.

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

마지막으로 수신한 확장 로봇 상태를 반환합니다 (thread-safe).
``subscribeRobotState()`` 를 먼저 호출해야 유효한 데이터를 얻을 수 있습니다.

**findGuiNetworkId()**

.. code-block:: cpp

    std::string findGuiNetworkId(const std::string& prefix = "gui");

연결된 GUI의 네트워크 ID를 찾습니다.

- ``prefix``: GUI ID 접두사 (기본값: "gui")
- **반환**: GUI 네트워크 ID (예: "gui53-230486654196"), 없으면 빈 문자열

.. code-block:: cpp

    std::string guiId = client.findGuiNetworkId();
    std::cout << "Connected GUI: " << guiId << std::endl;

**setManualControl()**

.. code-block:: cpp

    ServiceResult setManualControl(const std::string& gui_network_id = "");

수동 조이스틱 제어를 활성화합니다 (joy/gui).
GUI 네트워크 ID를 자동 감지하여 해당 GUI에서 조이스틱 명령을 받을 수 있도록 합니다.

- ``gui_network_id``: GUI 네트워크 ID (빈 문자열이면 자동 감지, 감지 실패 시 기존 ID 유지)
- **반환**: 서비스 호출 결과

.. note::
    GUI 네트워크 ID를 자동 감지하지 못해도 서비스 호출은 성공합니다.
    이 경우 로봇에 기존에 설정된 network_id가 유지됩니다.

.. code-block:: cpp

    auto result = client.setManualControl();
    if (result.success) {
        std::cout << "Manual control enabled" << std::endl;
    }

**setAutonomousControl()**

.. code-block:: cpp

    ServiceResult setAutonomousControl();

자율주행 제어를 활성화합니다 (vel_cmd/autonomy).

- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.setAutonomousControl();
    if (result.success) {
        std::cout << "Autonomous control enabled" << std::endl;
    }

**releaseControl()**

.. code-block:: cpp

    ServiceResult releaseControl(const std::string& source = "joy/gui");

제어권을 해제합니다 (None 상태로 전환).

- ``source``: 해제할 제어 소스 ("joy/gui" 또는 "vel_cmd/autonomy")
- **반환**: 서비스 호출 결과

.. code-block:: cpp

    client.releaseControl("joy/gui");
    client.releaseControl("vel_cmd/autonomy");

.. note::
    제어 상태는 ``ExtendedRobotState.joy_listen_type`` 으로 확인할 수 있습니다:

    - ``JOY (0)``: 수동 조이스틱 제어 활성 (joy/gui)
    - ``VEL_CMD (1)``: 자율주행 속도 명령 수신 중 (vel_cmd/autonomy)
    - ``NONE (2)``: 제어 소스 없음

    ``setManualControl()`` 호출 시 GUI의 와이파이 아이콘이 초록색으로 변합니다.

GPS 사용 시 주의사항
--------------------

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS 모듈이 로봇에 장착** 되어 있어야 합니다
- GPS fix가 유효해야 합니다 (``/fix`` 토픽)
- GPS가 없으면 ``setWaypoints()`` 는 성공하지만 로봇이 목표에 도달하지 못합니다
