import sys, pygame
pygame.init()

size = width, height = 500, 300
red = (255, 60, 60)
red_light = (255, 125, 125)
green = (60, 255, 60)
green_light = (125, 255, 125)
blue = (60, 60, 255)
blue_light = (125, 125, 255)
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

    smallText = pygame.font.SysFont("comicsansms",20)
    textSurf, textRect = text_objects("Choose your item", smallText)
    textRect.center = (width/2, height/2-70) 
    screen.blit(textSurf, textRect)

    b1_enabled = button("Item 1", 110, 180, 80, 50, red, red_light, b1_enabled, action=handle_click1)
    b2_enabled = button("Item 2", 210, 180, 80, 50, green, green_light, b2_enabled, action=handle_click2)
    b3_enabled = button("Item 3", 310, 180, 80, 50, blue, blue_light, b3_enabled, action=handle_click3)

    # pygame.draw.rect(screen, red, (100, 450, 80, 50))
    # pygame.draw.rect(screen, green, (200, 450, 80, 50))
    # pygame.draw.rect(screen, blue, (300, 450, 80, 50))

    pygame.display.update()
    clock.tick(15)