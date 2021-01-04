import turtle

third = 0.333333333333333
ang = 60

def seg(turt, mag, height):
    if (height == 0):
        draw(turt, mag)
    else:
        seg(turt, third*mag, height-1)
        turt.left(ang)
        seg(turt, third*mag, height-1)
        turt.right(2*ang)
        seg(turt, third*mag, height-1)
        turt.left(ang)
        seg(turt, third*mag, height-1)

def draw(turt, mag):
    turt.forward(mag)
    turt.left(ang)
    turt.forward(mag)
    turt.right(2*ang)
    turt.forward(mag)
    turt.left(ang)
    turt.forward(mag)

mag = 500
screen = turtle.Screen()
screen.setup(1920, 1080)
turtle.bgcolor('black')

turt = turtle.Turtle(visible=False)
turt.penup()
turt.pensize(1)
turt.pencolor('red')
turt.setx(-1.5*mag)
turt.sety(-0.3*mag)
turt.pendown()
turt.speed(0)

seg(turt, mag, 4)

screen.tracer(True)
canvas = screen.getcanvas()
#canvas.postscript(file='tmp.eps', width=1920, height=1080)

turtle.done()
