import serial
import time 
import matplotlib.pyplot as plt 
import matplotlib.animation as animation
from collections import deque

ser = serial.Serial('/dev/cu.usbmodem101', 115200, timeout=1)
time.sleep(2)

N = 200          # liczba punktów na wykresie

acc_x = deque([0] * N, maxlen=N)    #listy zapisujace poprzednie 200 pomiarow z akcelerometru
acc_y = deque([0] * N, maxlen=N)
acc_z = deque([0] * N, maxlen=N)

gyro_x = deque([0] * N, maxlen=N)   #listy zapisujace poprzednie 200 pomiarow z gyro
gyro_y = deque([0] * N, maxlen=N)
gyro_z = deque([0] * N, maxlen=N)


fig, (acc_plot, gyro_plot) = plt.subplots(2, 1)

#ACC plot 

acc_plot.set_title("ACC")
acc_plot.set_xlabel("samples")
acc_plot.set_ylabel("value")

line_ax, = acc_plot.plot(range(N), acc_x, label="ax")
line_ay, = acc_plot.plot(range(N), acc_y, label="ay")
line_az, = acc_plot.plot(range(N), acc_z, label="az")

acc_plot.legend()

#Gyro plot 
gyro_plot.set_title("GYRO")
gyro_plot.set_xlabel("samples")
gyro_plot.set_ylabel("value")

line_gx, = gyro_plot.plot(range(N), gyro_x, label="gx")
line_gy, = gyro_plot.plot(range(N), gyro_y, label="gy")
line_gz, = gyro_plot.plot(range(N), gyro_z, label="gz")

gyro_plot.legend()

# wg matplotlib documentation
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
    gyro_x.append(gx)
    gyro_y.append(gy)
    gyro_z.append(gz)

    #aktualizacja wykresu
    line_ax.set_ydata(acc_x)
    line_ay.set_ydata(acc_y)
    line_az.set_ydata(acc_z)
    line_gx.set_ydata(gyro_x)
    line_gy.set_ydata(gyro_y)
    line_gz.set_ydata(gyro_z)

    # autoskalowanie wykresu
    acc_plot.relim()
    acc_plot.autoscale_view()
    gyro_plot.relim()
    gyro_plot.autoscale_view()

    return line_ax, line_ay, line_az, line_gx, line_gy, line_gz


ani = animation.FuncAnimation(fig, update, interval=100)

plt.tight_layout()
plt.show()
