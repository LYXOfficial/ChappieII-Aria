#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP
#include "../App_Files.h"
#include "driver/i2s.h"
#include "MP3DecoderHelix.h"
#define I2S_DOUT EMMA_SPEAKER_DATA_PIN
#define I2S_BCLK EMMA_SPEAKER_BCLK_PIN
#define I2S_LRC EMMA_SPEAKER_LRCLK_PIN
// #include "../../../../ChappieBsp/Utility/ESP32-AudioI2S/Audio.h"

using namespace libhelix;
extern CHAPPIE *device;
using namespace std;
struct WAVHeader
{
    char riff[4];
    uint32_t fileSize;
    char wave[4];
    char fmt[4];
    uint32_t fmtSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4];
    uint32_t dataSize;
};
static uint8_t mp3Buffer[4096];
static short pcmBuffer[1940 * 2];
static uint8_t buffer[4096];
static HMP3Decoder hMP3Decoder = NULL;
static File musicFile;
class MusicPlayer
{
private:
    lv_obj_t *musicPlayerScreen, *lastScreen, *playingNameLabel, *backButton, *playButton, *playIcon;
    string playingPath, playingName;
    bool isPlaying = true;
    TaskHandle_t wavPlayTaskHandle = nullptr;

public:
    MusicPlayer(const char *path, lv_obj_t *ls)
    {
        playingPath = path;
        musicPlayerScreen = lv_obj_create(NULL);
        lv_obj_set_style_text_font(musicPlayerScreen, &ui_font_FontHarmonySans18, 0);
        lv_obj_set_size(musicPlayerScreen, 280, 240);
        lv_obj_set_pos(musicPlayerScreen, 0, 0);
        lv_obj_clear_flag(musicPlayerScreen, LV_OBJ_FLAG_SCROLLABLE);
        lv_scr_load(musicPlayerScreen);
        lv_obj_set_style_bg_color(musicPlayerScreen, lv_color_hex(0x000000), 0);
        lv_obj_set_style_text_color(musicPlayerScreen, lv_color_hex(0xeaeaeb), 0);
        lastScreen = ls;
        playingNameLabel = lv_label_create(musicPlayerScreen);
        playingName = playingPath.substr(playingPath.find_last_of("/") + 1, playingPath.length());
        playingName = playingName.substr(0, playingName.find_last_of("."));
        lv_label_set_text(playingNameLabel, playingName.c_str());
        lv_label_set_long_mode(playingNameLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_align(playingNameLabel, LV_ALIGN_TOP_MID, 15, 25);
        lv_obj_set_size(playingNameLabel, 180, 20);
        int16_t textWidth = lv_txt_get_width(playingName.c_str(), playingName.length(), &ui_font_FontHarmonySans18, 0, NULL);
        int16_t horPad = (180 - textWidth) / 2;
        lv_obj_set_style_pad_left(playingNameLabel, horPad, 0);
        backButton = lv_btn_create(musicPlayerScreen);
        lv_obj_set_size(backButton, 50, 50);
        lv_obj_set_pos(backButton, 10, 10);
        lv_obj_set_style_radius(backButton, 25, 0);
        lv_obj_set_style_bg_color(backButton, lv_color_hex(0x000000), 0);
        lv_obj_set_style_shadow_opa(backButton, LV_OPA_0, 0);
        lv_obj_set_style_bg_color(backButton, lv_color_hex(0x111111), LV_STATE_PRESSED);
        lv_obj_t *backIcon = lv_label_create(backButton);
        lv_label_set_text(backIcon, LV_SYMBOL_LEFT);
        lv_obj_align(backIcon, LV_ALIGN_CENTER, 0, 0);
        lv_obj_add_event_cb(backButton, [](lv_event_t *e)
                            {
            MusicPlayer *player = (MusicPlayer *)(lv_event_get_user_data(e));
            player->quit(); }, LV_EVENT_CLICKED, this);
        playButton = lv_btn_create(musicPlayerScreen);
        lv_obj_set_size(playButton, 60, 60);
        lv_obj_align(playButton, LV_ALIGN_BOTTOM_MID, 0, -10);
        lv_obj_set_style_radius(playButton, 30, 0);
        lv_obj_set_style_bg_color(playButton, lv_color_hex(0x000000), 0);
        lv_obj_set_style_shadow_opa(playButton, LV_OPA_0, 0);
        lv_obj_set_style_bg_color(playButton, lv_color_hex(0x111111), LV_STATE_PRESSED);
        playIcon = lv_label_create(playButton);
        lv_label_set_text(playIcon, LV_SYMBOL_PAUSE);
        lv_obj_align(playIcon, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_text_font(playIcon, &lv_font_montserrat_24, 0);
        lv_obj_add_event_cb(playButton, [](lv_event_t *e)
                            {
            MusicPlayer *player = (MusicPlayer *)(lv_event_get_user_data(e));
            player->isPlaying?player->pause():player->play(); }, LV_EVENT_CLICKED, this);
        if (String(path).endsWith(".mp3") || String(path).endsWith(".wav"))
        {
            xTaskCreatePinnedToCore(
                wavPlayTask,
                "WAVPlayTask",
                16384,
                this,
                1,
                &wavPlayTaskHandle,
                APP_CPU_NUM);
        }
    }

    static void wavPlayTask(void *param)
    {
        MusicPlayer *player = static_cast<MusicPlayer *>(param);
        device->lvgl.disable();
        musicFile = SD.open(player->playingPath.c_str());
        if (!musicFile)
        {
            printf("Error opening file %s\n", player->playingPath.c_str());
            delete player;
        }
        device->lvgl.enable();
        if (String(player->playingPath.c_str()).endsWith(".mp3"))
        {
            // Audio audio = Audio(false, 3, I2S_NUM_1);
            // audio.connecttoFS(SD, player->playingPath.c_str());
            // hMP3Decoder = MP3InitDecoder();
            // int bytesRead = 0,offset = 0;
            // MP3FrameInfo frameInfo;
            // i2s_start(EMMA_SPEAKER_I2S_PORT);
            // i2s_zero_dma_buffer(EMMA_SPEAKER_I2S_PORT);
            // while (device->lvgl.disable(), musicFile.available())
            // {
            //     bytesRead = musicFile.read(mp3Buffer, sizeof(mp3Buffer)-1024);
            //     device->lvgl.enable();

            //     if (bytesRead <= 0)
            //         break;

            //     offset = MP3FindSyncWord(mp3Buffer, bytesRead);
            //     while (offset >= 0 && bytesRead - offset > 0)
            //     {
            //         uint8_t *readPtr = mp3Buffer + offset;
            //         int bytesLeft = bytesRead - offset;
            //         if(bytesLeft < 1024) {
            //             int extraBytes = musicFile.read(readPtr + bytesLeft, 1024 - bytesLeft);
            //             bytesLeft += extraBytes;

            //             // 确保数据对齐到 4 字节边界
            //             uint32_t alignment = (uint32_t)readPtr & 3;
            //             if (alignment) {
            //                 alignment = 4 - alignment;
            //                 memcpy(mp3Buffer + alignment, readPtr, bytesLeft);
            //                 readPtr = mp3Buffer + alignment;
            //             }
            //         }
            //         int err = MP3Decode(hMP3Decoder, &readPtr, &bytesLeft, pcmBuffer, 0);
            //         if (err)
            //         {
            //             printf("MP3 Decode Error: %d\n", err);
            //             break;
            //         }

            //         MP3GetLastFrameInfo(hMP3Decoder, &frameInfo);
            //         size_t bytesWritten = 0;
            //         i2s_set_sample_rates(EMMA_SPEAKER_I2S_PORT, frameInfo.samprate << 1);
            //         i2s_set_clk(
            //             EMMA_SPEAKER_I2S_PORT,
            //             frameInfo.samprate<<1,
            //             (frameInfo.bitsPerSample == 16) ? I2S_BITS_PER_SAMPLE_16BIT : I2S_BITS_PER_SAMPLE_32BIT,
            //             frameInfo.nChans == 2 ? I2S_CHANNEL_STEREO : I2S_CHANNEL_MONO);

            //         i2s_write(EMMA_SPEAKER_I2S_PORT, pcmBuffer, frameInfo.outputSamps * sizeof(short), &bytesWritten, portMAX_DELAY);

            //         offset = MP3FindSyncWord(readPtr, bytesLeft);
            //     }
            // }
            // MP3FreeDecoder(hMP3Decoder); // 确保释放解码器
            // hMP3Decoder = NULL;
            // while (1)
            // {
            //     audio.loop();
            // }
        }
        else if (String(player->playingPath.c_str()).endsWith(".wav"))
        {
            device->lvgl.disable();
            WAVHeader header = player->readWAVHeader(musicFile);
            musicFile.seek(header.fmtSize + sizeof(WAVHeader) - 8);
            device->lvgl.enable();
            size_t bytesRead, bytesWritten = 0;
            i2s_start(EMMA_SPEAKER_I2S_PORT);
            i2s_set_sample_rates(EMMA_SPEAKER_I2S_PORT, header.sampleRate << 1);
            i2s_set_clk(EMMA_SPEAKER_I2S_PORT, header.sampleRate << 1, (header.bitsPerSample == 16) ? I2S_BITS_PER_SAMPLE_16BIT : I2S_BITS_PER_SAMPLE_32BIT, I2S_CHANNEL_MONO);
            i2s_zero_dma_buffer(EMMA_SPEAKER_I2S_PORT);
            while (device->lvgl.disable(), (bytesRead = musicFile.read(buffer, sizeof(buffer))) > 0)
            {
                device->lvgl.enable();
                // for(size_t i = 0; i < bytesRead; i +=2){
                //     int16_t sample = (buffer[i] << 8) | buffer[i+1];
                //     // sample/=2;
                //     // printf("%d\n",sample);
                //     buffer[i] = sample>>8;
                //     buffer[i+1] = sample&0xFF;
                // }
                i2s_write(EMMA_SPEAKER_I2S_PORT, buffer, bytesRead, &bytesWritten, portMAX_DELAY);
            }
        }
        vTaskDelay(100);
        i2s_stop(EMMA_SPEAKER_I2S_PORT);
        i2s_zero_dma_buffer(EMMA_SPEAKER_I2S_PORT);
        musicFile.close();
        vTaskDelete(NULL);
        device->lvgl.enable();
    }

    WAVHeader readWAVHeader(File &file)
    {
        WAVHeader header;
        file.read((uint8_t *)&header, sizeof(WAVHeader));
        return header;
    }

    ~MusicPlayer()
    {
        device->lvgl.disable();

        if (wavPlayTaskHandle != nullptr)
        {
            vTaskSuspend(wavPlayTaskHandle);
            vTaskDelay(pdMS_TO_TICKS(50));
            vTaskDelete(wavPlayTaskHandle);
            wavPlayTaskHandle = nullptr;
        }
        i2s_stop(EMMA_SPEAKER_I2S_PORT);
        i2s_zero_dma_buffer(EMMA_SPEAKER_I2S_PORT);
        if (hMP3Decoder)
            MP3FreeDecoder(&hMP3Decoder);
        if (musicFile)
            musicFile.close();
        device->lvgl.enable();
        lv_scr_load(lastScreen);
    }

    void quit()
    {
        delete this;
    }

    void play()
    {
        isPlaying = true;
        lv_label_set_text(playIcon, LV_SYMBOL_PAUSE);
        vTaskResume(wavPlayTaskHandle);
    }

    void pause()
    {
        isPlaying = false;
        lv_label_set_text(playIcon, LV_SYMBOL_PLAY);
        vTaskSuspend(wavPlayTaskHandle);
    }
};

#endif