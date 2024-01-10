import re
import os

patterns = ["TODO", "FIXME"]

def audit(filename):
  with open(filename, 'r') as file:
    ret = {}
    line_number = 0
    todo_list = []
    fixme_list = []

    for line in file:
      for p in patterns:
        if re.search(p, line):
          tag = p
          line = re.sub(p, '', line)
          line = line.split('//')[1]
          line = line.strip()
          if p == "TODO":
            todo_list.append((line_number, line))
          elif p == "FIXME":
            fixme_list.append((line_number, line))

      line_number += 1
    
    ret["FILENAME"] = filename
    ret["TODO"] = todo_list
    ret["FIXME"] = fixme_list 
    ret["LINES"] = line_number 
  return ret 
  
if __name__ == "__main__":
  dirs = os.listdir()
  for d in dirs:
    if d[-2:] == '.c':
      res = audit(d)
      print("FILENAME: ", res["FILENAME"]," --- ", res["LINES"], 'lines')


      if (len(res["TODO"]) > 0):
        print("--------- TODO ----------")
        for t in res["TODO"]:
          print(t[0], ": ", t[1], '\n')

      if (len(res["FIXME"]) > 0):
        print("--------- FIXME ----------")
        for t in res["FIXME"]:
          print(t[0], ": ", t[1], '\n')


          

              
              











