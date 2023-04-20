#!../../cgi_bin/python3.7
import random

# Génère une couleur aléatoire en format hexadécimal
def random_color():
    r = random.randint(0, 255)
    g = random.randint(0, 255)
    b = random.randint(0, 255)
    return "#{:02x}{:02x}{:02x}".format(r, g, b)

# Génère une liste de points aléatoires
def generate_points(num_points):
    points = []
    for i in range(num_points):
        x = random.randint(0, 800)
        y = random.randint(0, 600)
        color = random_color()
        points.append((x, y, color))
    return points

# Génère une liste de lignes aléatoires
def generate_lines(num_lines):
    lines = []
    for i in range(num_lines):
        x1 = random.randint(0, 800)
        y1 = random.randint(0, 600)
        x2 = random.randint(0, 800)
        y2 = random.randint(0, 600)
        color = random_color()
        lines.append((x1, y1, x2, y2, color))
    return lines

# Génère une page web qui affiche les points et les lignes
def generate_webpage(points, lines):
    html = "<html><head><title>Points and Lines</title></head><body>"
    html += "<svg width='800' height='600'>"
    for point in points:
        html += "<circle cx='{}' cy='{}' r='5' fill='{}' />".format(point[0], point[1], point[2])
    for line in lines:
        html += "<line x1='{}' y1='{}' x2='{}' y2='{}' stroke='{}' stroke-width='2' />".format(line[0], line[1], line[2], line[3], line[4])
    html += "</svg></body></html>"
    return html

# Génère 50 points et 30 lignes aléatoires et affiche la page web correspondante
points = generate_points(50)
lines = generate_lines(30)
webpage = generate_webpage(points, lines)
print(webpage)
