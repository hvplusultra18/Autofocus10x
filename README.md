# Autofocus in 10x

This project is the implementation of an algorithm which automates the focusing processes of a microscope
 
 ## Installations 
 install cv2 library as we are going to be using this mainly for the computer vision part.

 Now there are two ways of installing the cv2 library:

 * install the main modules package:
 ```bash 
    
    $pip install opencv-python
 ```
 *  Full package:
 ```bash
    $pip install opencv-contrib python
    
 ```
 I've installed all the packages and the pi documentation also recommends you to use the second method.

 The PySerial library can be installed similarly:
 ```bash
    $pip install pyserial
 ```
 The rest of the libraries are preinstalled(assuming numpy has already been installed by the user)

 ## Working 
 initialize the arduino us port by using the command:
 ```python
    ser=serial.Serial('/dev/ttyUSB0',9600,timeout=1)
 ```

 We store it as a variable as it acts as a channel through which we communicate with the Arduino.
 We also use the sleep command as a way of telling the PI4 to wait for sometime before excecuting the next command 
 ```python
 def movexclock(distance):
	if __name__== "__main__" :
		camera.start_preview()
		ser.reset_input_buffer()
		command = "xclk,{}".format(distance)
		ser.write(command.encode("utf-8"))
 ```  
 Let me explain what this block of code does:
 ```python
 if __name__== "__main__" :
 ```
 We write the communication command lines nested under this if statement so that the command is run only when it goes throught he main python interpreter.
 ``` python
 camera.start_preview()
 ```
 It lets you see in real time what your commands are actually doing to the machine.  A live stream of the camera connected to the Pi4 appears on your computer
 ```python
 ser.reset_input_buffer()
 ```
 The data is sent in byte sized chunks called buffers so this line clears the data and resets it for a new run.
 ```python
    command = "xclk,{}".format(distance)
    ser.write(command.encode("utf-8"))
 ```
 This block is going to be unclear as the command given to the ser.write method is actually in another file. I currently dont have it in my laptop and I will try to upload the file ASAP. This block is for the clockwise rotation of the motors which are responsible for movement of the platform in the x-direction.