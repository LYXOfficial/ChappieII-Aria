#include "App_Files.h"
static std::string app_name = "文件";
CHAPPIE *device;

namespace App
{
    using namespace fs;
    using namespace std;
    FS rootfs(SD);
    File rootdir;
    lv_obj_t *dirLabel, *filesList;
    vector<char *> pathHistory;
    MusicPlayer *player = nullptr;

    std::string App_Files_appName()
    {
        return app_name;
    }

    void *App_Files_appIcon()
    {
        return NULL;
    }

    /**
     * @brief Called when App is on create
     *
     */

    void renderDir(const char *path)
    {
        char *symbol;
        lv_obj_clean(filesList);
        lv_obj_t *btn;
        File *file, *parent = new File(rootfs.open(path));
        if (!(*parent))
        {
            lv_obj_t *label = lv_label_create(lv_scr_act());
            lv_label_set_text(label, "请插入 SD 卡");
            lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
            return;
        }
        lv_label_set_text(dirLabel, parent->path());
        pathHistory.push_back(strdup(parent->path()));
        if (strcmp(parent->path(), "/"))
        {
            btn = lv_list_add_btn(filesList, LV_SYMBOL_DIRECTORY, "..");
            string parentPath = string(parent->path());
            parentPath = parentPath.substr(0, parentPath.rfind('/'));
            if (parentPath.length() == 0)
                parentPath = "/";
            char *newPath = strdup(parentPath.c_str());
            pathHistory.push_back(newPath);
            lv_obj_add_event_cb(btn, [](lv_event_t *event)
                                {
                    char *path = (char *)event->user_data;
                    renderDir(path); }, LV_EVENT_CLICKED, (void *)newPath);
        }
        file = new File(parent->openNextFile());
        while (*file)
        {
            if (file->isDirectory())
                symbol = LV_SYMBOL_DIRECTORY;
            else if (String(file->name()).endsWith(".mp3") || String(file->name()).endsWith(".wav"))
                symbol = LV_SYMBOL_AUDIO;
            else
                symbol = LV_SYMBOL_FILE;
            btn = lv_list_add_btn(filesList, symbol, file->name());
            if (file->isDirectory())
            {
                char *newPath = strdup(file->path());
                pathHistory.push_back(newPath);
                lv_obj_add_event_cb(btn, [](lv_event_t *event)
                                    {
                    char *path = (char *)event->user_data;
                    renderDir(path); }, LV_EVENT_CLICKED, (void *)newPath);
            }
            else if (String(file->name()).endsWith(".mp3") || String(file->name()).endsWith(".wav"))
            {
                char *newPath = strdup(file->path());
                pathHistory.push_back(newPath);
                lv_obj_add_event_cb(btn, [](lv_event_t *event)
                                    {
                    char *path = (char *)event->user_data;
                    player = new MusicPlayer(path, lv_scr_act()); }, LV_EVENT_CLICKED, (void *)newPath);
            }
            lv_obj_t *label = lv_obj_get_child(btn, 1);
            lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
            file->close();
            delete file;
            file = new File(parent->openNextFile());
        }
    }

    void App_Files_onCreate()
    {
        UI_LOG("[%s] onCreate\n", App_Files_appName().c_str());
        lv_obj_set_style_text_font(lv_scr_act(), &ui_font_FontHarmonySans18, 0);
        filesList = lv_list_create(lv_scr_act());
        dirLabel = lv_label_create(lv_scr_act());
        lv_obj_set_size(filesList, 280, 205);
        lv_obj_set_pos(filesList, 0, 35);
        lv_obj_set_pos(dirLabel, 30, 5);
        lv_obj_set_size(dirLabel, 250, 30);
        lv_label_set_text(dirLabel, "");
        renderDir("/");
    }

    void App_Files_onLoop()
    {
    }

    void App_Files_onDestroy()
    {
        UI_LOG("[%s] onDestroy\n", App_Files_appName().c_str());
        if (player != nullptr)
        {
            delete player;
        }
        for (auto path : pathHistory)
        {
            delete path;
        }
        pathHistory.clear();
    }

    void App_Files_getBsp(void *bsp)
    {
        device = (CHAPPIE *)bsp;
    }
}
