#if 1
#pragma once
#include "../AppTypedef.h"
#include "../../ChappieUIConfigs.h"
#include "../../../ChappieBsp/Chappie.h"
#include <FS.h>
#include <SD.h>
#include "../../Launcher/UI/ui.h"
#include "../../../ChappieBsp/SD/ChappieSD.hpp"
#include "Apps/MusicPlayer.hpp"

/**
 * @brief Create an App in name space 
 * 
 */
namespace App {

    std::string App_Files_appName();
    void* App_Files_appIcon();
    void App_Files_onCreate();
    void App_Files_onLoop();
    void App_Files_onDestroy();
    void App_Files_getBsp(void* bsp);
    void renderDir(const char* path);
    void gotoDir(lv_event_t *event);

    /**
     * @brief Declace your App like this 
     * 
     */
    App_Declare(Files);
}

#endif
