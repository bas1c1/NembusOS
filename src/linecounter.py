import os
ar = os.listdir("impl") + os.listdir("include") + os.listdir("kernel")
count = 0

for i in os.listdir("impl"):
  f = open("impl/" + i, "r")
  count += len(f.readlines())
  f.close()

for i in os.listdir("include"):
  f = open("include/" + i, "r")
  count += len(f.readlines())
  f.close()

for i in os.listdir("kernel"):
  f = open("kernel/" + i, "r")
  count += len(f.readlines())
  f.close()

print(count)
