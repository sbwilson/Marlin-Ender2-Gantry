### Problem Outline: Differential Drive for Tilt and Rotation

Marlin firmware, at its core, is designed to interpret G-code commands (like `G0` for rapid move and `G1` for linear interpolation) to control Cartesian axes (X, Y, Z) and an extruder (E). The `I` and `J` parameters in G-code are typically unused or have specific, non-motion-related functions depending on the G-code command.

The problem is to repurpose `G0` and `G1` commands, specifically utilizing their `I` and `J` parameters, to control a new "differential drive" mechanism. This mechanism will translate `I` into a **tilt angle** and `J` into a **rotation angle**. This requires:

1.  **G-code Parsing:** Modifying Marlin's G-code interpreter to recognize and extract `I` and `J` values when present in `G0` or `G1` commands.
2.  **Kinematic Translation:** Developing a new kinematic model that converts the desired tilt (`I`) and rotation (`J`) angles into appropriate step commands for two independent motors (let's call them M1 and M2) that constitute the differential drive.
3.  **Angle Limiting and Wrapping:** Implementing logic to enforce physical limits on the tilt angle (0-70 degrees) and handle rotation angle limits (-180 to 180 degrees) with an optional "soft limit" feature that allows wrapping around the 180/-180 boundary while always ensuring the shortest path is taken.

### Mathematical Derivation Summary

Assume a differential drive system controlled by two motors, M1 and M2. These motors are mechanically arranged such that:
*   When M1 and M2 move in the **same direction** (e.g., both forward), the system primarily undergoes **tilt**.
*   When M1 and M2 move in **opposite directions** (e.g., M1 forward, M2 backward), the system primarily undergoes **rotation**.

Let:
*   `I_target` be the desired target tilt angle (in degrees).
*   `J_target` be the desired target rotation angle (in degrees).
*   `M1_steps` be the required steps for Motor 1.
*   `M2_steps` be the required steps for Motor 2.

The relationship between the desired angles and the motor steps can be expressed as:

`M1_steps = (K_tilt * I_target) + (K_rot * J_target)`
`M2_steps = (K_tilt * I_target) - (K_rot * J_target)`

Where:
*   `K_tilt`: A scaling constant that converts tilt degrees into motor steps. This factor depends on the mechanical design (e.g., gear ratios, arm lengths) and the steps-per-degree of the tilt mechanism.
*   `K_rot`: A scaling constant that converts rotation degrees into motor steps. This factor depends on the mechanical design and the steps-per-degree of the rotation mechanism.

These `K` values are specific to the hardware and would typically be defined in Marlin's configuration files (e.g., `Configuration.h`).

### Angle Limits and Rotation Wrapping Logic

1.  **Tilt Angle (I) Limits:**
    *   The `I_target` value must be constrained within `0` and `70` degrees. If a G-code command requests an `I` outside this range, it should be clamped to the nearest valid limit (0 or 70).

2.  **Rotation Angle (J) Limits:**
    *   The primary range for `J_target` is `-180` to `180` degrees.

    *   **Soft Limits Disabled (Hard Stop):** If a configuration parameter (e.g., `DIFFERENTIAL_DRIVE_SOFT_LIMITS`) is disabled, any `J_target` outside the `-180` to `180` range will be clamped to `-180` or `180` degrees.

    *   **Soft Limits Enabled (Wrap Around & Shortest Path):** If `DIFFERENTIAL_DRIVE_SOFT_LIMITS` is enabled, `J_target` values will wrap around the `-180`/`180` boundary. Crucially, the system must always calculate the shortest angular path to the new `J_target` and never cross the `180` degree point in a way that would unwind cables.

        Let `J_current` be the current rotation angle and `J_target_raw` be the angle from the G-code.

        First, normalize `J_target_raw` to the `-180` to `180` range:
        `J_target_normalized = fmod(J_target_raw + 180, 360) - 180;`
        (This `fmod` operation handles both positive and negative wrapping.)

        Next, calculate the shortest angular delta (`J_delta`) from `J_current` to `J_target_normalized`:
        `J_delta = J_target_normalized - J_current;`

        To ensure the shortest path across the `180/-180` boundary:
        `if (J_delta > 180) { J_delta -= 360; }`
        `else if (J_delta < -180) { J_delta += 360; }`

        The final `J_target` for the kinematic calculation will then be `J_current + J_delta`. This ensures that even if the raw target is, say, 200 degrees and the current is 170 degrees, the system moves -160 degrees (170 -> 180 -> -180 -> -160) instead of 30 degrees (170 -> 200), which would be the longer path.

### Implementation Details:

*   **Homing:**
    *   The `I` (tilt) axis will home to 0 degrees using an endstop.
    *   The `J` (rotation) axis will not home for now.
*   **Motor Drivers:**
    *   The `I` axis will use the `E0` stepper motor driver.
    *   The `J` axis will use the `E1` stepper motor driver.
*   **Kinematic Constants (`K_TILT`, `K_ROT`):**
    *   These values will be derived from the degrees per step of the steppers and the gear ratios of the mechanical system. Placeholders will be used initially in the configuration files.

---

**Implementation Task List for Differential Drive**

This task list outlines the steps to integrate the differential drive (tilt `I` and rotation `J`) into Marlin firmware.

**Phase 1: Configuration and Data Structures**

*   **Task 1.1: Add Differential Drive Configuration to `Marlin/Configuration_adv.h`**
    *   Append a new section `@section differential_drive` to the end of `Marlin/Configuration_adv.h`.
    *   Define `K_TILT_STEPS_PER_DEGREE` and `K_ROT_STEPS_PER_DEGREE` (float values).
    *   Define `DIFFERENTIAL_DRIVE_SOFT_LIMITS` (boolean).
    *   *Note: This task has already been completed in the previous turn.*

*   **Task 1.2: Enable Differential Drive and Map Drivers in `Marlin/Configuration.h`**
    *   Add `#define DIFFERENTIAL_DRIVE` near other kinematics definitions.
    *   Modify `E0_DRIVER_TYPE` and `E1_DRIVER_TYPE` to be conditionally mapped to `I_DRIVER_TYPE` and `J_DRIVER_TYPE` respectively, only when `DIFFERENTIAL_DRIVE` is enabled. This will involve using `#if ENABLED(DIFFERENTIAL_DRIVE)` blocks.
    *   Set `I_MIN_POS`, `I_MAX_POS`, `J_MIN_POS`, `J_MAX_POS`.
    *   Update `DEFAULT_AXIS_STEPS_PER_UNIT`, `DEFAULT_MAX_FEEDRATE`, and `DEFAULT_MAX_ACCELERATION` to include `I` and `J` axes.

*   **Task 1.3: Extend Position Data Structures (`Marlin/src/inc/MarlinConfig.h`)**
    *   Locate the `xyze_pos_t` (or similar) structure.
    *   Add `i` and `j` members of type `float` to this structure.
    *   Ensure any related types (e.g., `xyze_long_t`) are also updated if necessary.

**Phase 2: G-code Parsing**

*   **Task 2.1: Parse I and J Parameters in `Marlin/src/gcode/motion/G0_G1.cpp`**
    *   Inside `GcodeSuite::G0_G1()`, after parsing `XYZ` and `E`, add logic to parse `I` and `J` values.
    *   Use `parser.floatval('I')` and `parser.floatval('J')` to retrieve floating-point values.
    *   Store these parsed values into the extended `destination` structure.

**Phase 3: Kinematics and Motion Planning**

*   **Task 3.1: Implement Kinematics in `Marlin/src/module/planner.cpp`**
    *   Within `planner.buffer_line()`, wrap the differential drive logic in an `#if ENABLED(DIFFERENTIAL_DRIVE)` block.
    *   Implement tilt angle clamping for `I_target` using `constrain(target_I, I_MIN_POS, I_MAX_POS)`.
    *   Implement rotation wrapping and shortest path logic for `J_target`, conditional on `DIFFERENTIAL_DRIVE_SOFT_LIMITS`.
        *   Calculate `J_target_normalized` using `fmod`.
        *   Calculate `J_delta` and adjust for shortest path across the `180/-180` boundary.
        *   Update `J_target` for kinematic calculation.
    *   Calculate `E0_steps` and `E1_steps` using `K_TILT_STEPS_PER_DEGREE`, `K_ROT_STEPS_PER_DEGREE`, `I_target`, and `J_target`.
    *   Adjust overall feedrate and `segment_time` to synchronize `IJ` movement with `XYZ` movements, considering the "longest" move in steps.

**Phase 4: Homing**

*   **Task 4.1: Implement I-axis Homing in `Marlin/src/gcode/motion/G28.cpp`**
    *   In `GcodeSuite::G28()`, add conditional logic to home the `I` axis if commanded.
    *   Implement the homing procedure for the `I` axis to its 0-degree endstop (move towards endstop, trigger, back off, slow re-home).
    *   Explicitly ensure no homing logic is added for the `J` axis.

**Phase 5: Motor Control Verification**

*   **Task 5.1: Verify Stepper Motor Control (`Marlin/src/module/stepper.cpp`)**
    *   Review `stepper.cpp` to confirm that step generation for `E0` and `E1` (now used for `I` and `J`) is handled correctly by Marlin's existing stepper logic. No significant code changes are expected here unless specific low-level driver configurations are needed.
