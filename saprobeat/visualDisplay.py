import random
import pygame
import numpy as np
from PIL import Image
import pyaudio


# CONSTENTS
Δ = 1
LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'
font_px = 15
FORMAT = pyaudio.paInt16
DURATION = 420
RATE = 44100
CHUNK = 2048
CHANNEL = 1
N = 10

def resize_image(img, nw, nh):
    img = Image.open(img)
    w, h = img.size
    r = min(nw/w, nh/h)
    return np.asarray(img.resize((int(r*w), int(r*h))))

# ----------------------------------------------------------------------
class Display(object):
    def __init__(self, r=0, g=255, b=0, delay=30, fade=21, imgs=[]):
        self.r = r
        self.g = g
        self.b = b
        self.delay = delay
        self.fade = fade
        self.running = 0
        self.imgs = [np.asarray(Image.open(img)) for img in imgs]

        # initialise pygame & friends
        pygame.init()
        self.screen_info = pygame.display.Info()
        self.width = self.screen_info.current_w
        self.height = self.screen_info.current_h
        self.winsur = pygame.display.set_mode((self.width, self.height))
        self.font = pygame.font.SysFont('andalemono', 21)
        self.bg = pygame.Surface((self.width, self.height),
            pygame.SRCALPHA, 32).convert_alpha()
        self.screen = pygame.display.set_mode((self.width, self.height), pygame.FULLSCREEN)
        self.winsur.fill((0, 0, 0))

        # flags
        self.rendering = 0

        self.stream_init()


    # initialise pyaudio & friends
    def stream_init(self):
        p = pyaudio.PyAudio()
        self.stream = p.open(format=FORMAT,
            channels=CHANNEL,
            rate=RATE,
            input=True,
            frames_per_buffer=CHUNK)


    def read_audio(self):
        data = np.frombuffer(self.stream.read(CHUNK, exception_on_overflow=0),
            dtype=np.int16)
        results = np.fft.rfft(data)
        freqs = np.fft.rfftfreq(len(data), d=1.0/RATE)
        # extract frequencies associated with FFT values
        top_N = np.argsort(np.abs(results))[-N:][::-1]

        return freqs[top_N], 20 * np.log10(np.abs(results)[top_N])


    # render img in texts
    def render(self, texts, img, threshold=300):
        w, h, _ = img.shape

        for i in range(w)[::font_px]:
            for j in range(h)[::font_px]:
                if sum(img[j][i][0:3]) > threshold:
                    text = random.choice(texts)
                    self.winsur.blit(text, ((self.width-w)//2+i, j+(self.height-h)//2))


    # main display loop
    def display(self):
        drops = [0 for _ in range(int(self.width / font_px))]

        img = random.choice(self.imgs)

        start = -np.inf
        count = 0
        self.running = 1

        while self.running:
            count += 1


            texts = [self.font.render(LETTER[i], 1, (self.r, self.g, self.b)) for i in range(44)]

            # press space to quit
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = 0
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_SPACE:
                        self.running = 0

            pygame.time.delay(self.delay) # text fading

            # cursed, but just in case
            try:
                freqs, vols = self.read_audio()
                # print(max(vols))
            except OSError as e:
                self.stream_init()
                print(count, e)
                continue

            if not self.rendering:
                if max(vols) > 120 or count%420 == 0: # replace by condition for img to show up (volume > ?)
                    self.rendering = 1
                    img = random.choice(self.imgs)
                    start = count

            if count < start + DURATION:
                self.render(texts, img)
            else:
                self.rendering = 0

            # fading effect
            self.winsur.blit(self.bg, (0, 0))
            try:
                self.bg.fill(pygame.Color(0, 0, 0, self.fade))
            except ValueError as e:
                print(self.fade)

            # display text drops
            for i in range(len(drops)):
                text = random.choice(texts)
                self.winsur.blit(text, (i * font_px, drops[i] * font_px))
                drops[i] += 1
                if drops[i] * 10 > self.height-200 or random.random() > 0.95:
                    drops[i] = 0

            pygame.display.flip()
            self.update()

    # modify me for different effects
    def update(self, freqs=None, vols=None):
        self.delay = (self.delay + random.randint(-Δ, Δ)) % 69
        self.fade = max(1, min((self.fade + random.randint(-Δ, Δ)), 200))
        if freqs is None:
            # random
            self.r = (self.r + random.randint(-Δ, Δ)) % 256
            self.g = (self.g + random.randint(-Δ, Δ)) % 256
            self.b = (self.b + random.randint(-Δ, Δ)) % 256
        else:
            print(freqs)
            for freq in freqs:
                # rgb <-> freqs
                if freq < 246: # low (< 246Hz)
                    self.r = int(freq * 256 / 246)
                elif 246 <= freq < 2500: # mid (246 ~ 2500Hz)
                    self.g = int((freq-246) * 256 / 2254)
                else: # hi (> 2500Hz)
                    self.b = int((freq-2500) * 256 / 7500)




# ------------------------------MULTITHREADING--------------------------------
# it lags too much

# import threading

# class Display(object):
#     def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
#         self.r = r
#         self.g = g
#         self.b = b
#         self.delay = delay
#         self.fade = fade
#         self.running = 0
#         self.update_thread = None

#     def display(self):
#         font_px = 15
#         pygame.init()
#         screen_info = pygame.display.Info()
#         width = screen_info.current_w
#         height = screen_info.current_h

#         winsur = pygame.display.set_mode((width, height))
#         font = pygame.font.SysFont('andalemono', 21)
#         bg = pygame.Surface((width, height), pygame.SRCALPHA, 32).convert_alpha()
#         screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN)
#         winsur.fill((0, 0, 0))

#         LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'

#         column = int(width / font_px)
#         drops = [0 for _ in range(column)]

#         self.running = 1
#         self.update_thread = threading.Thread(target=self.update_loop)
#         self.update_thread.start()

#         while self.running:
#             bg.fill(pygame.Color(0, 0, 0, self.fade))
#             texts = [font.render(LETTER[i], 1, (self.r, self.g, self.b)) for i in range(44)]
#             for event in pygame.event.get():
#                 if event.type == pygame.QUIT:
#                     self.running = 0
#                 elif event.type == pygame.KEYDOWN:
#                     if event.key == pygame.K_SPACE:
#                         self.running = 0

#             pygame.time.delay(self.delay)

#             winsur.blit(bg, (0, 0))

            # for i in range(len(drops)):
            #     text = random.choice(texts)
            #     winsur.blit(text, (i * font_px, drops[i] * font_px))
            #     drops[i] += 1
            #     if drops[i] * 10 > height or random.random() > 0.95:
            #         drops[i] = 0

#             pygame.display.flip()

#         self.update_thread.join()

#     def update_loop(self):
#         while self.running:
#             self.update()

#     def update(self):
#         self.r = (self.r + random.randint(-Δ, Δ)) % 256
#         self.g = (self.g + random.randint(-Δ, Δ)) % 256
#         self.b = (self.b + random.randint(-Δ, Δ)) % 256
#         self.delay = self.delay + random.randint(-Δ, Δ) % 69
#         self.fade = (self.fade + random.randint(-Δ, Δ)) % 200


# ------------------------------INTERUPT--------------------------------

# import time

# class Display(object):
#     def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
#         self.r = r
#         self.g = g
#         self.b = b
#         self.delay = delay
#         self.fade = fade
#         self.running = 0
#         self.last_update_time = time.time()
#         self.update_interval = 1

#     def display(self):
#         font_px = 15
#         pygame.init()
#         screen_info = pygame.display.Info()
#         width = screen_info.current_w
#         height = screen_info.current_h

#         winsur = pygame.display.set_mode((width, height))
#         font = pygame.font.SysFont('andalemono', 21)
#         bg = pygame.Surface((width, height), pygame.SRCALPHA, 32).convert_alpha()
#         screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN) # full screen
#         winsur.fill((0, 0, 0))

#         LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'

#         column = int(width / font_px)
#         drops = [0 for _ in range(column)]

#         self.running = 1
#         while self.running:
#             texts = [font.render(LETTER[i], 1, (self.r, self.g, self.b)) for i in range(44)]

#             for event in pygame.event.get():
#                 if event.type == pygame.QUIT:
#                     self.running = 0
#                 elif event.type == pygame.KEYDOWN:
#                     if event.key == pygame.K_SPACE:
#                         self.running = 0
#                     else:
#                         self.handle_update_interrupt()

#             pygame.time.delay(self.delay)
#             bg.fill(pygame.Color(0, 0, 0, self.fade))

#             winsur.blit(bg, (0, 0))

#             for i in range(len(drops)):
#                 text = random.choice(texts)
#                 winsur.blit(text, (i * font_px, drops[i] * font_px))
#                 drops[i] += 1
#                 if drops[i] * 10 > height or random.random() > 0.95:
#                     drops[i] = 0

#             pygame.display.flip()

#     def handle_update_interrupt(self):
#         if time.time() - self.last_update_time >= self.update_interval:
#             self.last_update_time = time.time()
#             self.update()

#     def update(self):
#         self.r = (self.r + random.randint(-Δ, Δ)) % 256
#         self.g = (self.g + random.randint(-Δ, Δ)) % 256
#         self.b = (self.b + random.randint(-Δ, Δ)) % 256
#         self.delay = (self.delay + random.randint(-Δ, Δ)) % 69
#         self.fade = (self.fade + random.randint(-Δ, Δ)) % 200


# -----------------------------------------------------------------------
if __name__ == '__main__':
    Display(imgs=["logo.jpg"]).display()
