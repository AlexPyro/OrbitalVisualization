import matplotlib.pyplot as plt

# Listes pour stocker les valeurs de x et y
t_values = []
x_values = []
y_values = []
z_values = []

# Ouvrir le fichier texte en mode lecture
with open('C:\\Users\\vmn\\Documents\\PIR_comparaison\\smoothrefpos.txt','r') as file:
          for line in file:
        # Diviser la ligne en utilisant des espaces comme séparateurs
                    parts = line.split()
                    if len(parts) >= 7:
            # Extraire les valeurs de la cinquième et sixième colonnes
                              t = float(parts[2])
                              x = float(parts[4])
                              y = float(parts[5])
                              z = float(parts[6])
                              x_values.append(x)
                              y_values.append(y)
                              z_values.append(z)
                              t_values.append(t)


#ouverture du deuxième fichier
# Listes pour stocker les valeurs du deuxième fichier
t_values2 = []
x_values2 = []
y_values2 = []
z_values2 = []
# Ouvrir le deuxième fichier texte en mode lecture
with open('C:\\Users\\vmn\\Documents\\PIR_comparaison\\smooth1pos.txt', 'r') as file2:
    for line in file2:
        parts = line.split()
        if len(parts) >= 7:
            t = float(parts[2])
            x = float(parts[4])
            y = float(parts[5])
            z = float(parts[6])
            x_values2.append(x)
            y_values2.append(y)
            z_values2.append(z)
            t_values2.append(t)

# Tracer le graphique
plt.plot(x_values2, y_values2, label='Fichier 2', color='blue')
plt.plot(x_values, y_values, label='Fichier 1', color='red')


plt.xlabel('x')
plt.ylabel('y')
plt.axis('equal')
plt.title('Trajectoire de l orbite du satellite en une journée projeté selon x et y')
plt.grid(True)
plt.show()
