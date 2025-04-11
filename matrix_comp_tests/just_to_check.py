import numpy as np 
import time
import os

#this will be sooo slow ahhh

def matrix_from_file(i):
  f = open("result/matrix"+str(i)+".conf", "r")

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

def array_from_file(path):
  f = open(path, "r")

  line = f.readline()
  elements = line.replace('\n','').split('-')
  vector = []
  for i in range(len(elements)):
    x = int(elements[i])
    vector.append(x)

  f.close()
  np_vector = np.array(vector)
  return np_vector

def result_from_file(i):
  path = "./result/result"+str(i)+".conf"
  return array_from_file(path)

def vector_form_file(i):
  path = "./result/vector"+str(i)+".conf"
  return array_from_file(path)

def check_result(i):

  start = time.time_ns()

  matrix = matrix_from_file(i) 
  vector = vector_form_file(i)
  result = result_from_file(i)

  end = time.time_ns()
  print("Time taken to read the matrix, vector and result from file: ", (end-start), "ns")

  start = time.time_ns()
  

  start = time.time_ns()
  except_res = matrix.dot(vector)
  end = time.time_ns()
  print(result)
  print(except_res)
  print("Time taken to multiply the matrix and vector: ", (end-start), "ns")
  res = True
  for i in range(len(result)):
    if(result[i] != except_res[i]):
      res = False
      break
  
  print("Result is: ", res)

if __name__ == "__main__":
  
  for i in range(1,1+len(os.listdir('./result'))//3):
    check_result(i)
