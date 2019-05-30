#include "Keyboard.hpp"

class Keyboard
{
    private:
    std::vector<bool> kbd_m;
    public:
    Keyboard(bool HAS_LEDS, int rows, int cols);
    ~Keyboard();
};
Keyboard::Keyboard(bool HAS_LEDS, int rows, int cols){
    this->kbd_m = std::vector<bool> {1,0,1,0};
    if (HAS_LEDS) {
        return;
    };
}
Keyboard::~Keyboard(){
    
    //delete (this->kbd_m); // impossibiru!1
}