import cv2
import time
import numpy as np
from collections import deque


class KalmanFilter:
    """卡尔曼滤波器用于稳定目标位置预测"""

    def __init__(self):
        self.kf = cv2.KalmanFilter(4, 2)
        self.kf.transitionMatrix = np.array([
            [1, 0, 1, 0],
            [0, 1, 0, 1],
            [0, 0, 1, 0],
            [0, 0, 0, 1]
        ], np.float32)

        self.kf.measurementMatrix = np.array([
            [1, 0, 0, 0],
            [0, 1, 0, 0]
        ], np.float32)

        self.kf.processNoiseCov = 1e-4 * np.eye(4, dtype=np.float32)
        self.kf.measurementNoiseCov = 1e-1 * np.eye(2, dtype=np.float32)
        self.kf.errorCovPost = np.eye(4, dtype=np.float32)

    def update(self, measurement):
        if measurement is not None:
            self.kf.correct(np.array([[np.float32(measurement[0])], [np.float32(measurement[1])]]))
        prediction = self.kf.predict()
        return (prediction[0, 0], prediction[1, 0])


def detect_largest_rectangle():
    # 打开默认摄像头
    cap = cv2.VideoCapture(0)

    # 设置摄像头参数
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
    cap.set(cv2.CAP_PROP_FPS, 60)
    cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
    cap.set(cv2.CAP_PROP_AUTOFOCUS, 0)
    cap.set(cv2.CAP_PROP_FOCUS, 50)

    if not cap.isOpened():
        print("无法打开摄像头")
        return

    # 性能跟踪变量
    prev_time = time.time()
    fps = 0
    processing_times = deque(maxlen=30)

    # 跟踪和稳定性变量
    kalman = KalmanFilter()
    prev_center = None
    prev_gray = None
    trajectory = deque(maxlen=50)
    lost_frames = 0
    max_lost_frames = 10
    stabilization_factor = 0.85

    # 形态学核
    kernel = np.ones((3, 3), np.uint8)

    # 多尺度参数
    scales = [0.7, 0.85, 1.0, 1.15, 1.3]

    # 光流跟踪点
    track_points = None
    track_window = None
    term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03)

    # 创建调试窗口
    cv2.namedWindow("Largest Rectangle Detection", cv2.WINDOW_NORMAL)
    cv2.namedWindow("Edges", cv2.WINDOW_NORMAL)
    cv2.namedWindow("Debug", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("Largest Rectangle Detection", 800, 600)

    # 创建控制面板
    cv2.createTrackbar('Canny Sigma', 'Debug', 33, 100, lambda x: None)
    cv2.createTrackbar('Min Area', 'Debug', 4000, 10000, lambda x: None)
    cv2.createTrackbar('Stabilization', 'Debug', 85, 100, lambda x: None)

    try:
        while True:
            start_time = time.time()

            # 读取一帧图像
            ret, frame = cap.read()
            if not ret:
                print("无法获取图像")
                break

            # 计算FPS
            curr_time = time.time()
            fps = 1 / (curr_time - prev_time) if (curr_time - prev_time) > 0 else 0
            prev_time = curr_time

            # 获取控制面板参数
            sigma = cv2.getTrackbarPos('Canny Sigma', 'Debug') / 100.0
            min_area = cv2.getTrackbarPos('Min Area', 'Debug')
            stabilization_factor = cv2.getTrackbarPos('Stabilization', 'Debug') / 100.0

            # 转换为灰度图
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # 高斯模糊
            blurred = cv2.GaussianBlur(gray, (7, 7), 0)

            # 多尺度边缘检测
            all_contours = []
            for scale in scales:
                # 缩放图像
                if scale != 1.0:
                    scaled = cv2.resize(blurred, None, fx=scale, fy=scale)
                else:
                    scaled = blurred.copy()

                # 自适应Canny边缘检测
                v = np.median(scaled)
                lower_thresh = int(max(0, (1.0 - sigma) * v))
                upper_thresh = int(min(255, (1.0 + sigma) * v))
                edges = cv2.Canny(scaled, lower_thresh, upper_thresh)

                # 形态学操作
                edges = cv2.dilate(edges, kernel, iterations=1)
                edges = cv2.erode(edges, kernel, iterations=1)

                # 寻找轮廓
                contours, _ = cv2.findContours(edges.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

                # 将轮廓缩放到原始尺寸并确保数据类型正确
                if scale != 1.0:
                    scaled_contours = []
                    for cnt in contours:
                        # 缩放轮廓并转换为正确的数据类型
                        scaled_cnt = cnt * (1 / scale)
                        scaled_cnt = scaled_cnt.astype(np.int32)  # 确保是32位整数类型
                        scaled_contours.append(scaled_cnt)
                    contours = scaled_contours

                # 过滤掉无效轮廓
                valid_contours = []
                for cnt in contours:
                    # 确保轮廓有足够的点并且是正确的形状
                    if len(cnt) >= 4 and cnt.ndim == 3 and cnt.shape[1] == 1 and cnt.shape[2] == 2:
                        valid_contours.append(cnt)

                all_contours.extend(valid_contours)

            # 在原始图像上绘制边缘（用于显示）
            v_main = np.median(blurred)
            lower_thresh_main = int(max(0, (1.0 - sigma) * v_main))
            upper_thresh_main = int(min(255, (1.0 + sigma) * v_main))
            edges_main = cv2.Canny(blurred, lower_thresh_main, upper_thresh_main)
            edges_main = cv2.dilate(edges_main, kernel, iterations=1)
            edges_main = cv2.erode(edges_main, kernel, iterations=1)

            max_area = 0
            largest_rect = None
            found_rect = False

            # 遍历所有轮廓（多尺度合并后的）
            for contour in all_contours:
                try:
                    # 检查轮廓有效性
                    if len(contour) < 4:
                        continue

                    area = cv2.contourArea(contour)
                    if area < min_area:
                        continue

                    perimeter = cv2.arcLength(contour, True)
                    approx = cv2.approxPolyDP(contour, 0.03 * perimeter, True)

                    # 检查是否为凸四边形
                    if len(approx) == 4 and cv2.isContourConvex(approx):
                        # 计算矩形角度
                        vectors = []
                        for i in range(4):
                            pt1 = approx[i][0]
                            pt2 = approx[(i + 1) % 4][0]
                            vectors.append(np.array(pt2) - np.array(pt1))

                        # 计算相邻边的角度
                        angles = []
                        for i in range(4):
                            v1 = vectors[i]
                            v2 = vectors[(i + 1) % 4]
                            cos_angle = np.dot(v1, v2) / (np.linalg.norm(v1) * np.linalg.norm(v2) + 1e-5)
                            angle = np.arccos(np.clip(cos_angle, -1.0, 1.0))
                            angles.append(angle)

                        # 检查角度是否接近90度（允许±20度偏差）
                        valid_angles = sum(1 for angle in angles if np.abs(angle - np.pi / 2) < np.deg2rad(20))

                        if valid_angles >= 3:  # 至少3个角接近直角
                            if area > max_area:
                                max_area = area
                                largest_rect = approx
                                found_rect = True
                except Exception as e:
                    # 忽略处理单个轮廓时的错误
                    continue

            # 处理检测结果
            current_center = None

            if found_rect:
                lost_frames = 0

                # 获取角点
                corners = np.array([point[0] for point in largest_rect], dtype=np.float32)

                # 亚像素角点精确化
                criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.01)
                cv2.cornerSubPix(gray, corners, (10, 10), (-1, -1), criteria)

                # 计算精确中心点
                center_x = np.mean(corners[:, 0])
                center_y = np.mean(corners[:, 1])
                current_center = (center_x, center_y)

                # 更新轨迹
                trajectory.append((int(center_x), int(center_y)))

                # 更新光流跟踪窗口
                track_window = cv2.boundingRect(largest_rect)
                track_points = cv2.goodFeaturesToTrack(
                    gray,
                    maxCorners=200,
                    qualityLevel=0.01,
                    minDistance=10,
                    blockSize=7,
                    mask=np.zeros_like(gray)
                )

                # 绘制矩形和角点
                cv2.drawContours(frame, [largest_rect.astype(int)], -1, (0, 255, 0), 3)
                for (x, y) in corners:
                    cv2.circle(frame, (int(x), int(y)), 6, (0, 0, 255), -1)

            # 光流跟踪（当检测失败时）
            elif prev_gray is not None and track_points is not None and len(track_points) > 0:
                # 计算光流
                new_points, status, _ = cv2.calcOpticalFlowPyrLK(
                    prev_gray, gray, track_points, None, winSize=(15, 15),
                    maxLevel=2, criteria=term_crit
                )

                # 选择有效的点
                good_old = track_points[status == 1]
                good_new = new_points[status == 1]

                if len(good_new) > 5:
                    # 计算点的平均位移
                    displacement = np.mean(good_new - good_old, axis=0)

                    # 更新跟踪窗口
                    if track_window is not None:
                        x, y, w, h = track_window
                        track_window = (
                            int(x + displacement[0]),
                            int(y + displacement[1]),
                            w, h
                        )

                    # 更新跟踪点
                    track_points = good_new.reshape(-1, 1, 2)

                    # 绘制光流向量
                    for i, (new, old) in enumerate(zip(good_new, good_old)):
                        a, b = new.ravel()
                        c, d = old.ravel()
                        cv2.line(frame, (int(a), int(b)), (int(c), int(d)), (0, 255, 255), 2)
                        cv2.circle(frame, (int(a), int(b)), 3, (0, 255, 255), -1)

                    # 如果连续丢失帧数少，使用光流预测位置
                    if lost_frames < max_lost_frames and track_window is not None:
                        x, y, w, h = track_window
                        current_center = (x + w / 2, y + h / 2)
                        lost_frames += 1
                        found_rect = True  # 标记为找到（预测）

            # 更新卡尔曼滤波器
            if current_center is not None:
                # 使用卡尔曼滤波平滑位置
                kalman_center = kalman.update(current_center)

                # 帧间稳定性处理
                if prev_center is not None:
                    center_x = stabilization_factor * kalman_center[0] + (1 - stabilization_factor) * prev_center[0]
                    center_y = stabilization_factor * kalman_center[1] + (1 - stabilization_factor) * prev_center[1]
                else:
                    center_x, center_y = kalman_center

                prev_center = (center_x, center_y)

                # 绘制中心点和轨迹
                cv2.circle(frame, (int(center_x), int(center_y)), 10, (255, 0, 255), -1)

                # 绘制轨迹
                for i in range(1, len(trajectory)):
                    cv2.line(frame, trajectory[i - 1], trajectory[i], (255, 255, 0), 2)

                # 显示坐标和面积
                cv2.putText(frame, f"({int(center_x)},{int(center_y)})",
                            (int(center_x) + 15, int(center_y) - 15),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 0, 255), 2)
                if found_rect:  # 只有实际检测时才显示面积
                    cv2.putText(frame, f"面积: {int(max_area)}",
                                (int(center_x) + 15, int(center_y) + 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 255), 2)
                else:
                    cv2.putText(frame, "预测位置",
                                (int(center_x) + 15, int(center_y) + 20),
                                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (150, 150, 255), 2)
            else:
                # 未检测到矩形
                lost_frames += 1
                if lost_frames > max_lost_frames:
                    trajectory.clear()  # 清除轨迹
                    prev_center = None
                    track_points = None

            # 更新前一帧灰度图
            prev_gray = gray.copy()

            # 显示FPS和状态信息
            processing_time = time.time() - start_time
            processing_times.append(processing_time)
            avg_processing = np.mean(processing_times) if processing_times else 0

            cv2.putText(frame, f"FPS: {int(fps)}", (10, 30),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 2)
            cv2.putText(frame, f"处理时间: {avg_processing * 1000:.1f}ms", (10, 60),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
            cv2.putText(frame, f"阈值: {lower_thresh_main}-{upper_thresh_main}", (10, 90),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
            cv2.putText(frame, f"稳定性: {stabilization_factor:.2f}", (10, 120),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 255), 2)
            cv2.putText(frame,
                        f"状态: {'检测' if found_rect and lost_frames == 0 else '预测' if found_rect else '丢失'}",
                        (10, 150), cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                        (0, 255, 0) if found_rect and lost_frames == 0 else
                        (0, 255, 255) if found_rect else (0, 0, 255), 2)

            # 创建调试图像
            debug_img = np.zeros_like(frame)
            debug_img[:, :, 2] = edges_main  # 红色通道显示边缘

            # 显示轮廓
            cv2.drawContours(debug_img, all_contours, -1, (0, 255, 0), 1)

            # 显示检测窗口
            if track_window is not None:
                x, y, w, h = track_window
                cv2.rectangle(debug_img, (x, y), (x + w, y + h), (255, 255, 0), 2)

            # 显示图像
            cv2.imshow("Largest Rectangle Detection", frame)
            cv2.imshow("Edges", edges_main)
            cv2.imshow("Debug", debug_img)

            # 按键处理
            key = cv2.waitKey(1)
            if key == 27:  # ESC键退出
                break
            elif key == ord('r'):  # 重置跟踪
                trajectory.clear()
                prev_center = None
                track_points = None
                lost_frames = 0
                kalman = KalmanFilter()  # 重置卡尔曼滤波器

    except Exception as e:
        print(f"发生错误: {e}")
    finally:
        cap.release()
        cv2.destroyAllWindows()


if __name__ == "__main__":
    detect_largest_rectangle()