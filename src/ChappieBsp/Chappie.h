/**
 * @file Chappie.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "EmmaBSP/Emma.h"
#include "CTP/ChappieCTP.hpp"
#include "IMU/ChappieIMU.hpp"
#include "Power/ChappiePower.hpp"
#include "SD/ChappieSD.hpp"
#include "ENV/ChappieENV.hpp"
#include "Utility/BM8563/I2C_BM8563.h"
#include "Lvgl/ChappieLvgl.hpp"
//  Network
#include "ChappieNet/WiFi/ConfigWiFi.h"
//Resoure 
#include "ChappieUI/Resource/ResourcePool.h"
class CHAPPIE : public EMMA {
    private:
        
    public:
        const std::string Logo = R"(
  ___ _  _  __  ____ ____ __ ____ 
 / __) )( \/ _\(  _ (  _ (  |  __)
( (__) __ (    \) __/) __/)( ) _) 
 \___)_)(_|_/\_(__) (__) (__|____)
 
)";
        const std::string Yeely = R"(
 _  _  ____  ____  __   _  _ 
( \/ )( ___)( ___)(  ) ( \/ )
 \  /  )__)  )__)  )(__ \  / 
 (__) (____)(____)(____)(__) 
 
)";
        ChappieCTP Tp;

        ChappieIMU Imu;

        ChappieSD Sd;

        ChappiePower Pow;

        ChappieLvgl lvgl;

        I2C_BM8563 Rtc = I2C_BM8563(I2C_BM8563_DEFAULT_ADDRESS, Wire);

        ChappieENV Env;

        // Net --WiFi --Bluet 
        ConfigWiFi Wf;

        void init();
    
};


