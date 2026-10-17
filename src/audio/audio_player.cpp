#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "audio_player.h"
#include "config.h"

namespace {
DFRobotDFPlayerMini player;
bool ready = false;
}

bool audioPlayerInit() {
    Serial2.begin(DFPLAYER_BAUD);
    ready = player.begin(Serial2, true, true);
    if (ready) player.volume(20); // DFPlayer volume range is commonly 0-30.
    return ready;
}

bool audioPlayCommand(GestureCommand command) {
    if (!ready || command == GestureCommand::NONE) return false;

    // Expected files: /mp3/0001.mp3 through /mp3/0005.mp3.
    uint16_t track = static_cast<uint16_t>(command) + 1;
    player.playMp3Folder(track);
    return true;
}

void audioSetVolume(uint8_t volume) {
    if (ready) player.volume(constrain(volume, 0, 30));
}

void audioStop() {
    if (ready) player.stop();
}
