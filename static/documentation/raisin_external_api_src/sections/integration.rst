Integration Package
===================

패키지 구성
-----------

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
---------------

.. code-block:: bash

    # Ubuntu 22.04
    sudo apt-get install libeigen3-dev libssl-dev libpcl-dev libopencv-dev

네트워크 요구사항
-----------------

- 로봇과 클라이언트가 **동일 네트워크** 에 있어야 합니다
- 멀티캐스트 트래픽이 허용되어야 합니다 (Robot ID 자동 발견용)
- 방화벽: TCP/UDP 포트 8080, 38371 등 허용 필요

Robot ID 확인
-------------

Robot ID는 ``raisin_master`` 실행 시 콘솔에 출력됩니다:

.. code-block:: text

    [raisin_master] Robot ID: railab_raibo-xxx
    [raisin_master] Listening on port 38371

또는 raisin_gui에서 확인할 수 있습니다.
