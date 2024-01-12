import time
import pyautogui

def simulate_5_press():
    pyautogui.press('5')
    print("Pressed 5")

if __name__ == "__main__":
    # try:
    #     while True:

    #         simulate_5_press()
    #         time.sleep(300)  # 300 seconds = 5 minutes
    # except KeyboardInterrupt:
    #     print("Script stopped.")
    pyautogui.moveTo("Snowman_Tonic.png")
    pyautogui.doubleClick()