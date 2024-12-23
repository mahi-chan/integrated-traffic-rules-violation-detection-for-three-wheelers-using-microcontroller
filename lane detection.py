import cv2
import numpy as np
import time

# Load pre-trained lane detection model (e.g., Hough Transform, deep learning model, etc.)
# For demonstration purposes, we'll use a simple edge detection approach here.
def detect_lanes(frame):
    # Convert frame to grayscale
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Apply edge detection (e.g., Canny edge detection)
    edges = cv2.Canny(gray, 50, 150)

    # Define region of interest (ROI)
    height, width = edges.shape
    roi_vertices = [(0, height), (width // 2, height // 2), (width, height)]
    mask = np.zeros_like(edges)
    cv2.fillPoly(mask, [np.array(roi_vertices)], 255)
    masked_edges = cv2.bitwise_and(edges, mask)

    # Detect lines using Hough Transform (simplified for demonstration)
    lines = cv2.HoughLinesP(masked_edges, 1, np.pi / 180, threshold=50, minLineLength=100, maxLineGap=10)

    return lines

# Example function to check if car is changing to the right lane
def check_right_lane_change(lines):
    if lines is not None:  # Check if lines were detected
        # For demonstration, assume that if any line has a positive slope, it indicates a right lane change
        for line in lines:
            x1, y1, x2, y2 = line[0]
            slope = (y2 - y1) / (x2 - x1 + 1e-6)  # Avoid division by zero
            if slope > 0:
                return True
    return False

# Example video stream processing loop using webcam
def process_webcam_stream():
    cap = cv2.VideoCapture(0)  # Use the default webcam (change index if needed)
    start_time = None
    in_right_lane = False
    warning_count = 0
    left_line_detected = False  # Initialize the variable
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        # Detect lanes
        lines = detect_lanes(frame)

        # Check for right lane change
        if check_right_lane_change(lines):
            if not in_right_lane:
                if start_time is None:
                    start_time = time.time()
                else:
                    elapsed_time = time.time() - start_time
                    if elapsed_time >= 2:  # Car in right lane for 2 seconds
                        # Check if there's a straight line on the left (parking lane)
                        left_line_detected = any(line[0][0] < frame.shape[1] // 2 for line in lines)
                        if not left_line_detected:
                            print("Warning!")
                            in_right_lane = True
                            warning_count += 1
        else:
            start_time = None  # Reset timer if not in right lane
            in_right_lane = False

        # Color the left lane in yellow (assuming a straight line)
        if left_line_detected:
            cv2.line(frame, (0, frame.shape[0]), (frame.shape[1] // 2, frame.shape[0] // 2), (0, 255, 255), 5)

        # Display processed frame in a window
        cv2.imshow("Lane Detection", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

    print(f"Total warnings: {warning_count}")

# Run the webcam stream processing
process_webcam_stream()
