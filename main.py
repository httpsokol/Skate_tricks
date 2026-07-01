import serial
import time 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
from collections import deque

ser = serial.Serial('/dev/cu.usbmodem101', 115200, timeout=1)
time.sleep(2)

N = 200          # liczba punktów na wykresie

acc_x = deque([0] * N, maxlen=N)    #listy zapisujace poprzednie 200 pomiarow 
acc_y = deque([0] * N, maxlen=N)
acc_z = deque([0] * N, maxlen=N)

fig, ax_plot = plt.subplots()
ax_plot.set_title("ACC")
ax_plot.set_xlabel("samples")
ax_plot.set_ylabel("value")

line_x, = ax_plot.plot(range(N), acc_x, label="ax")
line_y, = ax_plot.plot(range(N), acc_y, label="ay")
line_z, = ax_plot.plot(range(N), acc_z, label="az")

ax_plot.legend()


def update(frame):

    line = ser.readline().decode("utf-8").strip()

    try:
        ax, ay, az, gx, gy, gz = map(int, line.split(","))
    except:
        return

    # dodawanie najnowszych pomiarow do buforu
    acc_x.append(ax)
    acc_y.append(ay)
    acc_z.append(az)

    #aktualizacja wykresu
    line_x.set_ydata(acc_x)
    line_y.set_ydata(acc_y)
    line_z.set_ydata(acc_z)

    # autoskalowanie wykresu
    ax_plot.relim()
    ax_plot.autoscale_view()

    return line_x, line_y, line_z


ani = animation.FuncAnimation(fig, update, interval=100)

plt.tight_layout()
plt.show()
