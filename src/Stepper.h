#pragma once

#include <stdint.h>
#include <algorithm>

class Stepper
{
    static constexpr unsigned vPullIn_min = 50;            // smallest possible pullIn frequency  (steps/s)
    static constexpr unsigned vMaxMax = 500000;            // largest speed possible (steps/s)
    static constexpr unsigned aMax = 500000;               // speed up to 500kHz within 1 s (steps/s^2)

    static constexpr unsigned vPullIn_default = 100;       // should work with any motor 
    static constexpr unsigned vDefault = 800;              // should work with any motor (1 rev/sec in 1/4-step mode)
    static constexpr unsigned aDefault = 2500;             // reasonably low (~0.5s for reaching the default speed)
    static constexpr unsigned vMaxDefault = vMaxMax;

public:
    Stepper(const int StepPin, const int DirPin);

    inline Stepper& setPullInSpeed(unsigned pullInFreq) { v_pullIn = std::max(vPullIn_min, pullInFreq); return *this; } // largest speed the motor starts without accelerating (steps/s)
    inline Stepper& setMaxSpeed(unsigned speed) { vMax = std::min(vMaxMax, speed); return *this; }
    inline Stepper& setAcceleration(unsigned _a) { a = std::min(aMax, _a); return *this; }                           // acceleration (steps/s^2)
    Stepper& setStepPinPolarity(int p);
    Stepper& setInverseRotation(bool b);
        
    void setTargetAbs(int pos);			// Set target position 
    void setTargetRel(int delta);		// Set target position relative to current position

    inline void setStepPin() { *stepPinActiveReg = 1; }
    inline void clearStepPin() { *stepPinInactiveReg = 1; }
    
    inline int32_t getPosition() const { return position + dirCw* current; }

    volatile int current;
    unsigned target;
    int D;
    unsigned v_pullIn;
    unsigned vMax;
    unsigned a;

    static bool cmpDelta(const Stepper * a, const Stepper * b) { return a->target > b->target; }
    static bool cmpAcc(const Stepper* a, const Stepper*b) { return a->a < b->a; }
    static bool cmpVpullIn(const Stepper* a, const Stepper*b) { return a->v_pullIn < b->v_pullIn; }
    static bool cmpV(const Stepper* a, const Stepper*b) { return a->vMax < b->vMax; }

private:
    int32_t position;

    volatile uint32_t* stepPinActiveReg;
    volatile uint32_t* stepPinInactiveReg;
    volatile uint32_t* dirPinCwReg;
    volatile uint32_t* dirPinCcwReg;
    int dirCw;
    const int stepPin, dirPin;
};


