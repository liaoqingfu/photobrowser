#ifndef ADDRESS_H
#define ADDRESS_H

#define COM tr("/dev/ttymxc2")
#define PCCOM "dev/ttySAC2"

//从机地址
#define SLAVE tr("00")

//功能代码
#define READ tr("03")
#define WRITE tr("06")

//摇杆使用者地址
#define Y_ADDRESS tr("0001")
//摇杆状态
#define MASTER_Y tr("0001")
#define SLAVE_Y tr("0000")


//舵机最大角度(0~1000)
#define RUDDER_ADDRESS tr("0002")

//LED最大电流(0~500mA)
#define A_ADDRESS tr("0003")


//电池状态
#define BAT_ADDRESS tr("0004")

#define BAT_LOW tr("1")  //电量不足
#define BAT_CH tr("1")  //充电


//按键地址
#define KEY_ADDRES tr("0005")
//按键键值
#define POWER_KEY tr("0000") //关机
#define PHOTO_KEY tr("0001") //拍照
#define UP_KEY tr("0004") //上键
#define LEFT_KEY tr("0005") //左键
#define DOWN_KEY tr("0006") //下键
#define RIGHT_KEY tr("0007") //右键
#define OK_KEY tr("0008") //中键

#define UP_LONG_KEY tr("000C") //上键长按
#define LEFT_LONG_KEY tr("000D") //左键长按
#define DOWN_LONG_KEY tr("000E") //下键长按
#define RIGHT_LONG_KEY tr("000F") //右键长按
#define OK_LONG_KEY tr("0010") //中键长按

#define NONE_KEY tr("00FF") //无按键


//led亮度地址
#define LED_ADDRESS tr("0006")

//LED弱光亮度等级
#define LED_WEAK_0 tr("0000")
#define LED_WEAK_1 tr("0001")
#define LED_WEAK_2 tr("0002")
#define LED_WEAK_3 tr("0003")
#define LED_WEAK_4 tr("0004")
#define LED_WEAK_5 tr("0005")

//LED强光亮度等级
#define LED_STRONG_0 tr("0006")
#define LED_STRONG_1 tr("0007")
#define LED_STRONG_2 tr("0008")
#define LED_STRONG_3 tr("0009")
#define LED_STRONG_4 tr("000A")
#define LED_STRONG_5 tr("000B")

//摇杆X坐标(100~3200) 中心值 1650
#define ROCKER_X_ADDRESS tr("0007")

//摇杆Y坐标(100~3200) 中心值 1650
#define ROCKER_Y_ADDRESS tr("0008")



/*########电量显示功能#########*/
#define ELEC_NOT_ENOUGH tr("1")
#define ELEC_CHARGE tr("1")
#define ELEC_NORMAL tr("0")


/*########摇杆边界#######*/
#define CENTER_X 1630
#define CENTER_Y 1645
#define X_MIN_STEP 200
#define Y_MIN_STEP 200

#endif // ADDRESS_H
