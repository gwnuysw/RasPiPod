# RasPiPod
gwnu 2nd grade raspberrypi project
this device is based on raspberrypi and atmega2560 controlboard.
FlowerPod envirenment is watched by sensers.
atmega2560 controlboard gets Humi, temp, soli moisture data and control AC motor to give water on soil envirenment .
The data that atmega2560 gained sended to raspberrypi using wifi AP comunication.
raspberrypi and atmega2560 control board share same wifi AP.
raspberrypi write received Humi, temp, soil moisture data on *.txt file.
