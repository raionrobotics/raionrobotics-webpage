Troubleshooting
===============

일반적인 문제와 해결 방법입니다.

연결 문제
---------

로봇에 연결되지 않음
^^^^^^^^^^^^^^^^^^^^

**증상:**

* ``connect()`` 가 false 반환
* "Connection failed" 오류

**해결 방법:**

1. **네트워크 확인**

   .. code-block:: bash

       # 로봇 IP로 핑 테스트
       ping 192.168.1.xxx

2. **Robot ID 확인**

   * raisin_master 콘솔 또는 raisin_gui에서 Robot ID 확인
   * Robot ID가 정확히 일치하는지 확인

3. **멀티캐스트 확인**

   * 로봇과 클라이언트가 동일 서브넷에 있어야 함
   * 공유기/스위치가 멀티캐스트를 차단하지 않는지 확인

4. **방화벽 확인**

   .. code-block:: bash

       # Ubuntu에서 방화벽 상태 확인
       sudo ufw status

       # 필요시 비활성화
       sudo ufw disable

서비스 타임아웃
^^^^^^^^^^^^^^^

**증상:**

* ``setWaypoints()`` 등이 "service timeout" 반환
* 서비스 호출이 실패

**해결 방법:**

1. **Autonomy 플러그인 확인**

   * raisin_master에서 Autonomy 플러그인이 로드되었는지 확인
   * Fast-LIO 플러그인도 로드되어야 함 (맵 설정 시)

2. **연결 상태 확인**

   .. code-block:: cpp

       if (!client.isConnected()) {
           std::cerr << "Not connected!" << std::endl;
       }

3. **타임아웃 증가** (필요시)

   네트워크 지연이 큰 환경에서는 서비스 응답이 늦을 수 있습니다.

맵 로드 문제
------------

"Map saved." 대신 "service timeout"
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**원인:**

* Fast-LIO 플러그인이 로드되지 않음

**해결 방법:**

1. raisin_master에서 Fast-LIO 플러그인 로드 확인
2. LiDAR가 정상 작동하는지 확인
3. 로봇에서 ``/cloud_registered`` 토픽이 발행되는지 확인

PCD 파일을 찾을 수 없음
^^^^^^^^^^^^^^^^^^^^^^^

**원인:**

* 파일 경로가 잘못됨

**해결 방법:**

1. **절대 경로 사용 권장**

   .. code-block:: cpp

       // 상대 경로 대신 절대 경로 사용
       client.setMap("/home/user/maps/office.pcd", ...);

2. **파일 존재 확인**

   .. code-block:: bash

       ls -la /path/to/map.pcd

Waypoint 문제
-------------

로봇이 움직이지 않음
^^^^^^^^^^^^^^^^^^^^

**증상:**

* Waypoint 설정 성공했지만 로봇이 이동하지 않음
* 경로가 생성되지 않음

**해결 방법:**

1. **frame 이름 확인** (가장 흔한 원인)

   .. code-block:: cpp

       // setMap의 map_name과 Waypoint의 frame이 일치해야 함!
       client.setMap("...", 0, 0, 0, 0, "my_map");
       Waypoint("my_map", 5.0, 0.0);  // ✓ 동일

       // 틀린 예시
       client.setMap("...", 0, 0, 0, 0, "my_map");
       Waypoint("map", 5.0, 0.0);     // ✗ 불일치!

2. **Localization 상태 확인**

   * 맵이 제대로 로드되었는지 확인
   * 로봇 위치가 맵 내에 정확히 설정되었는지 확인

3. **Autonomy 모드 확인**

   * ``setAutonomousControl()`` 이 호출되었는지 확인

4. **미션 상태 확인**

   .. code-block:: cpp

       auto status = client.getMissionStatus();
       std::cout << "Waypoints: " << status.waypoints.size() << std::endl;

좌표가 맞지 않음
^^^^^^^^^^^^^^^^

**증상:**

* 로봇이 예상과 다른 위치로 이동
* Waypoint 좌표가 맵과 일치하지 않음

**해결 방법:**

1. **초기 위치 확인**

   * ``setMap()`` 에서 설정한 초기 위치가 실제 로봇 위치와 일치하는지 확인
   * 초기 yaw(방향)도 정확해야 함

2. **좌표계 확인**

   * Waypoint 좌표는 맵 기준 좌표 (맵 원점 기준)
   * 미터 단위 사용

구독 문제
---------

콜백이 호출되지 않음
^^^^^^^^^^^^^^^^^^^^

**증상:**

* ``subscribeOdometry()`` 등 콜백이 실행되지 않음

**해결 방법:**

1. **연결 상태 확인**

   * 로봇에 정상 연결되었는지 확인

2. **메인 루프 유지**

   .. code-block:: cpp

       // 프로그램이 종료되지 않도록 메인 루프 유지
       while (running) {
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
       }

3. **토픽 발행 확인**

   * raisin_cli로 해당 토픽이 발행되는지 확인

빌드 문제
---------

라이브러리를 찾을 수 없음
^^^^^^^^^^^^^^^^^^^^^^^^^

**증상:**

* 링크 에러: "cannot find -lraisin_network"

**해결 방법:**

1. **경로 확인**

   .. code-block:: cmake

       # RAISIN_SDK_PATH가 올바른지 확인
       message(STATUS "SDK Path: ${RAISIN_SDK_PATH}")

2. **라이브러리 존재 확인**

   .. code-block:: bash

       ls -la /path/to/raisin_sdk_install/lib/

실행 시 라이브러리 로드 실패
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**증상:**

* "error while loading shared libraries"

**해결 방법:**

.. code-block:: bash

    # LD_LIBRARY_PATH 설정
    export LD_LIBRARY_PATH=/path/to/raisin_sdk_install/lib:$LD_LIBRARY_PATH

    # 또는 ldconfig 사용
    sudo ldconfig /path/to/raisin_sdk_install/lib

도움 요청
---------

문제가 해결되지 않으면:

1. 에러 메시지 전체 내용 기록
2. 사용 중인 코드 스니펫 준비
3. 네트워크 구성 정보 확인
4. raisin_master 콘솔 로그 확인
