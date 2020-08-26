# The Protector
  The Protector is a safety and automation system that can be used in shops,airports,railway stations and other public places to enable a touchless interface and to prevent the spread of COVID-19 virus. This repository consists of Arduino MKR wifi 1010's .ino sketch file along with the android app's source code of Protector written in Python3 language and uses Kivy python package to build the application.
# Uploading Sketch to Arduino MKR wifi 1010:
  1. Connect the Arduino MKR wifi 1010 board to your PC and open the Arduino IDE.
  2. In the Arduino IDE, Navigate to Tools > Board > Boards Manager and then search and install Arduino SAMD Boards(32-bits ARM Cortex-M0+).
  3. After installing the board,clone this repository and navigate to the protector_sever folder and open the sketch in your Arduino IDE.
  4. Enter your RFID tag or card's key in the RFID section of the code.
  5. Now,enter your network's SSID and PASS in the arduino_secrets.h tab and then select Tools > Board > "Arduino MKR Wifi 1010".
  6. Now,just upload the code to your board by pressing 'Ctrl + R' or go to Sketch > Upload.
  
# Building the Protector App:
  1. Let's assuming that you are using "Ubuntu os",and now create a python3 virtual environment using terminal. 
  2. Now activate the environment and install kivy and buildozer using pip.
  3. After installing,Clone this repository to your computer and then navigate to the Protector app folder.
  4. Now open the main.py add your Arduino MKR wifi 1010's IP address to the "root_url".
  5. Save the file,turn on the Protector device and then run the main.py using terminal.
  6. If everything goes fine as planned,then start building the app by running the command "buildozer -v android debug"
  7. The above command starts building your app and it may take a while at first time,so just relax and wait for the build to get completed.
  8. Once build gets completed, you can find a apk file in the bin folder inside the Protector app folder.
  9. Copy the apk file into your smartphone and then install it.
  10. Now make sure the Protector device is ON and then launch the Protector app.
