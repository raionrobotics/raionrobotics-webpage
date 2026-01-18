Overview
========

Raisin 시스템은 외부 연동을 위해 두 가지 방식을 제공합니다:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - 방식
     - 설명
     - 용도
   * - **API Example**
     - ``RaisinClient`` 래퍼 클래스 예제 코드
     - 빠른 시작, 참고용 구현체
   * - **Direct Network**
     - ``raisin_network`` 라이브러리 직접 사용
     - 커스텀 서비스 호출, 세밀한 제어 필요 시

.. note::
    **API Example (RaisinClient)** 는 공식 지원 SDK가 아닌 예제 코드입니다.
    외부 개발자가 Raisin 플랫폼을 연동하는 방법을 보여주는 참고용 구현체이며,
    필요에 따라 수정하여 사용하시기 바랍니다.

Architecture
------------

.. code-block:: text

    +----------------------------------------------+
    |            External Application              |
    +----------------------------------------------+
    |  Option A: API Example                       |
    |  +----------------------------------------+  |
    |  |  #include "raisin_client.hpp"          |  |
    |  |  RaisinClient client;                  |  |
    |  |  client.connect("ROBOT_ID");           |  |
    |  |  client.setMap(...);                   |  |
    |  |  client.setWaypoints(...);             |  |
    |  +----------------------------------------+  |
    +----------------------------------------------+
    |  Option B: Direct Network                    |
    |  +----------------------------------------+  |
    |  |  raisin::Network network;              |  |
    |  |  raisin::Node node;                    |  |
    |  |  auto client = node.createClient       |  |
    |  |      <SetWaypoints>(...);              |  |
    |  +----------------------------------------+  |
    +----------------------------------------------+
                        |
                        | WebSocket
                        v
    +----------------------------------------------+
    |                   Raibo2                     |
    |  +------------------+  +------------------+  |
    |  | Autonomy Plugin  |  | Fast-LIO Plugin  |  |
    |  | - Waypoints      |  | - Localization   |  |
    |  | - Navigation     |  | - Map Loading    |  |
    |  +------------------+  +------------------+  |
    +----------------------------------------------+
