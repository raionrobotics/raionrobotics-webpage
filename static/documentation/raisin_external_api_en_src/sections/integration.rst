Integration Package
===================

Package Contents
----------------

The ``raisin_sdk_install/`` folder contains:

.. code-block:: text

    raisin_sdk_install/
    ├── include/                    # Header files
    │   ├── raisin_network/         # Network library
    │   ├── raisin_interfaces/      # Message/service definitions
    │   └── ...
    └── lib/                        # Compiled libraries
        ├── libraisin_network.so
        ├── libraisin_interfaces.so
        └── ...

System Requirements
-------------------

.. code-block:: bash

    # Ubuntu 22.04
    sudo apt-get install libeigen3-dev libssl-dev libpcl-dev libopencv-dev

Network Requirements
--------------------

- Robot and client must be on the **same network**
- Multicast traffic must be allowed (for Robot ID discovery)
- Firewall: Allow TCP/UDP ports 8080, 38371, etc.

Finding Robot ID
----------------

Robot ID is printed in the ``raisin_master`` console on startup:

.. code-block:: text

    [raisin_master] Robot ID: railab_raibo-xxx
    [raisin_master] Listening on port 38371

It can also be found in raisin_gui.
