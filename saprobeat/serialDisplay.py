import random
import pygame
import time
import serial

# speed of changing
Δ = 1

# Establish serial connection with Arduino
# ser = serial.Serial('/dev/tty.usbmodem101', 9600)
ser = serial.Serial('/dev/tty.usbserial-10', 115200)

class Display(object):
    def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
        self.r = r
        self.g = g
        self.b = b
        self.delay = delay
        self.fade = fade
        self.running = 0
        self.last_update_time = time.time()
        self.update_interval = 1

    def display(self):
        font_px = 15
        pygame.init()
        screen_info = pygame.display.Info()
        width = screen_info.current_w
        height = screen_info.current_h

        winsur = pygame.display.set_mode((width, height))
        font = pygame.font.SysFont('andalemono', 21)
        bg = pygame.Surface((width, height), pygame.SRCALPHA, 32).convert_alpha()
        screen = pygame.di /splay.set_mode((width, height), pygame.FULLSCREEN) # full screen
        winsur.fill((0, 0, 0))

        LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'

        column = int(width / font_px)
        drops = [0 for _ in range(column)]

        self.running = 1
        while self.running:
            texts = [font.render(LETTER[i], 1, (self.r, self.g, self.b)) for i in range(44)]

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = 0
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_SPACE:
                        self.running = 0

            pygame.time.delay(self.delay)
            bg.fill(pygame.Color(0, 0, 0, self.fade))

            winsur.blit(bg, (0, 0))

            for i in range(len(drops)):
                text = random.choice(texts)
                winsur.blit(text, (i * font_px, drops[i] * font_px))
                drops[i] += 1
                if drops[i] * 10 > height or random.random() > 0.95:
                    drops[i] = 0

            pygame.display.flip()

    def handle_update_interrupt(self, bpm, vol, freq):
        if time.time() - self.last_update_time >= self.update_interval:
            self.last_update_time = time.time()
            self.update(bpm, vol, freqs)

    def update(self):

        # fade [0, 256] <-> vol [0, 120]
        self.fade = 256 - vol*32/15

        # delay [0, 69] <-> bpm [74, 220]
        # f(196) = 6
        # f(99) = 40
        # f(91) = 50
        # f(74) = 69
        self.delay = .003 * (bpm-210)**2 + 5


        # # rgb <-> freqs
        # # low (< 246Hz)
        # self.r = int(256*)
        # # mid (246 ~ 2500Hz)
        # self.g = int(256*)
        # # hi (> 2500Hz)
        # self.b = int(256)


        # # random
        self.r = (self.r + random.randint(-Δ, Δ)) % 256
        self.g = (self.g + random.randint(-Δ, Δ)) % 256
        self.b = (self.b + random.randint(-Δ, Δ)) % 256
        # self.delay = (self.delay + random.randint(-Δ, Δ)) % 69
        # self.fade = (self.fade + random.randint(-Δ, Δ)) % 200


if __name__ == '__main__':
    d = Display()
    d.display()
    try:
        while 1:
            # Read data from serial
            data = ser.readline().strip().decode()
            vol, freq = map(float, data.split(','))

            # Split data into volume and frequency
            # bpm, vol, freq = map(float, data.split(','))

            d.handle_update_interrupt(bpm, vol, freq)

    except KeyboardInterrupt:
        # Close serial connection when Ctrl+C is pressed
        ser.close()
    print("Serial connection closed.")


