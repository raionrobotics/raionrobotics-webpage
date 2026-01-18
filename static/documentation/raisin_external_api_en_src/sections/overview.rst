Overview
========

Raisin provides two integration methods for external systems:

.. list-table::
   :header-rows: 1
   :widths: 20 40 40

   * - Method
     - Description
     - Use Case
   * - **API Example**
     - ``RaisinClient`` wrapper class example code
     - Quick start, reference implementation
   * - **Direct Network**
     - Direct use of ``raisin_network`` library
     - Custom service calls, fine-grained control

.. note::
    **API Example (RaisinClient)** is example code, not an officially supported SDK.
    It is a reference implementation showing how to integrate with the Raisin platform.
    Modify it as needed for your use case.

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
