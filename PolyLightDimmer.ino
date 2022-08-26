//Include the DMX master library supplied by TinkerKit
#include <DmxMaster.h>

void setup() {
  //Set the pin for DMX output
  DmxMaster.usePin(3);
  
  //Set the maximum number of DMX channels being used (default is 4, increase this if additional lights are required)
  DmxMaster.maxChannel(4);

  //Setup pinModes
  //Pin 10 reads if half brightness
  pinMode(10, INPUT);
  //Pin 11 reads if full brightness
  pinMode(11, INPUT);

  //Setup serial comms for debugging
  Serial.begin(9600);
}

//Delay variable can be modified to increase or decrease the speed the lights will change
int delayVar = 25;

//Global brightness state variable, off by default, will be updated when turned on
// 0   - no brightness
// 128 - half brightness
// 255 - full brightness
int globalBrightness = 0;

//Common values for full, half and none, for different intensities update here
int full = 255;
int half = 100;
int none = 0;


void loop() {
  
  //Read input pins from the relay
  int halfBrightState = digitalRead(10);
  int fullBrightState = digitalRead(11);


  //Lights full on
  if(fullBrightState == 1){
    //Slowly change brightness from current brightness to full
    changeBrightness(globalBrightness, full);
  }
  
  //Lights half on
  if (halfBrightState == 1){
    //Slowly change brightness from current brightness to half
    changeBrightness(globalBrightness, half);
  }
  
  //Lights off
  if(fullBrightState == 0 && halfBrightState == 0){
    //Slowly change brightness from current brightness to off
    changeBrightness(globalBrightness, none);
  }

  //Delay repeating the loop to allow for the relays to update (if this is too short when changing from full -> half it may cause eratic behaviour such as full -> off -> half)
  delay(1500);

}

//Function to change brightness, slowly changing state based on current state and target
void changeBrightness(int currentBrightness, int targetBrightness){
  
  //Dim current brightness to target
  if (currentBrightness > targetBrightness){
      //Loop through and decrement brightness value
      for(int brightness = currentBrightness; brightness >= targetBrightness; brightness--){
      
      //Update DMX channels to new brightness
      DmxMaster.write(1, brightness);
      DmxMaster.write(2, brightness);

      //Delay for a slow change in brightness
      delay(delayVar);

      //Update global brightness with current brightness
      globalBrightness = brightness;
    }
  }
  
  //Brighten current brightness to target
  else if (currentBrightness < targetBrightness){
    
    //The following section is to help reduce the sudden light flash from occurring when the current light state is off
    if(currentBrightness == 0){
      //Slowly turn on lights for channel 1 and 2
      currentBrightness == 1;
      DmxMaster.write(1, currentBrightness);
      DmxMaster.write(2, currentBrightness);
      delay(10);
      currentBrightness == 2;
      DmxMaster.write(1, currentBrightness);
      DmxMaster.write(2, currentBrightness);
      delay(10);
      currentBrightness == 3;
      DmxMaster.write(1, currentBrightness);
      DmxMaster.write(2, currentBrightness);
    }

    //Loop through and increment brightness value
    for(int brightness = currentBrightness; brightness <= targetBrightness; brightness++){
      
      //Update DMX channels to new brightness
      DmxMaster.write(1, brightness);
      DmxMaster.write(2, brightness);
      
      //Delay for a slow change in brightness
      delay(delayVar);

      //Update global brightness with current brightness
      globalBrightness = brightness;
    }
  }
  //If brightness is the same then do no change
  else {
    //Do nothing
  }
}
