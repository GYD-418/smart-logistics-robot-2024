import cv2
import numpy as np
# import serial
# import time

def get_color_mask(hsv_frame, lower_bound, upper_bound):
    mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)
    kernel = np.ones((5, 5), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    return mask

def find_largest_contour(contours, min_area=1000):
    if contours:
        max_contour = max(contours, key=cv2.contourArea)
        if cv2.contourArea(max_contour) > min_area:
            return max_contour
    return None

def main():
    cap = cv2.VideoCapture(0)

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        frame = cv2.GaussianBlur(frame, (15, 15), 0)
        hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # 定义颜色范围
        lower_red1 = np.array([0, 120, 70])
        upper_red1 = np.array([10, 255, 255])
        lower_red2 = np.array([170, 120, 70])
        upper_red2 = np.array([180, 255, 255])
        lower_green = np.array([35, 100, 100])
        upper_green = np.array([85, 255, 255])
        lower_blue = np.array([100, 150, 0])
        upper_blue = np.array([140, 255, 255])

        # 获取颜色掩码
        red_mask1 = get_color_mask(hsv_frame, lower_red1, upper_red1)
        red_mask2 = get_color_mask(hsv_frame, lower_red2, upper_red2)
        red_mask = cv2.bitwise_or(red_mask1, red_mask2)
        green_mask = get_color_mask(hsv_frame, lower_green, upper_green)
        blue_mask = get_color_mask(hsv_frame, lower_blue, upper_blue)

        # 找到轮廓
        contours_red, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours_green, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours_blue, _ = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

        # 找到最大轮廓
        max_contour_red = find_largest_contour(contours_red)
        max_contour_green = find_largest_contour(contours_green)
        max_contour_blue = find_largest_contour(contours_blue)

        # 初始化颜色x坐标字典
        color_x_dict = {'red': 0, 'green': 0, 'blue': 0}
        #初始化颜色w坐标字典
        color_w_dict = {'red':0,'green': 0,'blue': 0}
        #初始化color_byte为默认值
        color_byte = 0x00

        for color, max_contour, color_code in [('red', max_contour_red, (0, 0, 255)),
                                               ('green', max_contour_green, (0, 255, 0)),
                                               ('blue', max_contour_blue, (255, 0, 0))]:
            if max_contour is not None:
                (x, y, w, h) = cv2.boundingRect(max_contour)
                M = cv2.moments(max_contour)
                if M["m00"] != 0:
                    cx = int(M["m10"] / M["m00"])  # 计算中心x坐标
                    cy = int(M["m01"] / M["m00"])  # 计算中心y坐标
                    color_x_dict[color] = cx  # 存储x坐标
                    cv2.rectangle(frame, (x, y), (x + w, y + h), color_code, 3)
                    cv2.circle(frame, (cx, cy), 5, (255, 255, 255), -1)
                    cv2.putText(frame, color, (cx, cy), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255, 255, 255), 3)

                    # 根据颜色设置color_byte
                    if color == 'red':
                        color_byte = 0x01
                    elif color == 'green':
                        color_byte = 0x02
                    elif color == 'blue':
                        color_byte = 0x44

        # 获取各个位的值
        def get_digit_values(number):
            hundreds = number // 100
            tens = (number % 100) // 10
            ones = number % 10
            return hundreds, tens, ones

        # 获取每个颜色x坐标的各个位
        red_h, red_t, red_o = get_digit_values(color_x_dict['red'])
        green_h, green_t, green_o = get_digit_values(color_x_dict['green'])
        blue_h, blue_t, blue_o = get_digit_values(color_x_dict['blue'])
        #获取每个颜色w的值的每一位
        redw_h, redw_t, redw_o = get_digit_values(color_w_dict['red'])
        greenw_h, greenw_t, greenw_o = get_digit_values(color_w_dict['green'])
        bluew_h, bluew_t, bluew_o = get_digit_values(color_w_dict['blue'])
        # 构建发送的数据
        data = bytearray([0xFF, red_h, green_h, blue_h, red_t, green_t, blue_t, red_o, green_o, blue_o,
                          redw_h, greenw_h, bluew_h, redw_t, greenw_t, bluew_t, redw_o, greenw_o, bluew_o,color_byte, 0xFE])
        print(f"Generated data: {data}")

        # com.write(data)  # 通过串口发送数据
        print(f"{data}")  # 打印发送的数据

        # 显示图像
        cv2.imshow('Original', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    # com.close()

if __name__ == "__main__":
    main()