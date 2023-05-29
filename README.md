## Dormitory-smart-access-control

### 关于该项目的一些说明

1、主控芯片为STM32F103C8T6，使用OpenMV实现人脸识别，指纹传感器使用了AS608。

2、供电方式有DC12V供电和备用电池供电，备用电池采用单节18650锂电池和7.2V镍镉电池，并且该主控板具备为锂电池充电的功能。此外这两种供电模式之间可以实现自动切换，当外部12V供电因停电等原因停止供电时，该系统会自动切换到备用电池供电，无需手动切换。

3、摄像头搭载在二维云台上，能够自动调节位置对准人脸拍摄并识别，无需人为将脸对准摄像头，提高了使用的便捷性。

4、人脸识别和指纹识别两种模式可以手动切换，任意一种通过识别即可通过门禁。

5、识别过程和识别结果通过LCD显示屏展示。

---

### 项目展示

![caip1](https://github.com/yuanheci/Dormitory-smart-access-control/assets/97277559/78a78028-a07b-4560-9cde-720f728d7e52)

---

![caip2](https://github.com/yuanheci/Dormitory-smart-access-control/assets/97277559/eabaea9d-6f64-4565-87ef-c9b0ee823e73)
