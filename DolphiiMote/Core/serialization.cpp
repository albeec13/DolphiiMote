// Copyright 2013 Max Malmgren

// This file is part of DolphiiMote.

// DolphiiMote is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// DolphiiMote is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with DolphiiMote.  If not, see <http://www.gnu.org/licenses/>.

#include "serialization.h"
namespace dolphiimote { namespace serialization {

    std::array<u8, 23> _start_rumble = { 0xA2, 0x15, 0x01 };
    std::array<u8, 23> _stop_rumble = { 0xA2, 0x15, 0x00 };

    const std::array<u8, 23>& start_rumble()
    {
      return _start_rumble;
    }

    const std::array<u8, 23>& stop_rumble()
    {
      return _stop_rumble;
    }

    size_t rumble_size()
    {
      return 3;
    }

  void retrieve_motion_plus(checked_array<const u8> extension_data, dolphiimote_wiimote_data &output)
  {
    u8 speed_mask = ~0x03;

    if(extension_data.size() >= 6 && (extension_data[5] & 0x02))
    {
      output.valid_data_flags |= dolphiimote_MOTIONPLUS_VALID;

      output.motionplus.yaw_down_speed = extension_data[0] + ((u16)(extension_data[3] & speed_mask) << 6);
      output.motionplus.roll_left_speed = extension_data[1] + ((u16)(extension_data[4] & speed_mask) << 6);
      output.motionplus.pitch_left_speed = extension_data[2] + ((u16)(extension_data[5] & speed_mask) << 6);

      output.motionplus.slow_modes = (extension_data[3] & 0x03) << 1 | (extension_data[4] & 0x02) >> 1;
      output.motionplus.extension_connected = extension_data[4] & 0x01;
    }
  }

  void retrieve_nunchuck(checked_array<const u8> extension_data, dolphiimote_wiimote_data &output)
  {
    if(extension_data.size() >= 6)
    {    
      output.valid_data_flags |= dolphiimote_NUNCHUCK_VALID;

      output.nunchuck.stick_x = extension_data[0];
      output.nunchuck.stick_y = extension_data[1];
      output.nunchuck.x = (extension_data[2] << 2) | (extension_data[5] & 0x0C) >> 2;
      output.nunchuck.y = (extension_data[3] << 2) | (extension_data[5] & 0x30) >> 4;
      output.nunchuck.z = (extension_data[4] << 2) | (extension_data[5] & 0xC0) >> 6;

      output.nunchuck.buttons = ~(extension_data[5]) & 0x3;
    }
  }

  void retrieve_interleaved_nunchuck(checked_array<const u8> extension_data, dolphiimote_wiimote_data &output)
  {
    if(extension_data.size() >= 6 && !(extension_data[5] & 0x03))
    {    
      output.valid_data_flags |= dolphiimote_NUNCHUCK_VALID;

      output.nunchuck.stick_x = extension_data[0];
      output.nunchuck.stick_y = extension_data[1];
      output.nunchuck.x = (extension_data[2] << 1) | (extension_data[5] & 0x10) >> 4;
      output.nunchuck.y = (extension_data[3] << 1) | (extension_data[5] & 0x20) >> 5;
      output.nunchuck.z = ((extension_data[4] & ~0x1) << 1 ) | (extension_data[5] & 0xC0) >> 6;

      output.nunchuck.buttons = ~(extension_data[5] >> 2) & 0x3;
    }
  }

  void retrieve_classic_controller(checked_array<const u8> extension_data, dolphiimote_wiimote_data &output)
  {
	  if(extension_data.size() >= 6)
    { 	  
      output.valid_data_flags |= dolphiimote_CLASSIC_CONTROLLER_VALID;

      output.classic_controller.left_stick_x = extension_data[0] & 0x3F;
      output.classic_controller.left_stick_y = extension_data[1] & 0x3F;

      output.classic_controller.right_stick_x = ((extension_data[0] & 0xC0) >> 3) | ((extension_data[1] & 0xC0) >> 5) | ((extension_data[2] & 0xC0) >> 7);
      output.classic_controller.right_stick_y = extension_data[2] & 0x1F;

      output.classic_controller.left_trigger = ((extension_data[2] & 0x60) >> 2) | ((extension_data[3] & 0xE0) >> 5);
      output.classic_controller.right_trigger = extension_data[3] & 0x1F;

      output.classic_controller.buttons = ~((extension_data[4] << 8) | extension_data[5]);
    }
  }

  void retrieve_interleaved_classic_controller(checked_array<const u8> extension_data, dolphiimote_wiimote_data &output)
  {
	  if(extension_data.size() >= 6)
    {
      if((extension_data[5] & 0x03) == 0)
      {
        output.valid_data_flags |= dolphiimote_CLASSIC_CONTROLLER_VALID;

        output.classic_controller.left_stick_x = extension_data[0] & 0x3E;
        output.classic_controller.left_stick_y = extension_data[1] & 0x3E;

        output.classic_controller.right_stick_x = ((extension_data[0] & 0xC0) >> 3) | ((extension_data[1] & 0xC0) >> 5) | ((extension_data[2] & 0xC0) >> 7);
        output.classic_controller.right_stick_y = extension_data[2] & 0x1F;

        output.classic_controller.left_trigger = ((extension_data[2] & 0x60) >> 2) | ((extension_data[3] & 0xE0) >> 5);
        output.classic_controller.right_trigger = extension_data[3] & 0x1F;

        output.classic_controller.buttons = ~(((extension_data[4] & 0xFE ) << 8) | (extension_data[5] & 0xFC) | ((extension_data[1] & 0x01) << 1) | (extension_data[0] & 0x01));
      }
    }
  }

  void retrieve_button_state(u8 reporting_mode, checked_array<const u8> data, dolphiimote_wiimote_data &output)
  {
    if(data.size() > 4)
    {
      u8 first = data[2];
      u8 second = data[3];

      output.button_state = first << 8 | second;
    }
  }

  void retrieve_infrared_camera_data(u8 reporting_mode, checked_array<const u8> data, dolphiimote_wiimote_data &output)
  {
    
  }

  void retrieve_acceleration_data(u8 reporting_mode, checked_array<const u8> data, struct dolphiimote_wiimote_data &output)
  {
    if(data.size() > 7)
    {    
      output.valid_data_flags |= dolphiimote_ACCELERATION_VALID;

      output.acceleration.x = (data[4] << 2) | (data[2] >> 5 & 0x3);
      output.acceleration.y = (data[5] << 2) | (data[3] >> 4 & 0x2);
      output.acceleration.z = (data[6] << 2) | (data[3] >> 5 & 0x2);
    }
  }
  }
}