#include "FakeArduino.h"
#include "../src/kernel/MsgService.h"
#include "../src/tasks/TakeOffPattern.h"
void serialEvent();

int main() {
    MsgService.init();

    std::string input;
    Serial.inject("TAKE_OFF");
    while (true) {
        // std::cout << "> ";
        // if (!std::getline(std::cin, input)) break;

        Serial.inject("TAKE_OFF");

        MsgService.SerialEvent();  // let MsgService read from the fake Serial buffer

        // TakeOffPattern pattern;
        // if (MsgService.isMsgAvailable(pattern)) {
        //     Msg* msg = MsgService.receiveMsg(pattern);
        //     std::cout << "[Matched] " << msg->getContent().c_str() << std::endl;
        //     delete msg;
        // }
    }
}