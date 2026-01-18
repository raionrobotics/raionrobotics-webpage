Service Interface Reference
===========================

사용 가능한 서비스 인터페이스 정의입니다.

planning/set_waypoints
----------------------

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
----------------------

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
------------------------

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
---------------------

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
----------------

.. code-block:: text

    string frame      # 좌표 프레임: "map_name", "gps", "odom"
    float64 x         # X 좌표 (GPS: latitude)
    float64 y         # Y 좌표 (GPS: longitude)
    float64 z         # Z 좌표 (GPS: altitude)
    bool use_z        # Z 좌표 검사 여부

set_listen (Joy 제어)
---------------------

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

- 활성화: ``"토픽명<&>클라이언트ID"`` (예: ``"joy<&>my_app"``)
- 비활성화: ``"토픽명<&><CLOSE>"`` (예: ``"joy<&><CLOSE>"``)

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

robot_state (Topic)
-------------------

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
     - 상태 코드 (0 = 정상)
   * - temperature
     - double
     - 모터 온도 (°C)

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
