# import random
# import pygame

# class Display(object):
#     def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
#         self.r = r
#         self.g = g
#         self.b = b
#         self.delay = delay
#         self.fade = fade
#         self.running = False

#     def display(self):
#         font_px = 15
#         pygame.init()
#         screen_info = pygame.display.Info()
#         width = screen_info.current_w
#         height = screen_info.current_h

#         winsur = pygame.display.set_mode((width, height))
#         font = pygame.font.SysFont('andalemono', 21)
#         bg = pygame.Surface((width, height), pygame.SRCALPHA, 32).convert_alpha()
#         # screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN)
#         winsur.fill((0, 0, 0))

#         LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'

#         column = int(width / font_px)
#         drops = [0 for _ in range(column)]

#         self.running = True
#         while self.running:
            # print(self.r, self.g, self.b, self.delay, self.fade)
            # texts = [font.render(LETTER[i], True, (self.r, self.g, self.b)) for i in range(44)]
#             for event in pygame.event.get():
#                 if event.type == pygame.QUIT:
#                     self.running = False
#                 elif event.type == pygame.KEYDOWN:
#                     if event.key == pygame.K_SPACE:
#                         self.running = False

#             pygame.time.delay(self.delay)

#             winsur.blit(bg, (0, 0))
#             bg.fill(pygame.Color(0, 0, 0, self.fade))

#             for i in range(len(drops)):
#                 text = random.choice(texts)
#                 winsur.blit(text, (i * font_px, drops[i] * font_px))
#                 drops[i] += 1
#                 if drops[i] * 10 > height or random.random() > 0.95:
#                     drops[i] = 0

#             pygame.display.flip()
#             self.update()

#     def update(self):
#         self.r = (self.r + random.randint(-1, 1)) % 256
#         self.g = (self.g + random.randint(-1, 1)) % 256
#         self.b = (self.b + random.randint(-1, 1)) % 256
#         self.delay = self.delay + random.randint(-1, 1) % 69
#         self.fade = (self.fade + random.randint(-1, 1)) % 256

# if __name__ == '__main__':
#     d = Display()
#     d.display()



# ------------------------------MULTITHREADING--------------------------------


# import random
# import pygame
# import threading

# class Display(object):
#     def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
#         self.r = r
#         self.g = g
#         self.b = b
#         self.delay = delay
#         self.fade = fade
#         self.running = False
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

#         self.running = True
#         self.update_thread = threading.Thread(target=self.update_loop)
#         self.update_thread.start()

#         while self.running:
#             bg.fill(pygame.Color(0, 0, 0, self.fade))
#             texts = [font.render(LETTER[i], True, (self.r, self.g, self.b)) for i in range(44)]
#             for event in pygame.event.get():
#                 if event.type == pygame.QUIT:
#                     self.running = False
#                 elif event.type == pygame.KEYDOWN:
#                     if event.key == pygame.K_SPACE:
#                         self.running = False

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
#         self.r = (self.r + random.randint(-1, 1)) % 256
#         self.g = (self.g + random.randint(-1, 1)) % 256
#         self.b = (self.b + random.randint(-1, 1)) % 256
#         self.delay = self.delay + random.randint(-1, 1) % 256
#         self.fade = (self.fade + random.randint(-1, 1)) % 256

# if __name__ == '__main__':
#     d = Display()
#     d.display()


# ------------------------------INTERUPT--------------------------------

import random
import pygame
import time

class Display(object):
    def __init__(self, r=0, g=255, b=0, delay=30, fade=21):
        self.r = r
        self.g = g
        self.b = b
        self.delay = delay
        self.fade = fade
        self.running = False
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
        # screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN) # full screen
        winsur.fill((0, 0, 0))

        LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'

        column = int(width / font_px)
        drops = [0 for _ in range(column)]

        self.running = True
        while self.running:
            texts = [font.render(LETTER[i], True, (self.r, self.g, self.b)) for i in range(44)]

            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.running = False
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_SPACE:
                        self.running = False
                    else:
                        self.handle_update_interrupt()

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

    def handle_update_interrupt(self):
        if time.time() - self.last_update_time >= self.update_interval:
            self.last_update_time = time.time()
            self.update()

    def update(self):
        self.r = (self.r + random.randint(-1, 1)) % 256
        self.g = (self.g + random.randint(-1, 1)) % 256
        self.b = (self.b + random.randint(-1, 1)) % 256
        self.delay = (self.delay + random.randint(-1, 1)) % 69
        self.fade = (self.fade + random.randint(-1, 1)) % 256

if __name__ == '__main__':
    d = Display()
    d.display()