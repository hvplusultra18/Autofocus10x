import serial
import time
import cv2 
from time import sleep
import picamera
from picamera import PiCamera
camera=PiCamera()
import numpy as np
#initialize the serial port
ser=serial.Serial('/dev/ttyUSB0',9600,timeout=1)
sleep(1)
#function for clockwise x movement
def movexclock(distance):
	if __name__== "__main__" :
		camera.start_preview()
		ser.reset_input_buffer()
		command = "xclk,{}".format(distance)
		ser.write(command.encode("utf-8"))
		
#anticlock x movement
def movexanticlock(distance):

	if __name__== "__main__" :
		camera.start_preview()
		ser.reset_input_buffer()
		command = "xcclk,{}".format(distance)
		ser.write(command.encode("utf-8"))			
#dy movement				
def movey(steps):
	if __name__== "__main__" :
		camera.start_preview()
		command="ycclk,{}".format(steps)
		ser.write(command.encode("utf-8"))
		sleep(1)		
#calculate variance of an image		
def variance(image):
	bg=cv2.GaussianBlur(image,(11,11),0)
	v=cv2.Laplacian(bg,cv2.CV_64F).var()
	return v
#bring camera to initial position
def init():
	if __name__== "__main__" :
		ser.reset_input_buffer()
		ser.write("init".encode("utf-8"))
	
#first autofocus(involves both coarse and fine motor only)
def auto():	
	if __name__ == '__main__':
		z=[]
		var=[]
		l=0
		camera.start_preview()
		ser.reset_input_buffer()
		sleep(1)
		camera.resolution=(320,240)
		camera.framerate=24
		ser.write("zcclk,12250".encode("utf-8"))
		sleep(30)
		for i in range(21):
			image=np.empty((240*320*3),dtype=np.uint8)
			camera.capture(image,'bgr')
			image=image.reshape((240,320,3))
			camera.capture("/home/pi/Downloads/10x/New/image{0}({1}).jpg".format(i,variance(image)))
			var.append(variance(image))
			ser.write("zcclk,50".encode("utf-8"))
			sleep(1)
		
		var=np.array(var) 
		l=np.argmax(var)
		sleep(2)
		a="zclk,{}".format(1050-l*50)
		ser.write(a.encode("utf-8"))
		camera.resolution=(1920,1088)
		sleep((1050-l*50)/400+2)
		camera.capture("/home/pi/Downloads/10x autoscan/test3/focusedimage.jpg")
		sleep(2)
		print(var)

# autofocus for points duing motion(involves fine motor only)
def auto3(i,j):
	z=[]
	var=[]
	l=0
	if __name__=="__main__":
		camera.resolution=(320,240)
		camera.framerate=24
		ser.reset_input_buffer()
		ser.write("zclk,200".encode("utf-8"))
		sleep(1)
		for i in range(11):
			camera.start_preview()
			image=np.empty((240*320*3),dtype=np.uint8)
			camera.capture(image,'bgr')
			image=image.reshape((240,320,3))
			var.append(variance(image))
			ser.write("zcclk,50".encode("utf-8"))
			sleep(1)
		#wait for one second so that chance of command overlap minimizes
		sleep(1)
		var=np.array(var) 
		l=np.argmax(var)
		a="zclk,{}".format(550-l*50)
		ser.write(a.encode("utf-8"))
		camera.resolution=(1920,1088)
		sleep((550-l*50)/400+2)
		camera.capture("/home/pi/Downloads/10x autoscan/blood/imagerow{0},{1}.jpg".format(i,j))
		sleep(1)
		print(var)



def scan():
	if __name__=="__main__":
		s=0
		m=0
		for i in range(10):
			for j in range(10):
				if(i%2==0): #change direction for each y movement
					movexclock(15)
				else:
					movexanticlock(15)
				
				sleep(1)
				if (j%5==0 and i>0): # autofocus on every 5th frame
					auto3(i,j)
				else:
					#capture and save
					camera.resolution=(1920,1088)
					camera.capture("/home/pi/Downloads/10x autoscan/blood/imagerow{0},{1}.jpg".format(i,j))
			
				
			movey(20)
			sleep(1)	
		#move back to the bottom 
		ser.write("zclk,13000".encode("utf-8"))
init()	
sleep(10)
auto()
sleep(1)
scan()