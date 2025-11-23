import turtle
from math import sqrt

half = 0.5

class Triangle:

    def __init__(self, top, side):
        if (type(side) is list):
            self.x = top
            self.y = side
        else:
            self.top = top
            self.side = side
            self.x = [ top[0], top[0]+(1/2)*side,       top[0]-(1/2)*side ]
            self.y = [ top[1], top[1]-(sqrt(3)/2)*side, top[1]-(sqrt(3)/2)*side ]

    def v1(self):
        return [self.x[0], self.y[0]]

    def v2(self):
        return [self.x[1], self.y[1]]

    def v3(self):
        return [self.x[2], self.y[2]]

    def inside(self):
        bottom = [ self.x[0], self.top[1]-(sqrt(3)/2*self.side) ]
        right  = self.mid(self.v1(), self.v2())
        left   = self.mid(self.v1(), self.v3())
        xi = [ bottom[0], right[0], left[0] ]
        yi = [ bottom[1], right[1], left[1] ]
        return Triangle(xi, yi)

    def mid(self, p1, p2):
        return [ (p1[0]+p2[0])/2.0, (p1[1]+p2[1])/2.0 ]

def sierpinski(turt, top, side, height):
    if (height > 0):
        triang = Triangle(top, side)
        inside = triang.inside()
        draw(turt, inside)
        triang = Triangle(top, half*side)
        sierpinski(turt, triang.v1(), half*side, height-1)
        sierpinski(turt, triang.v2(), half*side, height-1)
        sierpinski(turt, triang.v3(), half*side, height-1)

def draw(turt, triang):
    turt.penup()
    turt.goto(triang.v1()[0], triang.v1()[1])
    turt.pendown()
    turt.goto(triang.v2()[0], triang.v2()[1])
    turt.goto(triang.v3()[0], triang.v3()[1])
    turt.goto(triang.v1()[0], triang.v1()[1])

side = 1000
screen = turtle.Screen()
screen.setup(1920, 1080)
turtle.bgcolor('black')

turt = turtle.Turtle(visible=False)
turt.penup()
turt.pensize(1)
turt.pencolor('orange')
turt.pendown()
turt.speed(0)

yshift = -0.4*side

sierpinski(turt, [0, sqrt(3)/2*side+yshift], side, 6)

big_t = Triangle([0, sqrt(3)/2*side+yshift], side)
draw(turt, big_t)

screen.tracer(True)
canvas = screen.getcanvas()

turtle.done()
