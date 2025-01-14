/*----------------------------------------------------------------------------/
  Lovyan GFX - Graphics library for embedded devices.

Original Source:
 https://github.com/lovyan03/LovyanGFX/

Licence:
 [FreeBSD](https://github.com/lovyan03/LovyanGFX/blob/master/license.txt)

Author:
 [lovyan03](https://twitter.com/lovyan03)

Contributors:
 [ciniml](https://github.com/ciniml)
 [mongonta0716](https://github.com/mongonta0716)
 [tobozo](https://github.com/tobozo)
 [imliubo](https://github.com/imliubo)
/----------------------------------------------------------------------------*/
#pragma once

#include "../v1_init.hpp"
#include "common.hpp"
#include "../v1/platforms/framebuffer/Panel_fb.hpp"

namespace lgfx
{
 inline namespace v1
 {
//----------------------------------------------------------------------------

  class LGFX : public LGFX_Device
  {
    lgfx::Panel_fb _panel_instance;

    bool init_impl(bool use_reset, bool use_clear)
    {
      return LGFX_Device::init_impl(false, use_clear);
    }

  public:

    /// create instance.
    /// @param width display width.
    /// @param height display height.
    /// @param device_name Specify the path (e.g., "/dev/fb1") or device name (e.g., "st7789") of the device to be operated.
    LGFX(int width = 320, int height = 240, const char* device_name = "/dev/fb0")
    {
      auto cfg = _panel_instance.config();
      cfg.memory_width = width;
      cfg.panel_width = width;
      cfg.memory_height = height;
      cfg.panel_height = height;
      _panel_instance.config(cfg);
      _panel_instance.setDeviceName(device_name);
      setPanel(&_panel_instance);
      _board = board_t::board_FrameBuffer;
    }
  };

//----------------------------------------------------------------------------
 }
}

using LGFX = lgfx::LGFX;