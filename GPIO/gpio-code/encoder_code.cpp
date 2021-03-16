#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <math.h>
#include <JetsonGPIO.h>

#define _USE_MATH_DEFINES

using namespace std;
bool end_this_program = false;

inline void delay(int s){
    this_thread::sleep_for(chrono::seconds(s));
}

void signalHandler (int s){
    end_this_program = true;
}

int main(){
    const string input_pin_A = "12"; // BOARD pin 12, Addressing it as string to avoid bugs
    const string input_pin_B = "18";
    // When CTRL+C pressed, signalHandler will be called
    signal(SIGINT, signalHandler);
    // Pin Setup.
    GPIO::setmode(GPIO::BOARD);
    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(input_pin_A, GPIO::IN);
    GPIO::setup(input_pin_B, GPIO::IN);
    cout << "Starting demo now! Press CTRL+C to exit" << endl;
    int curr_value_A, past_value_A = GPIO::LOW, tick_no_A = 0;
    int curr_value_B, past_value_B = GPIO::LOW, tick_no_B = 0;

    while(!end_this_program){
        //delay(1); // Toggle the output every second
        curr_value_A = GPIO::input(input_pin_A);
        curr_value_B = GPIO::input(input_pin_B);
        if(curr_value_A != past_value_A){
            tick_no_A++;
            if(tick_no_A % 20 == 0){
                cout << "Reading " << curr_value_A << " from pin ";
                cout << input_pin_A << " tick number " << tick_no_A << "\n";
            }
        if(tick_no_A % 10 == 0){
            cout << "Distance covered on motor A " << double(tick_no_A) * (3*M_PI / 10) << "\n";
        	}
        }
        if(curr_value_B != past_value_B){
            tick_no_B++;
            if(tick_no_B % 20 == 0) {
                cout << "Reading " << curr_value_B << " from pin ";
                cout << input_pin_B << " tick number " << tick_no_B << "\n";
            }
            if(tick_no_B % 10 == 0){
            cout << "Distance covered on motor B " << double(tick_no_B) * (3*M_PI / 10) << "\n";
        	}
        }
        
        
        past_value_A = curr_value_A;
        past_value_B = curr_value_B;
    }

    GPIO::cleanup();

    return 0;
}

