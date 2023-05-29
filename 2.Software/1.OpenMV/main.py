# 人眼追踪例程
#
# 该脚本使用内置的前脸检测器来查找脸部，然后查找脸部的眼睛。如果你想确定
# 眼睛的瞳孔，请参阅iris_detection脚本，了解如何做到这一点。

import sensor, time, image, lcd

from pyb import UART
from pyb import LED
from pyb import Pin, Timer


red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)


#重置传感器
sensor.reset()

#传感器设置
sensor.set_contrast(1)
sensor.set_gainceiling(16)

#灰度模式
sensor.set_framesize(sensor.HQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)

#RGB模式
#sensor.set_pixformat(sensor.RGB565) # or sensor.GRAYSCALE
#sensor.set_framesize(sensor.QVGA) # Special 128x160 framesize for LCD Shield.


uart = UART(3,115200)
uart.init(115200, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位


# 50kHz pin7 timer4 channel1
light = Timer(4, freq=50000).channel(1, Timer.PWM, pin=Pin("P7"))
light.pulse_width_percent(1) # 控制亮度 0~100


lcd.init() # Initialize the lcd screen.


# 加载Haar算子
# 默认情况下，这将使用所有阶段，较低的阶段更快但不太准确。          25
face_cascade = image.HaarCascade("frontalface", stages=20)#人脸识别的haar算子
eyes_cascade = image.HaarCascade("eye", stages=24)#眼睛的haar算子
#image.HaarCascade(path, stages=Auto)加载一个haar模型。haar模型是二进制文件，
#这个模型如果是自定义的，则引号内为模型文件的路径；也可以使用内置的haar模型，
#比如“frontalface” 人脸模型或者“eye”人眼模型。
#stages值未传入时使用默认的stages。stages值设置的小一些可以加速匹配，但会降低准确率。
print(face_cascade, eyes_cascade)

# FPS clock
clock = time.clock()

count = 0

#人脸识别部分
NUM_SUBJECTS = 4 #图像库中不同人数，一共4人

camera_flag = 0

class collect_face:

    def collect(self):
        sensor.reset() # Initialize the camera sensor.
        sensor.set_pixformat(sensor.GRAYSCALE) # or sensor.GRAYSCALE
        sensor.set_framesize(sensor.B128X128) # or sensor.QQVGA (or others)
        sensor.set_windowing((92,112))
        sensor.skip_frames(10) # Let new settings take affect.
        sensor.skip_frames(time = 2000)

        camera_flag = 1

        green_led.off()

        num = 4 #设置被拍摄者序号，第一个人的图片保存到s1文件夹，第二个人的图片保存到s2文件夹，以此类推。每次更换拍摄者时，修改num值。
        n = 2 #设置每个人拍摄图片数量。

        #连续拍摄n张照片，每间隔3s拍摄一次。
        while(n):
            #红灯亮
            #pyb.LED(RED_LED_PIN).on()
            red_led.on()
            sensor.skip_frames(time = 3000) # Give the user time to get ready.等待3s，准备一下表情。

            #红灯灭，蓝灯亮
            #pyb.LED(RED_LED_PIN).off()
            red_led.off()

            #pyb.LED(BLUE_LED_PIN).on()
            blue_led.on()

            #保存截取到的图片到SD卡
            print(n)
            sensor.snapshot().save("singtown/s%s/%s.pgm" % (num, n) ) # or "example.bmp" (or others)

            n -= 1

            #pyb.LED(BLUE_LED_PIN).off()
            blue_led.off()
            print("Done! Reset the camera to see the saved image.")
        num+=1
        camera_flag = 0               #切换回追踪的像素



class camera_back:
    def windows_back(self):
        sensor.reset()
        sensor.set_contrast(1)
        sensor.set_gainceiling(16)
        sensor.set_framesize(sensor.HQVGA)
        sensor.set_pixformat(sensor.GRAYSCALE)
        sensor.skip_frames(10) # Let new settings take affect.
        sensor.skip_frames(time = 2000) #等待2s
        camera_flag = 0
        print(camera_flag)



class face_rendering:               #人脸识别

    #def __init__(self)
        #self.num =0
    def min(self,pmin, a, s):
        if a<pmin:
            pmin=a
            self.num=s
        return pmin
    def start_face_rendering(self):
        sensor.reset() # Initialize the camera sensor.
        sensor.set_pixformat(sensor.GRAYSCALE) # or sensor.GRAYSCALE
        sensor.set_framesize(sensor.B128X128) # or sensor.QQVGA (or others)
        sensor.set_windowing((92,112))
        sensor.skip_frames(10) # Let new settings take affect.
        sensor.skip_frames(time = 5000) #等待5s

        camera_flag = 1
        print(camera_flag)

        green_led.off()

        #将蓝灯赋值给变量led
        #led = pyb.LED(3) # Red LED = 1, Green LED = 2, Blue LED = 3, IR LEDs = 4.

        #SUB = "s1"
        #NUM_SUBJECTS = 3 #图像库中不同人数，一共6人
        NUM_SUBJECTS_IMGS = 17 #每人有17张样本图片

        #blue_led.on()        #蓝灯亮起表示开始识别

        # 拍摄当前人脸。
        img = sensor.snapshot()
        #img = image.Image("singtown/%s/1.pgm"%(SUB))
        d0 = img.find_lbp((0, 0, img.width(), img.height()))
        #d0为当前人脸的lbp特征
        img = None
        pmin = 999999
        self.num=0

        for s in range(1, NUM_SUBJECTS+1):
            dist = 0
            for i in range(2, NUM_SUBJECTS_IMGS+1):
                img = image.Image("singtown/s%d/%d.pgm"%(s, i))
                d1 = img.find_lbp((0, 0, img.width(), img.height()))
                #d1为第s文件夹中的第i张图片的lbp特征
                dist += image.match_descriptor(d0, d1)#计算d0 d1即样本图像与被检测人脸的特征差异度。
            print("Average dist for subject %d: %d"%(s, dist/NUM_SUBJECTS_IMGS))
            pmin = self.min(pmin, dist/NUM_SUBJECTS_IMGS, s)#特征差异度越小，被检测人脸与此样本更相似更匹配。
            print(pmin)

        print(self.num) # num为当前最匹配的人的编号。
       # blue_led.off()          #识别完成，关闭蓝灯

        if pmin<=8000:
            if self.num==1:                  #old任世豪
                data = bytearray([0xb1,0xb1,200,200,0x5b])       #200
                uart.write(data)

            if self.num==2:                  #王旭
                data = bytearray([0xb1,0xb1,201,200,0x5b])       #201
                uart.write(data)

            if self.num==3:                  #伍郭成
                data = bytearray([0xb1,0xb1,202,200,0x5b])       #202
                uart.write(data)

            if self.num==4:                  #任世豪
                data = bytearray([0xb1,0xb1,203,200,0x5b])       #203
                uart.write(data)

            #uart.write("a")
            green_led.on()                   #识别通过，亮绿灯
            time.sleep(3500)                 #延时1500ms
            green_led.off()

        else:
            data = bytearray([0xb1,0xb1,204,200,0x5b])          #204--#识别失败
            uart.write(data)
            red_led.on()                     #识别失败，亮红灯
            time.sleep(3500)                 #延时1500ms
            red_led.off()

        camera_flag = 0               #切换回追踪的像素
        sensor.reset()
        sensor.set_contrast(1)
        sensor.set_gainceiling(16)
        sensor.set_framesize(sensor.HQVGA)
        sensor.set_pixformat(sensor.GRAYSCALE)
        sensor.skip_frames(10) # Let new settings take affect.
        sensor.skip_frames(time = 2000) #等待2s
        print(camera_flag)



t = collect_face()
xu = face_rendering(0)
back = camera_back(0)


c = "ss"


#lcd.init() # Initialize the lcd screen.

while (True):
    clock.tick()

    # Capture snapshot
    #捕获快照
    img = sensor.snapshot()

    #y.start_face_rendering()

    #print("Face recognition")
    #y.start_face_rendering()

    lcd.display(img) # Take a picture and display the image.

    # Find a face !
    # Note: Lower scale factor scales-down the image more and detects smaller objects.
    # Higher threshold results in a higher detection rate, with more false positives.
    objects = img.find_features(face_cascade, threshold=1.0, scale=1.5)
    # 先利用haar算子找到视野中的人脸。image.find_features(cascade, threshold=0.5, scale=1.5),thresholds越大，
    # 匹配速度越快，错误率也会上升。scale可以缩放被匹配特征的大小。

    #返回矩形元组(x, y, w, h)-----------x,y为起始坐标点, w为矩形宽度, h为矩形高度。
    #矩形元组以数组形式调用。

    # Draw faces
    #将找到的人脸用矩形标记出来

    if uart.any():                 #接收来自STM32的数据
        c=uart.readline()
        print(c[0])

    if c[0]== 49:                  #'1'   人脸识别
        print("Face recognition")
        blue_led.on()
        xu.start_face_rendering()
        blue_led.off()
        c = "66"

    if c[0]== 50:                  #'2'   人脸库采集
        print("Face acquisition")
        blue_led.on()
        t.collect()

        blue_led.off()
        c = "66"

    #c = "66"                       #将c的值改变，防止重复进入执行

    for face in objects:
        img.draw_rectangle(face)

        green_led.on()
        count = 0

        #img.draw_cross( (int)((  (int)face[0] + (int)face[2]  )/2) ), (int)(( (int)face[1] + (int)face[3] )/2))    #在矩形框中心画十字
        img.draw_cross( (int)((face[0]+face[2])/1.3) , (int)((face[1]+face[3])/1.3) )    #在矩形框中心画十字

        #img.draw_string(100,40,'x=' + str((int)(((face[0]+face[2])/2)*255/320))   #在image上显示坐标
        #img.draw_string(100,60,'y=' + str(((face[1]+face[3])/2))))

        #img.draw_string(100,40,'x=' + str((int)(((face[0]+face[2])/2))))   #在image上显示坐标
        img.draw_string(100,40,'x=' + str((int)((((face[0]+face[2])/1.3)))))   #在image上显示坐标*255/320
        img.draw_string(100,60,'y=' + str((int)((face[1]+face[3])/1.3)))

        lcd.display(img) # Take a picture and display the image.

        #在终端打印坐标
        print('x=',(int)((((face[0]+face[2])/1.3))),'y=',(int)((face[1]+face[3])/1.3))#*255/320

        #向STM32发送坐标
        x = int(((face[0]+face[2])/1.3))    #*255/320
        y = int((face[1]+face[3])/1.3)
        data = bytearray([0xb3,0xb3,x,y,0x5b])
        uart.write(data)

        # Now find eyes within each face.
        # 现在找出每一张脸的眼睛。
        # Note: Use a higher threshold here (more detections) and lower scale (to find small objects)
        # 注意:这里使用更高的阈值(更多的检测)和更低的尺度(寻找小物体)
        #eyes = img.find_features(eyes_cascade, threshold=0.5, scale=1.2, roi=face)
        #在人脸中识别眼睛。roi参数设置特征寻找的范围，roi=face即在找到的人脸中识别眼睛。
        #将找到的眼睛标记出来
        #for e in eyes:
         #   img.draw_rectangle(e)

    count = count+1
    if count == 15:
        green_led.off()

    # 打印FPS。
    # 注意:实际FPS更高，流FB使它更慢。
    #print(clock.fps())
    #print(camera_flag)

