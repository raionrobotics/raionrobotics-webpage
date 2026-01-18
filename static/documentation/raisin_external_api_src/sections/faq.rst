FAQ
===

자주 묻는 질문입니다.

일반
----

Q: RaisinClient와 Direct Network의 차이점은?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``RaisinClient`` 는 일반적인 사용 사례를 위한 예제 래퍼 클래스입니다.
Direct Network는 ``raisin_network`` 라이브러리를 직접 사용하여 더 세밀한 제어가 가능합니다.

* **RaisinClient**: 빠른 시작, 간단한 API
* **Direct Network**: 커스텀 서비스, 고급 기능

Q: GPS 없이도 사용할 수 있나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 네. GPS 없이 PCD 맵 기반 자율주행이 가능합니다.

.. code-block:: cpp

    // PCD 맵 기반 (GPS 불필요)
    client.setMap("office.pcd", 0, 0, 0, 0, "office");
    Waypoint("office", 5.0, 3.0);  // 맵 좌표

GPS를 사용하려면 로봇에 GPS 모듈이 장착되어 있어야 합니다.

Q: 여러 로봇을 동시에 제어할 수 있나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 네. 각 로봇에 대해 별도의 ``RaisinClient`` 인스턴스를 생성하면 됩니다.

.. code-block:: cpp

    raisin_sdk::RaisinClient client1("controller1");
    raisin_sdk::RaisinClient client2("controller2");

    client1.connect("robot1_id");
    client2.connect("robot2_id");

Waypoint
--------

Q: Waypoint frame은 왜 중요한가요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** frame은 좌표계를 지정합니다. ``setMap()`` 에서 설정한 ``map_name`` 과
Waypoint의 ``frame`` 이 일치해야 로봇이 올바른 위치로 이동합니다.

.. code-block:: cpp

    // map_name = "floor1" 으로 설정했다면
    client.setMap("map.pcd", 0, 0, 0, 0, "floor1");

    // Waypoint도 "floor1" frame 사용
    Waypoint("floor1", 10.0, 5.0);  // OK

    // 다른 frame 사용 시 동작 안함
    Waypoint("map", 10.0, 5.0);     // Wrong

Q: 무한 순찰은 어떻게 설정하나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``repetition`` 을 0으로 설정하거나 ``startPatrol()`` 을 사용합니다.

.. code-block:: cpp

    // 방법 1
    client.setWaypoints(waypoints, 0);  // repetition = 0

    // 방법 2
    client.startPatrol(waypoints);

Q: 순찰 중 Waypoint를 추가할 수 있나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 네. ``appendWaypoint()`` 를 사용합니다.

.. code-block:: cpp

    // 기존 미션에 새 waypoint 추가
    client.appendWaypoint(raisin_sdk::Waypoint("my_map", 15.0, 10.0));

제어
----

Q: 수동/자율 제어 전환은 어떻게 하나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``setManualControl()`` 과 ``setAutonomousControl()`` 을 사용합니다.

.. code-block:: cpp

    // 수동 제어 (조이스틱)
    client.setManualControl();

    // 자율주행 제어
    client.setAutonomousControl();
    client.setWaypoints(waypoints, 1);

Q: 긴급 정지는 어떻게 하나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``stopNavigation()`` 으로 자율주행을 중지합니다.

.. code-block:: cpp

    // 자율주행 중지
    client.stopNavigation();

실제 로봇 긴급 정지는 조이스틱의 비상 정지 버튼을 사용하세요.

데이터
------

Q: Odometry 데이터는 어떤 좌표계인가요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 맵이 로드된 경우 맵 기준 좌표계, 그렇지 않으면 시작 위치 기준 odom 좌표계입니다.

Q: PointCloud 데이터 형식은?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``Point3D`` 구조체 벡터로 제공됩니다.

.. code-block:: cpp

    struct Point3D { float x, y, z; };

    client.subscribePointCloud([](const std::vector<raisin_sdk::Point3D>& points) {
        for (const auto& p : points) {
            // p.x, p.y, p.z 사용
        }
    });

Q: 배터리 잔량은 어떻게 확인하나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** ``subscribeRobotState()`` 로 배터리 정보를 구독합니다.

.. code-block:: cpp

    client.subscribeRobotState([](const raisin_sdk::ExtendedRobotState& state) {
        double percentage = 0.0;
        if (state.max_voltage > state.min_voltage) {
            percentage = (state.voltage - state.min_voltage) /
                        (state.max_voltage - state.min_voltage) * 100.0;
        }
        std::cout << "Battery: " << percentage << "%" << std::endl;
    });

호환성
------

Q: 어떤 로봇을 지원하나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** Raibo2 플랫폼을 지원합니다. Autonomy 플러그인과 Fast-LIO 플러그인이 로드되어 있어야 합니다.

Q: Windows에서 사용할 수 있나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 현재 Ubuntu Linux만 공식 지원합니다. Windows 지원은 추후 검토 예정입니다.

Q: ARM64 (Jetson 등)에서 사용할 수 있나요?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**A:** 네. x86_64와 arm64 아키텍처 모두 지원합니다.
