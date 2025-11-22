#include <LiquidCrystal_I2C.h> 
class Lcd {
private:
    LiquidCrystal_I2C lcd;

public:
    Lcd(uint8_t address, uint8_t width, uint8_t height)
        : lcd(address, width, height) {}

    void begin() {
        lcd.init();
        lcd.backlight();
    }

    void clear() {
        lcd.clear();
    }

    void printAt(int row, int col, const String& text) {
        lcd.setCursor(col, row);
        lcd.print(text);
    }

    // optional convenience
    void print(const String& text) {
        lcd.print(text);
    }
};