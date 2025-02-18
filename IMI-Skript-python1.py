import pyautogui
import serial
import time
import pygame
import tkinter as tk
from PIL import Image, ImageTk
import os
import ctypes

# Einstellungen
COM_PORT = 'COM4'
BAUD_RATE = 9600
SEND_INTERVAL = 0.25  # Intervall in Sekunden (20 ms)
BILDER_PFAD = "bilder"  # Ordner, in dem die Bilder gespeichert sind
AUDIO_FILE = "Audio/07 - Parabola.mp3"  # Pfad zur Audiodatei

# Initialisierung
pygame.mixer.init()
current_background = 0
bilder = []

# Bilder laden
for i in range(10):
    pfad = os.path.join(BILDER_PFAD, f"bild_{i}.png")
    if os.path.exists(pfad):
        bilder.append(pfad)
    else:
        bilder.append(None)

if not any(bilder):
    print("Keine Bilder gefunden. Bitte Bilder im Ordner 'bilder' ablegen und korrekt benennen.")
    exit()

# GUI-Setup
def setze_bild(index):
    global current_background
    if bilder[index]:
        bild = Image.open(bilder[index])
        bild = bild.resize((root.winfo_screenwidth(), root.winfo_screenheight()))
        foto = ImageTk.PhotoImage(bild)
        label.config(image=foto)
        label.image = foto
        current_background = index
    else:
        print(f"Bild {index} fehlt.")

root = tk.Tk()
root.attributes("-fullscreen", True)
root.configure(bg="black")
root.config(cursor="none")  # Mauszeiger ausblenden
label = tk.Label(root, bg="black")
label.pack(fill=tk.BOTH, expand=True)

# Serielle Kommunikation
try:
    ser = serial.Serial(COM_PORT, BAUD_RATE)
    print(f"Verbunden mit {COM_PORT}")
except serial.SerialException as e:
    print(f"Fehler beim Verbinden mit {COM_PORT}: {e}")
    exit()

# Audiodatei laden
try:
    pygame.mixer.music.load(AUDIO_FILE)
except pygame.error as e:
    print(f"Fehler beim Laden der Audiodatei: {e}")
    exit(1)

def handle_serial_data(data):
    global current_background
    if data.startswith("Q") and data.endswith("G") and len(data) == 3:
        if data[1] == "1" and not pygame.mixer.music.get_busy():
            pygame.mixer.music.play()
        elif data[1] == "0" and pygame.mixer.music.get_busy():
            pygame.mixer.music.stop()
    elif data.startswith("Ü") and data.endswith("Ä") and len(data) == 3:
        try:
            index = int(data[1])
            if 0 <= index < len(bilder) and index != current_background:
                setze_bild(index)
            else:
                print(f"Ungültiger oder gleicher Index: {index}")
        except ValueError:
            print(f"Ungültige Daten: {data}")
    else:
        print(f"Unbekannter Befehl: {data}")

def empfange_daten():
    try:
        if ser.in_waiting > 2:
            raw_data = ser.readline().decode("utf-8").strip()
            handle_serial_data(raw_data)
            ser.reset_input_buffer()
    except Exception as e:
        print(f"Fehler beim Empfangen der Daten: {e}")
    root.after(100, empfange_daten)

def beenden(event):
    print("Beenden durch ESC-Taste.")
    ser.close()
    pygame.mixer.quit()
    root.destroy()

# Mausposition senden
def sende_mausposition():
    x, y = pyautogui.position()
    position = f"S{x:04d}{y:04d}X"
    try:
        ser.write(position.encode('utf-8'))
        print(f"Gesendet: {position}")
    except Exception as e:
        print(f"Fehler beim Senden der Mausposition: {e}")
    root.after(int(SEND_INTERVAL * 1000), sende_mausposition)

# Hauptschleife
setze_bild(0)  # Standardbild setzen
root.bind("<Escape>", beenden)
root.after(100, empfange_daten)
root.after(int(SEND_INTERVAL * 1000), sende_mausposition)
root.mainloop()

# Serielle Verbindung schließen
ser.close()
pygame.mixer.quit()
