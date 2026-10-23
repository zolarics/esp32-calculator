# ESP32 Calculator

This repository contains code that can be uploaded to an ESP32, allowing it to function as a calculator.

# Setup Guide

**IMPORTANT**

This project was made using the following ESP32 Kit: https://us.elegoo.com/collections/arduino-kits/products/elegoo-esp-32-super-starter-kit
Some parts of the code (such as the keypad.h library) came from documented code found within elegoo's esp32 starter kit guide.


**Step 1: Preperation**

You will need the Arduino IDE or the CLI to upload the code to the ESP 32. You can download the tools from Arduino's website:
https://www.arduino.cc/


**Step 2: Download Requirements**

You can do this one of two ways:
A) Manually download the version you want
B) Clone the entire repository

*Choice A:*
- Select the version you want, then download ``calculator.ino`` and ``libraries.zip`` (note that calculator.ino will need to be put in a directory named "calculator")

*Choice B:*
- Run the following command in your terminal. This will download the entire repository into whatever directory you are currently in.
          
      git clone https://github.com/zolarics/esp32-calculator 


**Step 3: Selection and Wiring**

If you didn't download manually, then select the specific version of calculator you want to use.

Once you have the version you want, read the version's ``readme.md``, and follow ``wiring_guide.md``.


**Step 4: Install the ESP32 Core**

This step will vary depending on if you installed the IDE or the CLI

- **Arduino CLI**

  If this is a new install of the ardunio cli, run: 
  
      arduino-cli config init

  Search for the core with the following command: 
  
      arduino-cli core search esp32 --json

  As of writing this (10/22/2026), the correct core has the following information:
    
      "id": "esp32:esp32",
      "maintainer": "Espressif Systems",
      "website": "https://github.com/espressif/arduino-esp32",
      "email": "hristo@espressif.com"
    
  Once verified, install the correct version stated in the calculator version's readme:
    
      arduino-cli core install [id]@[version#]

  After installation, update the local cache of libraries:

      arduino-cli core update-index

- **Arduino IDE**: 

    Open "Boards Manager" (Left side of the screen OR top of the screen via Tools -> Board -> Boards Manager)
    
    Search for "ESP32", and look for a board by ``Espressif Systems``.
    
    Select the correct version of board from the calculator version's readme from the dropdown, and install it.


**Step 5: Import Libraries**

You will need to unzip the "libraries.zip" folder into where your ardunio software stores its libraries.

- **Linux**: 
  
  In your home folder, you can find the libraries folder under the ``Arduino`` directory in your home folder. `~/Arduino/libraries`
  - Note that it will not be there if you have not installed any libraries.
  - You can create it via your file manager or by running the following in your terminal.
              
        mkdir ~/Arduino && mkdir ~/Arduino/libraries
      
- **Windows**

  You can find the libraries folder under the ``Arduino`` directory in your user's documents folder. `~\Documents\Arduino\libraries`
  
  - Note that it will not be there if you have not installed any libraries. 
  - You can create it via your file manager or by running the following in powershell:

        mkdir ~\Documents\Arduino ; mkdir ~\Documents\Arduino\libraries


**Step 6: Upload Code**

This step will vary depending on if you install the IDE or the CLI

- **Arduino CLI**:

  Look for your board's FQBN id by running:
  
      arduino-cli board listall
      
  > If you are using the board from the Elegoo ESP32 starter kit, then the FQBN should be esp32:esp32:esp32. This can be found under "ESP32 Dev Module" in the above command.
  
  Navigate to the "calculator" directory in the calculator version you want to use.
  
  Compile the calculator code by running: 
  
      arduino-cli compile --fqbn [FQBN] calculator.ino
  
  Plug in your ESP32 and find the port it is connected to by running:
  
      arduino-cli board list
  
  > The FQBN may show up as Unknown, this is fine.
  
  To upload the code, run:
      
      arduino-cli upload -p [Port] --fqbn [FQBN] calculator.ino
      
  > On linux, you may get an error when uploading about your user not being in the "dialout" group.
  > You can fix this by running the following and then restarting your computer:
  
      sudo usermod $USER -a -G dialout

- **Arduino IDE**: 

  Open up the ``calculator.ino`` file in the calculator directory from the version of calculator you want to use. 
  
  Plug in your ESP32, then select the correct Board and Port it is connected to in the top left.
  > If you are using the board from the Elegoo ESP32 starter kit, then you should select "ESP32 Dev Module".
  
  > Across all boards, the port may show that it is connected to "Unknown". This is fine.
  
  Click "Upload" in the top left to upload the code.
