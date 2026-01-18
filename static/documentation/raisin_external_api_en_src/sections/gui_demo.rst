GUI Demo (simple_gui)
=====================

An OpenCV-based visualization demo program. It visualizes maps and robot positions, allowing you to set waypoints with mouse clicks.

Usage
-----

.. code-block:: bash

    ./simple_gui <robot_id> [pcd_path]

    # Examples
    ./simple_gui railab_raibo-xxx
    ./simple_gui railab_raibo-xxx ../maps/office1_example.pcd

Workflow
--------

1. Press ``O`` to open a PCD map file
2. Click on map to set robot initial position
3. Click in the direction the robot is facing to set yaw
4. Press ``S`` to send map and start localization
5. Click on map to add waypoints
6. Press ``W`` to send waypoints and start navigation

Keyboard Shortcuts
------------------

.. list-table::
   :header-rows: 1
   :widths: 15 85

   * - Key
     - Function
   * - ``O``
     - Open PCD map file
   * - ``S``
     - Send map (start localization)
   * - ``W``
     - Send waypoints (start navigation)
   * - ``C``
     - Clear waypoints
   * - ``R``
     - Toggle repetition (1 → 5 → ∞)
   * - ``X``
     - Reset initial pose
   * - ``+/-``
     - Zoom in/out
   * - ``Q``
     - Quit

Mouse Controls
--------------

- **Left-click**: Step-by-step workflow action (position/direction/waypoint)
- **Right-drag**: Pan view
- **Scroll**: Zoom

Status Display
--------------

- Current step and next action guide displayed at bottom of screen
- Waypoints cannot be added before localization is active
- Pressing ``S`` when localization is already active shows reset instructions

.. note::
    simple_gui uses the ``zenity`` package for file selection dialogs.
    Install with ``sudo apt-get install zenity`` on Ubuntu.
