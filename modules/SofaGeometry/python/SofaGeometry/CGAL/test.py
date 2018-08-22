import point2D

point1 = point2D.Point2(1.0, 5.0)
point2 = point2D.Point2(1.0, 3.0)
point1_bis = point2D.Point2(point1)
print(str(point1.x()) + "   " + str(point2.x()))
print(str(point1.y()) + "   " + str(point2.y()))

print("est equality  " + str(point2D.smaller(point1, point2)))


