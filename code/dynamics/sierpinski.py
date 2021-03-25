import turtle
from random import randint
from math import sqrt

scale = 400
num_iter = 2500

def midpoint(p, q):
    return [(p[0]+q[0])/2, (p[1]+q[1])/2]

def draw(turt, pnt):
    turt.goto(scale*pnt[0], scale*pnt[1])
    turt.dot()

screen = turtle.Screen()
screen.setup(1920, 1080)
turtle.bgcolor('green')

turt = turtle.Turtle(visible=False)
turt.penup()
turt.pensize(0.5)
turt.pencolor('pink')
turt.speed(0)

triang = [ [-1,-1+0.3],
           [1,-1+0.3],
           [0,sqrt(3)-1+0.3] ]

for point in triang:
    draw(turt, point)

seed = triang[2]
cur  = seed
for i in range(0, num_iter):
    rnd = randint(0,2)
    cur = midpoint(cur, triang[rnd])
    draw(turt, cur)

print("done")
canvas = screen.getcanvas()

turtle.done()
