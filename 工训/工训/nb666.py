# 二维码例程
#
# 这个例子展示了OpenMV Cam使用镜头校正来检测QR码的功能（请参阅qrcodes_with_lens_corr.py脚本以获得更高的性能）。
import sensor, image, time

from pyb import UART
uart = UART(3,9600)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # 必须关闭此功能，以防止图像冲洗…
clock = time.clock()


while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(1.8) # 1.8的强度参数对于2.8mm镜头来说是不错的。
    for code in img.find_qrcodes():
        a=code.payload()
        FH = bytearray([0xFF,int(a[0]),int(a[1]),int(a[2]),int(a[4]),int(a[5]),int(a[6]),0xFE])
        uart.write(FH)
        print(FH)


