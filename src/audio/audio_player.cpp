
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#include "audio_player.h"
#include "config.h"

namespace {
    DFRobotDFPlayerMini player;
    bool ready = false;
}

bool audioPlayerInit() {
    // STM32 USART2: PA2 = TX, PA3 = RX
    Serial2.begin(DFPLAYER_BAUD);
    delay(300);

    // Initialize DFPlayer and request an acknowledgement.
    ready = player.begin(Serial2, true, true);

    if (!ready) {
        Serial.println("ERROR: DFPlayer initialization failed");
        return false;
    }

    player.volume(20);  // Range: 0 to 30
    delay(200);

    Serial.println("DFPlayer initialized successfully");
    return true;
}

bool audioPlayCommand(GestureCommand command) {
    if (!ready) {
        Serial.println("ERROR: DFPlayer is not ready");
        return false;
    }

    uint16_t track;

    // Keep the track mapping explicit and easy to maintain.
    switch (command) {
        case GestureCommand::COFFEE:
            track = 1;
            break;

        case GestureCommand::WATER:
            track = 2;
            break;

        case GestureCommand::FOOD:
            track = 3;
            break;

        case GestureCommand::WASHROOM:
            track = 4;
            break;

        case GestureCommand::EMERGENCY:
            track = 5;
            break;

        case GestureCommand::NONE:
        default:
            return false;
    }

    // Plays /mp3/0001.mp3 through /mp3/0005.mp3.
    player.playMp3Folder(track);

    Serial.print("Requested audio track: ");
    Serial.println(track);

    return true;
}

void audioSetVolume(uint8_t volume) {
    if (ready) {
        player.volume(constrain(volume, 0, 30));
    }
}

void audioStop() {
    if (ready) {
        player.stop();
    }
}
