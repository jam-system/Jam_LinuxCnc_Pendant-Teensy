#pragma once

#include <Arduino.h>

/**
 * @brief Valid values for cmd[0]
 * Do not use enum class
 */
enum Cmd_e : uint8_t {
  CMD_ABSOLUTE_POS = 'A', // Absolute position
  CMD_SPINDLE_SPEED = 'S', //Commanded spindle speed (not RPM unless override is 100%) IN/OUT
  CMD_SPINDLE_OVERRIDE = 's', //Spindle override - combined with speed results in RPM IN/OUT
  CMD_SPINDLE_RPM = 'R', //Actual RPM of spindle as a result of SPINDLE_SPEED * OVERRIDE IN
  CMD_SPINDLE_DIRECTION = 'G', //Spindle direction IN
  CMD_FEED_OVERRIDE = 'f', //Feed override IN/OUT
  CMD_RAPID_OVERRIDE = 'r', //Rapid override IN/OUT
  CMD_JOG = 'J', //Jog
  CMD_JOG_VELOCITY = 'j', //Jog Velocity IN/OUT (sorta)
  CMD_JOG_CONTINUOUS = 'N', //Jog continuous (Nudge) payload is velocity
  CMD_JOG_STOP = 'n', //Jog stop (don't nudge)
  CMD_TASK_MODE = 'M', //mode IN/OUT
  CMD_TASK_STATE = 'E', //task_state (Estop and On/Off) IN/OUT
  CMD_INTERP_STATE = 'I', //interp_state
  CMD_CURRENT_VEL = 'v', //current_vel
  CMD_MOTION_TYPE = 't', //motion_type
  CMD_G5X_INDEX = 'W', //g5x_index (WCS) IN OUT
  CMD_G5X_OFFSET = '5', //g5x_offset IN
  CMD_G92_OFFSET = '9', //g92_offset IN
  CMD_TOOL_OFFSET = 'T', //Tool_offset IN: offset OUT: tool index?
  CMD_DTG = 'D', //DTG
  CMD_ALL_HOMED = 'H', //homed
  CMD_HOMED = 'h', //homed
  CMD_INI_VALUE = 'i', //ini value
  CMD_FLOOD = 'C', //Coolant IN/OUT
  CMD_MIST = 'c', //Little Coolant IN/OUT
  CMD_EXEC_STATE = 'e', 
  CMD_PROGRAM_STATE = 'p',
  CMD_AUTO = 'a',
  CMD_HEARTBEAT = 'b' //Heartbeat
};