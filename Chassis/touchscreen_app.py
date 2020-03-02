import sys, pygame
pygame.init()

size = width, height = 500, 500
red = (255, 0, 0)
green = (0, 255, 0)
blue = (0, 0, 255)
white = (255, 255, 255)
black = (0, 0, 0)

screen = pygame.display.set_mode(size)

clock = pygame.time.Clock()

def text_objects(text, font):
    textSurface = font.render(text, True, black)
    return textSurface, textSurface.get_rect()

def handle_click1():
    print("hi")

def handle_click2():
    print("what")

def handle_click3():
    print("bye")    

def button(msg, x, y, w, h, ic, ac, enabled, action=None):
    mouse = pygame.mouse.get_pos()
    click = pygame.mouse.get_pressed()
    # print(click)
    if x+w > mouse[0] > x and y+h > mouse[1] > y:
        pygame.draw.rect(screen, ac,(x,y,w,h))

        if click[0] == 1 and action != None and enabled:
            action()
            enabled = False
        elif click[0] == 0:
            enabled = True
    else:
        pygame.draw.rect(screen, ic,(x,y,w,h))
        enabled = True

    smallText = pygame.font.SysFont("comicsansms",20)
    textSurf, textRect = text_objects(msg, smallText)
    textRect.center = ( (x+(w/2)), (y+(h/2)) )
    screen.blit(textSurf, textRect)

    return enabled

b1_enabled = True
b2_enabled = True
b3_enabled = True

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    screen.fill(white)

    b1_enabled = button("button1", 100, 400, 80, 50, red, black, b1_enabled, action=handle_click1)
    b2_enabled = button("button2", 200, 400, 80, 50, green, black, b2_enabled, action=handle_click2)
    b3_enabled = button("button3", 300, 400, 80, 50, blue, black, b3_enabled, action=handle_click3)

    # pygame.draw.rect(screen, red, (100, 450, 80, 50))
    # pygame.draw.rect(screen, green, (200, 450, 80, 50))
    # pygame.draw.rect(screen, blue, (300, 450, 80, 50))

    pygame.display.update()
    clock.tick(15)