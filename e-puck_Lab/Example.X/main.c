#include <p30F6014A.h>
#include <motor_led/e_epuck_ports.h>
#include <motor_led/e_init_port.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include <uart/e_uart_char.h>
#include <stdio.h>

void Wait(long);

int GetSelector(void);

    

int main()
{
    // -------------Initialisation. ------------------
    e_init_port(); // Initialises the ports.
    e_init_ad_scan(ALL_ADC); // Initialises the analogue to digital converters.
    e_led_clear(); // Turn off all the LEDs in the initialisation stage.
    e_start_agendas_processing(); // Allows the robot to execute multiple tasks.
    
    e_init_uart1(); // Initialises the UART. Uncomment when you use Bluetooth communication.
    e_calibrate_ir(); // Uncomment when you use Proximity Sensors.
    e_init_motors(); // Initialises the motors. Uncomment when you use the motors.
// -------------------- Define variables -----------------
    int spiral=250;
    int proximity[8];
    int i;
    int proxlimit=300;
    int BiasLeft[8]={-10, -10, -5, 0, 0, 5, 10, 10};
    int BiasRight[8]={10, 10, 5, 0, 0, -5, -10, -10};
    int leftspeed;
    int rightspeed;
    
    
    e_set_speed_left(500);
    e_set_speed_right(500);
    
    Wait(5000);
            
    // --------------- Main Loop.------------------
    while(1)
    { 
        //Obtain proximity readings
        for (i=0;i<8;i++)
        {
            proximity[i]=e_get_prox(i); //store prox values into a matrix
        }
         
        //Avoiding obstacles in front of the e puck
        if (proximity[6]|| proximity[7] || proximity[0] || proximity[1] > proxlimit) // only execute if obstacle is in front of front sensors, include sensors 5 & 2?
        {
            for (i=0;i<8;i++)
            {
                leftspeed+=BiasLeft[i]*proximity[i]>>4;
                rightspeed+=BiasRight[i]*proximity[i]>>4;//Bit shift used to reduce size of speed values, play around with shift numbers
                
                if (leftspeed > 1000)
                {
                    leftspeed= 1000;
                    Wait(5000);
                }
                
                if (rightspeed > 1000)
                {
                    rightspeed=1000;
                    Wait(5000);
                }
                
                if (leftspeed < -1000)
                {
                    leftspeed= -1000;
                    Wait(5000);
                }
                
                if (rightspeed < -1000)
                {
                    rightspeed=-1000;
                    Wait(5000);
                }
            }
            
            e_set_speed_left(leftspeed);
            e_set_speed_right(rightspeed);
            Wait(5000);
            
        }
        else 
        {
            //Spiralling
            //e_set_front_led(0); // Turn the front LED off. 
            //e_set_speed_left(750);//Left wheel set to desired speed
            //spiral+=1; //Incrasing spiral value to increase radius of movement
            //if (spiral>750)
            //{
                //spiral=250; //Resets spiral radius to prevent forward and opposite spiralling
                //e_set_speed_right(spiral);
              //  wait(50000);
            //}
            
            e_set_speed_left(500);
            e_set_speed_right(500);           
        }
    }
}
    //do delays need to be added? where do i add these?

void Wait(long Duration) 
{
    long i;
    for(i=0;i<Duration;i++);
}

int GetSelector() {
    return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}
