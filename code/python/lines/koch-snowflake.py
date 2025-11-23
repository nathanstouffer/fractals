import turtle
from math import sqrt

third = 0.333333333333333
ang = 60

def segment(turt, mag, height):
    if (height == 0):
        draw(turt, mag)
    else:
        segment(turt, third*mag, height-1)
        turt.left(ang)
        segment(turt, third*mag, height-1)
        turt.right(2*ang)
        segment(turt, third*mag, height-1)
        turt.left(ang)
        segment(turt, third*mag, height-1)

def draw(turt, mag):
    turt.forward(mag)
    turt.left(ang)
    turt.forward(mag)
    turt.right(2*ang)
    turt.forward(mag)
    turt.left(ang)
    turt.forward(mag)

mag = 250
screen = turtle.Screen()
screen.setup(1920, 1080)
turtle.bgcolor('gray')

turt = turtle.Turtle(visible=False)
turt.penup()
turt.pensize(1)
color = 'purple'
turt.color(color, color)
turt.setx(-1.5*mag)
turt.sety(-(sqrt(3)/6)*3*mag)
turt.pendown()
turt.speed(0)

degree = 3
turt.begin_fill()
turt.left(60)
segment(turt, mag, degree)
turt.right(120)
segment(turt, mag, degree)
turt.right(120)
segment(turt, mag, degree)
turt.end_fill()

screen.tracer(True)
canvas = screen.getcanvas()
#canvas.postscript(file='tmp.eps', width=1920, height=1080)

turtle.done()
