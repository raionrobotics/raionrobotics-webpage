GUI Demo (simple_gui)
=====================

OpenCV 기반의 시각화 데모 프로그램입니다. 맵과 로봇 위치를 시각화하고, 마우스로 Waypoint를 설정할 수 있습니다.

실행 방법
---------

.. code-block:: bash

    ./simple_gui <robot_id> [pcd_path]

    # 예시
    ./simple_gui railab_raibo-xxx
    ./simple_gui railab_raibo-xxx ../maps/office1_example.pcd

워크플로우
----------

1. ``O`` 키로 PCD 맵 파일 열기
2. 맵에서 클릭하여 로봇 초기 위치 설정
3. 로봇이 바라보는 방향으로 클릭하여 방향(Yaw) 설정
4. ``S`` 키로 맵 전송 및 Localization 시작
5. 맵에서 클릭하여 Waypoint 추가
6. ``W`` 키로 Waypoint 전송 및 자율주행 시작

키보드 단축키
-------------

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
-----------

- **좌클릭**: 워크플로우 단계별 동작 (위치/방향/Waypoint)
- **우클릭 드래그**: 화면 이동 (Pan)
- **스크롤**: 줌

상태 표시
---------

- 화면 하단에 현재 단계 및 다음 동작 안내 표시
- Localization 활성화 전에는 Waypoint 추가 불가
- 이미 Localization이 활성화된 상태에서 ``S`` 를 누르면 리셋 안내

.. note::
    simple_gui는 ``zenity`` 패키지를 사용하여 파일 선택 다이얼로그를 표시합니다.
    Ubuntu에서 ``sudo apt-get install zenity`` 로 설치할 수 있습니다.
