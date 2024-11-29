#if 1
#include "App_Dino.h"
#include "../../../ChappieBsp/Chappie.h"
#include "Dino/include/Game.h"

static std::string app_name = "Dino";
static CHAPPIE* device;

LV_IMG_DECLARE(ui_img_icon_dinogame_png);


Game Dino;


namespace App {

    /**
     * @brief Return the App name laucnher, which will be show on launcher App list
     * 
     * @return std::string 
     */
    std::string App_Dino_appName()
    {
        return app_name;
    }


    /**
     * @brief Return the App Icon laucnher, NULL for default
     * 
     * @return void* 
     */
    void* App_Dino_appIcon()
    {
        // return NULL;
        return (void*)&ui_img_icon_dinogame_png;
    }


    /**
     * @brief Called when App is on create
     * 
     */
    void App_Dino_onCreate()
    {
        UI_LOG("[%s] onCreate\n", App_Dino_appName().c_str());

        device->Lcd.clear();
        lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);
        Dino.begin(device);
    }


    /**
     * @brief Called repeatedly, end this function ASAP! or the App management will be affected
     * If the thing you want to do takes time, try create a taak or lvgl timer to handle them.
     * Try use millis() instead of delay() here
     * 
     */
    void App_Dino_onLoop()
    {
        Dino.loop();
    }


    /**
     * @brief Called when App is about to be destroy
     * Please remember to release the resourse like lvgl timers in this function
     * 
     */
    void App_Dino_onDestroy()
    {
        UI_LOG("[%s] onDestroy\n", App_Dino_appName().c_str());
    }


    /**
     * @brief Launcher will pass the BSP pointer through this function before onCreate
     * 
     */
    void App_Dino_getBsp(void* bsp)
    {
        device = (CHAPPIE*)bsp;
    }
    
}

#endif
