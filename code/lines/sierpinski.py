import turtle
from math import sqrt

half = 0.5

class Triangle:

    def __init__(self, top, side):
        self.x = [ top[0], top[0]-(1/2)*side,       top[0]+(1/2)*side ]
        self.y = [ top[1], top[1]-(sqrt(3)/2)*side, top[1]-(sqrt(3)/2)*side ]

    def first(self):
        return [self.x[0], self.y[0]]

    def second(self):
        return [self.x[1], self.y[1]]

    def third(self):
        return [self.x[2], self.y[2]]

def sierpinski(turt, top, side, height):
    if (height == 0):
        triang = Triangle(top, side)
        draw(turt, triang)
    else:
        triang = Triangle(top, half*side)
        sierpinski(turt, triang.first(),  half*side, height-1)
        sierpinski(turt, triang.second(), half*side, height-1)
        sierpinski(turt, triang.third(),  half*side, height-1)

def draw(turt, triang):
    turt.penup()
    turt.goto(triang.first()[0],  triang.first()[1])
    turt.pendown()
    turt.goto(triang.second()[0], triang.second()[1])
    turt.goto(triang.third()[0],  triang.third()[1])
    turt.goto(triang.first()[0],  triang.first()[1])

side = 1000
screen = turtle.Screen()
screen.setup(1920, 1080)
turtle.bgcolor('black')

turt = turtle.Turtle(visible=False)
turt.penup()
turt.pensize(1)
turt.pencolor('white')
turt.pendown()
turt.speed(0)

yshift = -0.4*side
sierpinski(turt, [0, sqrt(3)/2*side+yshift], side, 5)

screen.tracer(True)
canvas = screen.getcanvas()

turtle.done()
