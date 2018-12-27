#! /usr/bin/env python3

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

connected = False

accelData = [[], [], []]
gyroData = [[], [], []]
buttonData = [0, 0, 0, 0, 0]
oldButtonData = [1, 1, 1, 1, 1]
ledColour = (255, 0, 0)
oldLedColour = (255, 255, 255)

updateString = "0,0,0,0,0,0,0,0,0,0,0"

relX = 0
relY = -45

def update():

    global oldButtonData
    global oldLedColour

    handChanged = False
    for i in range(len(buttonData)):
        if buttonData[i] != oldButtonData[i]:
            handChanged = True
            break

    for i in range(len(ledColour)):
        if ledColour[i] != oldLedColour[i]:
            handChanged = True
            break

    if handChanged:

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
        newHand.paste(ledColour, [ledPos[0],ledPos[1],ledPos[0]+49,ledPos[1]+47])
        newHand.paste(ledCover, (ledPos[0], ledPos[1]), ledCover)

        newHandImage = ImageTk.PhotoImage(newHand)
        handPanel.configure(image=newHandImage)
        handPanel.image = newHandImage

        oldButtonData = buttonData
        oldLedColour = ledColour

    # Update graphs

    root.after(10, update)

if __name__ == "__main__":

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

    def animateAccel(i):

        # Update the x/y/z accel graphs
        accelLines[0].set_ydata(np.sin(x+i/20.0))
        accelLines[1].set_ydata(np.sin(x+10+i/20.0))
        accelLines[2].set_ydata(np.sin(x+20+i/20.0))

        return accelLines

    def animateGyro(i):

        # Update the x/y/z accel graphs
        gyroLines[0].set_ydata(np.sin(x+i/20.0))
        gyroLines[1].set_ydata(np.sin(x+10+i/20.0))
        gyroLines[2].set_ydata(np.sin(x+20+i/20.0))

        return gyroLines

    x = np.arange(0, 2*np.pi, 0.01)
    graphs = plt.Figure(figsize=(6, 8), dpi=100,)

    canvas = FigureCanvasTkAgg(graphs, master=root)
    canvas.get_tk_widget().grid(row=0, column=0, rowspan=1)

    accelAxes = graphs.add_subplot(211)
    gyroAxes = graphs.add_subplot(212)
    accelAxes.get_xaxis().set_visible(False)
    gyroAxes.get_xaxis().set_visible(False)
    accelLines = []
    gyroLines = []

    for i in range(3):
        accelLines.append(accelAxes.plot(x, np.sin(x))[0])
        gyroLines.append(gyroAxes.plot(x, np.sin(x))[0])

    accelAni = animation.FuncAnimation(graphs, animateAccel, interval=25, blit=True)
    gyroAni = animation.FuncAnimation(graphs, animateGyro, interval=25, blit=True)

    graphs.tight_layout()
    graphs.subplots_adjust(bottom=0.05, top=0.97, right=0.95)

    update()
    def callbackHand(e):
        global buttonData
        if buttonData[0] == 0: buttonData = [1, 1, 1, 1, 1]
        else: buttonData = [0, 0, 0, 0, 0]

    def callbackLed(e):
        global ledColour
        if ledColour[0] == 0: ledColour = (255, 0, 0)
        else: ledColour = (0, 0, 255)

    root.bind("<Return>", callbackHand)
    root.bind("<BackSpace>", callbackLed)

    tk.mainloop()
