//Include the DMX master library supplied by TinkerKit
#include <DmxMaster.h>

void setup() {
  //Set the pin for DMX output
  DmxMaster.usePin(3);
  
  //Set the maximum number of DMX channels being used
  DmxMaster.maxChannel(4);

  //Setup pinModes
  //Pin 10 reads if full brightness
  pinMode(10, INPUT);
  //Pin 11 reads if half brightness
  pinMode(11, INPUT);

}

//Define delay variable to slow change in brightness
int delayVar = 10;

//Global brightness state variable, on by default
// 0   - no brightness
// 128 - half brightness
// 255 - full brightness
int globalBrightness = 255;
  
void loop() {

  //Read input pins from the relay
  int fullBrightState = digitalRead(10);
  int halfBrightState = digitalRead(11);

  //Lights full on
  if(fullBrightState == 1){
    //Slowly change brightness to new value
    changeBrightness(globalBrightness, 255);
    
    //Update global brightness to the new current state
    globalBrightness = 255;
  }
  
  //Lights half on
  else if (halfBrightState == 1){
    changeBrightness(globalBrightness, 128);
    
    //Update global brightness to the new current state
    globalBrightness = 128;
  }
  
  //Lights off
  else if (!fullBrightState && !halfBrightState){
    changeBrightness(globalBrightness, 0);
    
    //Update global brightness to the new current state
    globalBrightness = 0;
  }

}

//Function to change brightness
void changeBrightness(int currentBrightness, int targetBrightness){
  //slowly change state based on current state and target 
  //Dim current brightness to target
  if (currentBrightness > targetBrightness){
      for(int brightness = currentBrightness; brightness >= targetBrightness; brightness++){
      
      //Update DMX channel 1 to new brightness
      DmxMaster.write(1, brightness);

      //Define to slow change in brightness
      delay(delayVar);
    }
  }
  //Brighten current brightness to target
  else if (currentBrightness < targetBrightness){
    for(int brightness = currentBrightness; brightness <= targetBrightness; brightness++){
      
      //Update DMX channel 1 to new brightness
      DmxMaster.write(1, brightness);

      //Define to slow change in brightness
      delay(delayVar);
    }
  }
  //If brightness is the same then do no change
  else {
    //Do nothing
  }
}
