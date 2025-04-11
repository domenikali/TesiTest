import numpy as np 

#this will be sooo slow ahhh

def matrix_from_file():
  f = open("matrix.conf", "r")

  line = f.readline()
  matrix = []
  while line:
    row = line.split('-')
    matrix_row = []

    for i in range(len(row)):
      wheigts = row[i].split(' ')
      weight = 1
      for j in range(len(wheigts)):
        weight *= int(wheigts[j].replace('\n',''))

      matrix_row.append(weight)
    matrix.append(matrix_row)
    line = f.readline()

  f.close()
  np_matrix = np.array(matrix)
  return np_matrix

def vector_from_file():
  f = open("vector.conf", "r")

  line = f.readline()
  elements = line.split('-')
  vector = []
  for i in range(len(elements)):
    x = int(elements[i].replace('\n',''))
    vector.append(x)

  f.close()
  np_vector = np.array(vector)
  return np_vector

matrix = matrix_from_file() 
vector = vector_from_file()

print(matrix)
print(vector)
print(matrix.dot(vector))