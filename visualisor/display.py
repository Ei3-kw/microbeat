import random
import pygame

font_px = 15

pygame.init()
screen_info = pygame.display.Info()
width = screen_info.current_w
height = screen_info.current_h

winsur = pygame.display.set_mode((width, height))
font = pygame.font.SysFont('andalemono', 21)
bg = pygame.Surface((width, height), flags=pygame.SRCALPHA)
screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN)

pygame.Surface.convert(bg)
bg.fill(pygame.Color(0, 0, 0, 28))
winsur.fill((0, 0, 0))

LETTER = '1234567890!@#$%^&*qwertyuiopasdfghjklzxcvbnm'
texts = [font.render(LETTER[i], True, (0, 255, 0)) for i in range(44)]

column = int(width / font_px)
drops = [0 for i in range(column)]
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()
        elif event.type == pygame.KEYDOWN:
            change = pygame.key.get_pressed()
            if change[32]:
                exit()

    pygame.time.delay(30)

    winsur.blit(bg, (0, 0))

    for i in range(len(drops)):
        text = random.choice(texts)
        winsur. blit(text, (i * font_px, drops[i] * font_px))
        drops[i] += 1
        if drops[i] * 10 > height or random.random() > 0.95:

            drops[i] = 0
    
    pygame.display.flip()