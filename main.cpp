#include <iostream>
#include <cstdio>

#include <stdio.h>
#include <alsa/asoundlib.h>

int main() {
    int err;
    int card = -1;
    snd_ctl_t *handle;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    // snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
    snd_pcm_stream_t stream = SND_PCM_STREAM_CAPTURE;

    // 为 card_info 和 pcm_info 结构分配内存
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);

    // 枚举系统中的声卡
    while (snd_card_next(&card) >= 0 && card >= 0) {
        char name[32];
        sprintf(name, "hw:%d", card);

        // 打开声卡控制接口
        if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
            fprintf(stderr, "Cannot open control for card %d: %s\n", card, snd_strerror(err));
            continue;
        }

        // 获取声卡信息
        if ((err = snd_ctl_card_info(handle, info)) < 0) {
            fprintf(stderr, "Cannot get info for card %d: %s\n", card, snd_strerror(err));
            snd_ctl_close(handle);
            continue;
        }

        printf("Card %d: %s [%s], device %s\n", card,
               snd_ctl_card_info_get_id(info),
               snd_ctl_card_info_get_name(info),
               snd_ctl_card_info_get_driver(info));

        // 枚举声卡上的 PCM 设备
        int device = -1;
        while (snd_ctl_pcm_next_device(handle, &device) >= 0 && device >= 0) {
            printf("hw:%d,%d\n", card, device);
            snd_pcm_info_set_device(pcminfo, device);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);

            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0) {
                fprintf(stderr, "Cannot get PCM info for device %d: %s\n", device, snd_strerror(err));
                continue;
            }

            printf("  Device %d: %s [%s]\n", device,
                   snd_pcm_info_get_id(pcminfo),
                   snd_pcm_info_get_name(pcminfo));
        }

        // 关闭声卡控制接口
        snd_ctl_close(handle);
    }

    return 0;
}
