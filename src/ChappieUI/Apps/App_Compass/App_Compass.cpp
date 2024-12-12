#if 1
#include "App_Compass.h"
#include "../../../ChappieBsp/Chappie.h"
#include "DFRobot_BMM150.h"

static std::string app_name = "指南针";
static CHAPPIE* device;
DFRobot_BMM150_I2C bmm150(&Wire, I2C_ADDRESS_4);

namespace App {

    /**
     * @brief Return the App name laucnher, which will be show on launcher App list
     * 
     * @return std::string 
     */
    std::string App_Compass_appName()
    {
        return app_name;
    }


    /**
     * @brief Return the App Icon laucnher, NULL for default
     * 
     * @return void* 
     */
    void* App_Compass_appIcon()
    {
        return NULL;
    }


    /**
     * @brief Called when App is on create
     * 
     */
    void App_Compass_onCreate()
    {
        UI_LOG("[%s] onCreate\n", App_Compass_appName().c_str());
        bmm150.setOperationMode(BMM150_POWERMODE_NORMAL);
        bmm150.setPresetMode(BMM150_PRESETMODE_HIGHACCURACY);
        bmm150.setRate(BMM150_DATA_RATE_10HZ);
        bmm150.setMeasurementXYZ();
        bmm150.setThresholdInterrupt(LOW_THRESHOLD_INTERRUPT, 0, POLARITY_LOW);

    }


    /**
     * @brief Called repeatedly, end this function ASAP! or the App management will be affected
     * If the thing you want to do takes time, try create a taak or lvgl timer to handle them.
     * Try use millis() instead of delay() here
     * 
     */
    void App_Compass_onLoop()
    {
        // sBmm150MagData_t magData = bmm150.getGeomagneticData();
        // printf("mag x = %d uT\r\n",magData.x);
        // printf("mag x = %d uT\r\n",magData.y);
        // printf("mag x = %d uT\r\n",magData.z);
        // float compassDegree = bmm150.getCompassDegree();
        // printf("the angle between the pointing direction and north (counterclockwise) is: %d\r\n",compassDegree);
        // printf("--------------------------------\r\n");
        // delay(100);
        device->Vibrator.Buzzzzz(0x7fffffff);
        // vibrator test
    }


    /**
     * @brief Called when App is about to be destroy
     * Please remember to release the resourse like lvgl timers in this function
     * 
     */
    void App_Compass_onDestroy()
    {
        UI_LOG("[%s] onDestroy\n", App_Compass_appName().c_str());
    }


    /**
     * @brief Launcher will pass the BSP pointer through this function before onCreate
     * 
     */
    void App_Compass_getBsp(void* bsp)
    {
        device = (CHAPPIE*)bsp;
    }
    
}

#endif
