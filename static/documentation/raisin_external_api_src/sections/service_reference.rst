Service Interface Reference
===========================

사용 가능한 서비스 인터페이스 정의입니다.

Navigation Services
-------------------

planning/set_waypoints
~~~~~~~~~~~~~~~~~~~~~~

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
     - 반복 횟수 (infinite_loop가 true면 무시됨)
   * - current_index
     - uint8
     - 시작 인덱스
   * - infinite_loop
     - bool
     - 무한 순찰 여부

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
~~~~~~~~~~~~~~~~~~~~~~

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
     - 남은 반복 횟수 (infinite_loop가 true면 무시됨)
   * - current_index
     - uint8
     - 현재 목표 인덱스
   * - infinite_loop
     - bool
     - 무한 순찰 여부

planning/refine_waypoints
~~~~~~~~~~~~~~~~~~~~~~~~~

A* 알고리즘을 사용하여 그래프 상에서 waypoint를 최적화합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - waypoints
     - Waypoint[]
     - 입력 waypoint 배열
   * - nodes
     - GraphNode[]
     - 그래프 노드 배열
   * - edges
     - GraphEdge[]
     - 그래프 엣지 배열

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
   * - refined_waypoints
     - Waypoint[]
     - 최적화된 waypoint 목록 (그래프 경로를 따름)
   * - path_node_ids
     - int32[]
     - 경로를 구성하는 노드 ID 목록

**가능한 Response 메시지:**

- ``"Refined N waypoints"`` - 성공
- ``"No graph nodes provided"`` - 노드가 비어있음
- ``"service timeout"`` - 서비스 응답 없음

Patrol Route Services
---------------------

planning/list_waypoints_files
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

저장된 순찰 경로 파일 목록을 조회합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - directory
     - string
     - 조회할 디렉토리 (빈 문자열이면 기본 디렉토리)

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
   * - files
     - string[]
     - 파일 이름 목록

**가능한 Response 메시지:**

- ``"Found N files"`` - 성공 (N개 파일 발견)
- ``"service timeout"`` - 서비스 응답 없음

planning/load_waypoints_file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

저장된 순찰 경로 파일을 로드합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 로드할 파일 이름 (확장자 제외)

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

- ``"Waypoints loaded from: filename"`` - 성공
- ``"File not found: filename"`` - 파일 없음
- ``"service timeout"`` - 서비스 응답 없음

planning/save_waypoints_file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

현재 waypoint를 파일로 저장합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 저장할 파일 이름 (확장자 제외)

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

- ``"Waypoints saved to: filename"`` - 성공
- ``"service timeout"`` - 서비스 응답 없음

planning/resume_patrol
~~~~~~~~~~~~~~~~~~~~~~

현재 로드된 순찰 경로에서 가장 가까운 waypoint부터 순찰을 재개합니다.

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
   * - waypoint_index
     - uint8
     - 재개 시작 waypoint 인덱스

**가능한 Response 메시지:**

- ``"Resuming from waypoint N"`` - 성공 (N번 waypoint부터 시작)
- ``"No waypoints loaded"`` - 경로가 로드되지 않음
- ``"service timeout"`` - 서비스 응답 없음

.. note::
    ``planning/load_waypoints_file`` 로 경로를 먼저 로드해야 합니다.

Graph Services
--------------

save_graph_file
~~~~~~~~~~~~~~~

그래프를 파일로 저장합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 저장할 파일 이름
   * - nodes
     - GraphNode[]
     - 그래프 노드 배열
   * - edges
     - GraphEdge[]
     - 그래프 엣지 배열

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

- ``"Graph saved to: filename"`` - 성공
- ``"service timeout"`` - 서비스 응답 없음

load_graph_file
~~~~~~~~~~~~~~~

저장된 그래프 파일을 로드합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 로드할 파일 이름

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
   * - nodes
     - GraphNode[]
     - 로드된 노드 배열
   * - edges
     - GraphEdge[]
     - 로드된 엣지 배열

**가능한 Response 메시지:**

- ``"Graph loaded from: filename"`` - 성공
- ``"File not found: filename"`` - 파일 없음
- ``"service timeout"`` - 서비스 응답 없음

Map Services
------------

list_map_files
~~~~~~~~~~~~~~

로봇에 저장된 맵 파일 목록을 조회합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - directory
     - string
     - 조회할 디렉토리 (빈 문자열이면 기본 디렉토리)

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
   * - files
     - string[]
     - 맵 파일 이름 목록

load_laser_map
~~~~~~~~~~~~~~

로봇에 저장된 맵을 로드합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 로드할 맵 이름

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

- ``"Map loaded."`` - 성공
- ``"Map not found"`` - 맵 파일 없음
- ``"service timeout"`` - 서비스 응답 없음

.. note::
    SDK에서는 ``loadMap()`` 메소드가 이 서비스를 호출하고, 자동으로 그래프와 기본 경로도 로드합니다.

set_map (SetLaserMap)
~~~~~~~~~~~~~~~~~~~~~

Localization 초기 자세를 설정합니다.

.. note::
    SDK에서는 ``setInitialPose()`` 메소드를 사용하여 이 서비스를 호출합니다.
    먼저 ``loadMap()`` 으로 맵을 로드한 후에 호출해야 합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 맵 프레임 이름 (loadMap으로 로드된 맵 이름)
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

Locomotion Services
-------------------

stand_up
~~~~~~~~

로봇을 일어서게 합니다.

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

**가능한 Response 메시지:**

- ``"Trigger: started"`` - 성공
- ``"service timeout"`` - 서비스 응답 없음

sit_down
~~~~~~~~

로봇을 앉게 합니다.

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

**가능한 Response 메시지:**

- ``"Trigger: started"`` - 성공
- ``"service timeout"`` - 서비스 응답 없음

Control Mode Services
---------------------

set_listen (Joy 제어)
~~~~~~~~~~~~~~~~~~~~~

수동 조이스틱 제어를 활성화/비활성화합니다.

**Request:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - data
     - string
     - 제어 명령 문자열

**Request data 형식:**

- 수동 제어 활성화: ``"joy/gui<&>네트워크ID"``
- 자율 주행 활성화: ``"vel_cmd/autonomy"``
- 제어 해제: ``"토픽명<&><CLOSE>"`` (예: ``"joy/gui<&><CLOSE>"``)

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

Message Types
-------------

Waypoint
~~~~~~~~

.. code-block:: text

    string frame      # 좌표 프레임: "map_name", "gps", "odom"
    float64 x         # X 좌표 (GPS: latitude)
    float64 y         # Y 좌표 (GPS: longitude)
    float64 z         # Z 좌표 (GPS: altitude)
    bool use_z        # Z 좌표 검사 여부

GraphNode
~~~~~~~~~

.. code-block:: text

    int32 id          # 노드 ID
    float64 x         # X 좌표
    float64 y         # Y 좌표
    float64 z         # Z 좌표

GraphEdge
~~~~~~~~~

.. code-block:: text

    int32 from_node   # 시작 노드 ID
    int32 to_node     # 끝 노드 ID
    float64 cost      # 엣지 비용 (거리)

Topics
------

robot_state
~~~~~~~~~~~

로봇의 상태 정보를 발행하는 토픽입니다.

**토픽명:** ``robot_state``

**메시지 필드:**

.. list-table::
   :header-rows: 1
   :widths: 25 20 55

   * - Field
     - Type
     - Description
   * - actuator_states
     - ActuatorState[]
     - 액추에이터(모터) 상태 배열
   * - base_pos
     - double[3]
     - 베이스 위치 [x, y, z]
   * - base_quat
     - double[4]
     - 베이스 쿼터니언 [x, y, z, w]
   * - base_lin_vel
     - double[3]
     - 선속도 [vx, vy, vz]
   * - base_ang_vel
     - double[3]
     - 각속도 [wx, wy, wz]
   * - voltage
     - double
     - 현재 배터리 전압 (V)
   * - current
     - double
     - 현재 전류 (A)
   * - max_voltage
     - double
     - 최대 전압
   * - min_voltage
     - double
     - 최소 전압
   * - body_temperature
     - double
     - 본체 온도 (°C)
   * - state
     - int32
     - Locomotion 상태 (0-9)
   * - joy_listen_type
     - int32
     - Joy 소스 타입 (0=JOY, 1=VEL_CMD, 2=NONE)

**ActuatorState 메시지:**

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - Field
     - Type
     - Description
   * - name
     - string
     - 모터 이름 (예: "FR_hip")
   * - position
     - double
     - 관절 위치 (rad)
   * - velocity
     - double
     - 관절 속도 (rad/s)
   * - effort
     - double
     - 관절 토크 (Nm)
   * - status
     - uint16
     - CiA402 상태 코드 (아래 참조)
   * - temperature
     - double
     - 모터 온도 (°C)

**Actuator Status 코드 (CiA402 EtherCAT 표준):**

.. list-table::
   :header-rows: 1
   :widths: 15 30 55

   * - 값
     - 상태
     - 설명
   * - 0
     - NOT_READY_TO_SWITCH_ON
     - 준비 안됨 (**에러**)
   * - 8
     - FAULT
     - 결함 (**에러**)
   * - 33
     - READY_TO_SWITCH_ON
     - 스위치온 준비 (정상 대기)
   * - 35
     - SWITCHED_ON
     - 스위치온 (정상)
   * - 39
     - OPERATION_ENABLED
     - 동작 활성화 (정상 운전)
   * - 99
     - ECAT_CONN_ERROR
     - EtherCAT 연결 에러 (**에러**)

.. warning::
    **status 해석 주의:**

    - ``status == 0`` 은 "정상"이 **아닙니다**! NOT_READY 에러 상태입니다.
    - 정상 운전 중인 상태는 ``status == 39`` (OPERATION_ENABLED) 입니다.
    - 에러 판단: ``status == 0 || status == 8 || status == 99``

**Locomotion State 값:**

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - 값
     - 상태
   * - 0
     - COMM_DISABLED (통신 비활성)
   * - 1
     - COMM_ENABLED (통신 활성)
   * - 2
     - MOTOR_READY (모터 준비)
   * - 3
     - MOTOR_COMMUTATION (모터 정류 중)
   * - 4
     - MOTOR_ENABLED (모터 활성)
   * - 5
     - IN_TEST_MODE (테스트 모드)
   * - 6
     - STANDING_MODE (서있는 상태)
   * - 7
     - IN_CONTROL (제어 중/걷기)
   * - 8
     - SITDOWN_MODE (앉은 상태)
   * - 9
     - MOTOR_DISABLED (모터 비활성)

/Odometry
~~~~~~~~~

Fast-LIO 출력의 raw odometry 토픽입니다.

**토픽명:** ``/Odometry``

**메시지 타입:** nav_msgs/Odometry

.. note::
    맵 프레임에서의 위치를 얻으려면 ``subscribeMapOdometry()`` 를 사용하세요.
    ``/{map_name}/{robot_id}/Odometry`` 토픽을 구독합니다.

/cloud_registered
~~~~~~~~~~~~~~~~~

실시간 LiDAR 포인트 클라우드 토픽입니다.

**토픽명:** ``/cloud_registered``

**메시지 타입:** sensor_msgs/PointCloud2
