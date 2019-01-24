#! /usr/bin/env python3

#
#    ggmonitor.py - App for testing the gyroglove
#    https://github.com/Lumorti/gyroGlove
#

# Load the GUI framework
import tkinter as tk
from PIL import Image, ImageTk

# Allow graphs in the program
import numpy as np
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
import matplotlib.animation as animation
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")

# For comms with the Arduino
import serial
import time
import io

import platform

import math

connected = False

accelData = [[], [], []]
gyroData = [[], [], []]
buttonData = [0, 0, 0, 0, 0]
oldButtonData = [1, 1, 1, 1, 1]
ledColour = "l"
oldLedColour = "w"

updateString = "0,0,0,0,0,0,0,0,0,0,0,l"

relX = 0
relY = -45

maxPoints = 100
port = None
count = 1
osName = "linux"

def attempConnection(portName):

    global port

    try:
        port = serial.Serial(portName, 9600, timeout=2)
    except Exception as e:
        return False

    # If some connection has been made, ensure this is the right device
    if port:

        print("found port on " + str(portName))

        # Empty the serial buffer
        port.reset_input_buffer()

        return True

    return False

def updateGUI():

    global connected
    global count
    global root

    # If not connected, attempt to connect
    if not connected:

        print("attemping serial connection (attempt " + str(count) + ")")
        count += 1

        if osName == "Windows":
            connected = attempConnection('COM1')
            if not connected: connected = attempConnection('COM2')
        else:
            connected = attempConnection('/dev/ttyACM0')
            if not connected: connected = attempConnection('/dev/ttyACM1')

        root.after(1000, updateGUI)

    else:

        global oldButtonData
        global oldLedColour
        global canvas

        if port.in_waiting:

            # Get the update from the Serial input buffer
            rawString = port.readline()

            splitString = []

            try:

                updateString = str(rawString.decode("utf-8")).replace("\n", "").replace("\r", "")
                updateString = updateString.replace("\'", "").replace("--", "-")

                # Process the update string
                splitString = updateString.split(",")

            except Exception as e:

                pass

            print(splitString)

            if len(splitString) == 12:

                # Update the graph data sets
                for i in range(3):

                    accelData[i].append(int(splitString[i]))
                    if len(accelData[i]) > maxPoints: accelData[i] = accelData[i][1:maxPoints+1]

                    gyroData[i].append(int(splitString[i+3]))
                    if len(gyroData[i]) > maxPoints: gyroData[i] = gyroData[i][1:maxPoints+1]

                # Update the button info
                for i in range(5):
                    buttonData[i] = int(splitString[6+i])

                # Update the LED info
                ledColour = splitString[11]

                # Update the x/y/z accel graphs
                for i in range(3):
                    accelLines[i].set_data(np.arange(0, len(accelData[i]), 1), accelData[i])

                # Update the x/y/z gyro graphs
                for i in range(3):
                    gyroLines[i].set_data(np.arange(0, len(gyroData[i]), 1), gyroData[i])

                canvas.draw()

                # Check if the hand has changed at all
                handChanged = False
                for i in range(len(buttonData)):
                    if buttonData[i] != oldButtonData[i]:
                        handChanged = True
                        break

                # Check if the LED has changed at all
                if ledColour != oldLedColour:
                    handChanged = True

                # If one of those has changed, update everything
                if handChanged:

                    print("redrawing hand")

                    # Create base hand image
                    newHand = Image.new('RGBA', (570, 700))
                    newHand.paste(palm, (100+relX, 280+relY), palm)

                    # Is each finger closed or open?
                    if buttonData[1] == 0: newHand.paste(indexOpen, (318+relX, 82+relY), indexOpen)
                    else: newHand.paste(indexClosed, (309+relX, 280+relY), indexClosed)
                    if buttonData[2] == 0: newHand.paste(middleOpen, (248+relX, 47+relY), middleOpen)
                    else: newHand.paste(middleClosed, (246+relX, 265+relY), middleClosed)
                    if buttonData[3] == 0: newHand.paste(ringOpen, (168+relX, 77+relY), ringOpen)
                    else: newHand.paste(ringClosed, (171+relX, 267+relY), ringClosed)
                    if buttonData[4] == 0: newHand.paste(littleOpen, (91+relX, 148+relY), littleOpen)
                    else: newHand.paste(littleClosed, (100+relX, 294+relY), littleClosed)
                    if buttonData[0] == 0: newHand.paste(thumbOpen, (386+relX, 318+relY), thumbOpen)
                    else: newHand.paste(thumbClosed, (273+relX, 349+relY), thumbClosed)

                    # LED indicator
                    ledPos = (250, 620)
                    newHand.paste(ledCharToCol(ledColour), [ledPos[0],ledPos[1],ledPos[0]+49,ledPos[1]+47])
                    newHand.paste(ledCover, (ledPos[0], ledPos[1]), ledCover)

                    # Full hand plus LED image
                    newHandImage = ImageTk.PhotoImage(newHand)
                    handPanel.configure(image=newHandImage)
                    handPanel.image = newHandImage

                    for i in range(len(buttonData)):
                        oldButtonData[i] = buttonData[i]

                    oldLedColour = ledColour

        root.after(1, updateGUI)

def ledCharToCol(ledChar):

    if ledChar == "r": return (255, 0, 0)
    elif ledChar == "g": return (0, 255, 0)
    elif ledChar == "b": return (0, 0, 255)
    elif ledChar == "y": return (255, 255, 0)
    elif ledChar == "c": return (0, 255, 255)
    elif ledChar == "m": return (255, 0, 255)
    elif ledChar == "w": return (255, 255, 255)
    elif ledChar == "o": return (255, 165, 0)
    elif ledChar == "l": return (0, 0, 0)

if __name__ == "__main__":

    # Get the OS
    osName = platform.system()

    # Set up the tkinter background
    root = tk.Tk()
    root.configure(bg="white")

    # Load images for the hand display
    palm = Image.open('images/palm.png')
    littleOpen = Image.open('images/littleOpen.png')
    littleClosed = Image.open('images/littleClosed.png')
    ringOpen = Image.open('images/ringOpen.png')
    ringClosed = Image.open('images/ringClosed.png')
    middleOpen = Image.open('images/middleOpen.png')
    middleClosed = Image.open('images/middleClosed.png')
    indexOpen = Image.open('images/indexOpen.png')
    indexClosed = Image.open('images/indexClosed.png')
    thumbOpen = Image.open('images/thumbOpen.png')
    thumbClosed = Image.open('images/thumbClosed.png')
    ledCover = Image.open('images/led.png')

    # Set up the canvas used for drawing the hand, with a blank area to start with
    handPanel = tk.Label(root, width = 570, height = 700, bd=0)
    hand = Image.new('RGBA', (570, 700))
    handImage = ImageTk.PhotoImage(hand)
    handPanel.configure(bg="white", image=handImage)
    handPanel.grid(row=0, column=1)

    graphs = plt.Figure(figsize=(6, 8), dpi=100,)

    canvas = FigureCanvasTkAgg(graphs, master=root)
    canvas.get_tk_widget().grid(row=0, column=0, rowspan=1)

    accelAxes = graphs.add_subplot(211)
    #accelAxes.get_xaxis().set_visible(False)
    accelAxes.set_xlim([0, maxPoints])
    accelAxes.set_ylim([-40000, 40000])
    accelLines = []

    gyroAxes = graphs.add_subplot(212)
    #gyroAxes.get_xaxis().set_visible(False)
    gyroAxes.set_xlim([0, maxPoints])
    gyroAxes.set_ylim([-40000, 40000])
    gyroLines = []

    x = np.arange(0, maxPoints, 1)

    for i in range(3):
        gyroData[i] = list(np.zeros(maxPoints))
        accelData[i] = list(np.zeros(maxPoints))
        accelLines.append(accelAxes.plot(x, accelData[i])[0])
        gyroLines.append(gyroAxes.plot(x, gyroData[i])[0])

    accelLines[0].set_label('x')
    accelLines[1].set_label('y')
    accelLines[2].set_label('z')
    gyroLines[0].set_label('x')
    gyroLines[1].set_label('y')
    gyroLines[2].set_label('z')

    accelAxes.legend()
    gyroAxes.legend()

    graphs.tight_layout()
    graphs.subplots_adjust(bottom=0.05, top=0.97, right=0.95)

    root.after(10, updateGUI)
    root.mainloop()
