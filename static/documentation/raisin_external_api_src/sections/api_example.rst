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
        uint8_t repetition;               // 남은 반복 횟수 (infinite_loop가 true면 무시됨)
        bool infinite_loop;               // 무한 순찰 여부
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
        uint16_t status;            // CiA402 상태 코드 (아래 참조)
        double temperature;         // 모터 온도 (°C)
        double position;            // 관절 위치 (rad)
        double velocity;            // 관절 속도 (rad/s)
        double effort;              // 관절 토크 (Nm)
    };

**CiA402StatusWord** - 액추에이터 상태 코드 (EtherCAT CiA402 표준)

.. code-block:: cpp

    enum class CiA402StatusWord : uint16_t {
        NOT_READY_TO_SWITCH_ON = 0,   // 준비 안됨 (에러 상태)
        FAULT = 8,                     // 결함 (에러 상태)
        READY_TO_SWITCH_ON = 33,       // 스위치온 준비 (정상 대기)
        SWITCHED_ON = 35,              // 스위치온 (정상)
        OPERATION_ENABLED = 39,        // 동작 활성화 (정상 운전)
        ECAT_CONN_ERROR = 99           // EtherCAT 연결 에러
    };

.. note::
    **status 값 해석:**

    - ``0``, ``8``, ``99`` = **에러 상태** (빨간색 경고 필요)
    - ``33``, ``35`` = **정상 대기** (모터 준비/대기 중)
    - ``39`` = **정상 운전** (모터 작동 중)

**ResumePatrolResult** - 순찰 재개 결과

.. code-block:: cpp

    struct ResumePatrolResult {
        bool success;           // 성공 여부
        std::string message;    // 결과 메시지
        uint8_t waypoint_index; // 재개 시작 waypoint 인덱스
    };

**ListFilesResult** - 파일 목록 조회 결과

.. code-block:: cpp

    struct ListFilesResult {
        bool success;                    // 성공 여부
        std::string message;             // 결과 메시지
        std::vector<std::string> files;  // 파일 이름 목록
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
                                uint8_t start_index = 0,
                                bool infinite_loop = false);

Waypoint 목록을 설정하고 네비게이션을 시작합니다.

- ``waypoints``: Waypoint 목록
- ``repetition``: 반복 횟수 (``infinite_loop`` 가 ``true`` 면 무시됨)

  - ``1`` = 1회 통과 (기본값)
  - ``N`` = N회 반복

- ``start_index``: 시작 waypoint 인덱스
- ``infinite_loop``: 무한 순찰 여부 (``true`` 면 경로를 무한 반복)

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

무한 순찰을 시작합니다 ( ``setWaypoints(waypoints, 1, 0, true)`` 과 동일).

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

Locomotion Control
------------------

**standUp()**

.. code-block:: cpp

    ServiceResult standUp();

로봇을 일어서게 합니다.

- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.standUp();
    if (result.success) {
        std::cout << "Robot standing up" << std::endl;
    }

.. warning::
    로봇이 안전한 위치에 있는지 확인한 후 호출하세요.
    locomotion 상태가 ``SITDOWN_MODE`` 또는 ``STANDING_MODE`` 일 때 호출해야 합니다.

**sitDown()**

.. code-block:: cpp

    ServiceResult sitDown();

로봇을 앉게 합니다.

- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.sitDown();
    if (result.success) {
        std::cout << "Robot sitting down" << std::endl;
    }

.. note::
    로봇이 ``IN_CONTROL`` (걷는 중) 상태에서도 호출 가능합니다.
    로봇은 안전하게 멈추고 앉습니다.

Patrol Route Management
-----------------------

**listWaypointsFiles()**

.. code-block:: cpp

    ListFilesResult listWaypointsFiles();

저장된 순찰 경로 파일 목록을 조회합니다.

- **반환**: 파일 목록 결과

.. code-block:: cpp

    auto result = client.listWaypointsFiles();
    if (result.success) {
        std::cout << "Available routes:" << std::endl;
        for (const auto& file : result.files) {
            std::cout << "  - " << file << std::endl;
        }
    }

**loadWaypointsFile()**

.. code-block:: cpp

    ServiceResult loadWaypointsFile(const std::string& filename);

저장된 순찰 경로 파일을 로드합니다.

- ``filename``: 로드할 파일 이름 (확장자 제외)
- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.loadWaypointsFile("office_patrol");
    if (result.success) {
        std::cout << "Route loaded successfully" << std::endl;
    }

**resumePatrol()**

.. code-block:: cpp

    ResumePatrolResult resumePatrol();

현재 로드된 순찰 경로에서 가장 가까운 waypoint부터 순찰을 재개합니다.

- **반환**: 순찰 재개 결과 (성공 여부, 메시지, 시작 waypoint 인덱스)

.. code-block:: cpp

    auto result = client.resumePatrol();
    if (result.success) {
        std::cout << "Resuming from waypoint " << (int)result.waypoint_index << std::endl;
    }

.. note::
    ``loadWaypointsFile()`` 로 경로를 먼저 로드해야 합니다.
    로봇의 현재 위치에서 가장 가까운 waypoint를 자동으로 찾아 순찰을 시작합니다.

**loadAndResumePatrol()**

.. code-block:: cpp

    ResumePatrolResult loadAndResumePatrol(const std::string& filename);

순찰 경로를 로드하고 즉시 가장 가까운 waypoint부터 순찰을 재개합니다.
``loadWaypointsFile()`` + ``resumePatrol()`` 을 한 번에 수행합니다.

- ``filename``: 로드할 파일 이름 (확장자 제외)
- **반환**: 순찰 재개 결과

.. code-block:: cpp

    auto result = client.loadAndResumePatrol("office_patrol");
    if (result.success) {
        std::cout << "Patrol started from waypoint " << (int)result.waypoint_index << std::endl;
    } else {
        std::cerr << "Failed: " << result.message << std::endl;
    }

Map Management
--------------

**listMapFiles()**

.. code-block:: cpp

    ListFilesResult listMapFiles(const std::string& directory = "");

로봇에 저장된 맵 파일 목록을 조회합니다.

- ``directory``: 조회할 디렉토리 (빈 문자열이면 기본 맵 디렉토리)
- **반환**: 파일 목록 결과

.. code-block:: cpp

    auto result = client.listMapFiles();
    if (result.success) {
        std::cout << "Available maps:" << std::endl;
        for (const auto& file : result.files) {
            std::cout << "  - " << file << std::endl;
        }
    }

**saveMap()**

.. code-block:: cpp

    ServiceResult saveMap(const std::string& name);

현재 맵을 로봇에 저장합니다.

- ``name``: 저장할 맵 이름
- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.saveMap("office_floor1");
    if (result.success) {
        std::cout << "Map saved successfully" << std::endl;
    }

.. note::
    매핑 모드에서 생성된 맵을 저장할 때 사용합니다.

**loadMapFromRobot()**

.. code-block:: cpp

    ServiceResult loadMapFromRobot(const std::string& name);

로봇에 저장된 맵을 로드합니다.

- ``name``: 로드할 맵 이름
- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.loadMapFromRobot("office_floor1");
    if (result.success) {
        std::cout << "Map loaded successfully" << std::endl;
    }

.. note::
    ``setMap()`` 과 달리 로봇에 이미 저장된 맵을 로드합니다.
    초기 위치는 맵 저장 시점의 위치가 사용됩니다.

**startMapping()**

.. code-block:: cpp

    ServiceResult startMapping();

매핑 모드를 시작합니다.

- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.startMapping();
    if (result.success) {
        std::cout << "Mapping started" << std::endl;
    }

.. warning::
    매핑 중에는 로봇을 수동으로 조작하여 환경을 스캔해야 합니다.
    ``setManualControl()`` 로 수동 제어를 활성화한 후 사용하세요.

**stopMapping()**

.. code-block:: cpp

    ServiceResult stopMapping();

매핑 모드를 중지합니다.

- **반환**: 서비스 호출 결과

.. code-block:: cpp

    auto result = client.stopMapping();
    if (result.success) {
        std::cout << "Mapping stopped" << std::endl;
        // 맵 저장
        client.saveMap("new_map");
    }

Actuator Status Helpers
-----------------------

**isActuatorStatusError()**

.. code-block:: cpp

    bool isActuatorStatusError(uint16_t status);

액추에이터 상태 코드가 에러 상태인지 확인합니다.

- ``status``: CiA402 상태 코드
- **반환**: 에러 상태이면 ``true``

.. code-block:: cpp

    for (const auto& act : state.actuators) {
        if (raisin_sdk::isActuatorStatusError(act.status)) {
            std::cerr << act.name << " has error!" << std::endl;
        }
    }

**getActuatorStatusName()**

.. code-block:: cpp

    std::string getActuatorStatusName(uint16_t status);

액추에이터 상태 코드를 사람이 읽을 수 있는 문자열로 변환합니다.

- ``status``: CiA402 상태 코드
- **반환**: 상태 이름 문자열

.. list-table::
   :header-rows: 1
   :widths: 15 35 50

   * - 코드
     - 반환값
     - 설명
   * - 0
     - "NOT_READY"
     - 준비 안됨 (에러)
   * - 8
     - "FAULT"
     - 결함 (에러)
   * - 33
     - "READY"
     - 스위치온 준비
   * - 35
     - "SWITCHED_ON"
     - 스위치온
   * - 39
     - "OPERATIONAL"
     - 동작 중 (정상)
   * - 99
     - "ECAT_ERROR"
     - EtherCAT 에러
   * - 기타
     - "UNKNOWN(N)"
     - 알 수 없는 상태

GPS 사용 시 주의사항
--------------------

.. code-block:: cpp

    Waypoint::GPS(37.5665, 126.9780);

- **GPS 모듈이 로봇에 장착** 되어 있어야 합니다
- GPS fix가 유효해야 합니다 (``/fix`` 토픽)
- GPS가 없으면 ``setWaypoints()`` 는 성공하지만 로봇이 목표에 도달하지 못합니다
